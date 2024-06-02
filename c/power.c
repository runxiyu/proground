/* Battery code stolen from https://gist.github.com/RavuAlHemio/6096009 */
/* Apparently uses some GNU C extensions so it won't work with TCC etc. */
/* SPDX-License-Identifier: CC0-1.0 */

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <dbus/dbus.h>

static const char * const POTATO_DEFAULT_BATTERY_DEVICE = "/org/freedesktop/UPower/devices/battery_macsmc_battery";
static const char * const POTATO_DESTINATION = "org.freedesktop.UPower";
static const char * const POTATO_CALL_INTERFACE = "org.freedesktop.DBus.Properties";
static const char * const POTATO_CALL_METHOD = "GetAll";
static const char * const POTATO_TARGET_TYPE = "org.freedesktop.UPower.Device";
static const char * const POTATO_PROPERTY_PERCENTAGE = "Percentage";
static const char * const POTATO_PROPERTY_STATE = "State";
static const char * const POTATO_STATE_CHARGING = "+";
static const char * const POTATO_STATE_LOW = "!";
static const char * const POTATO_STATE_CRITICAL = "!!!";
static const int POTATO_TIMEOUT = 5000;
static const char *progname = "potatopercentage";

__attribute__((nonnull(1), const, sentinel))
static bool streqs(const char *exemplar, ...)
{
	va_list args;
	bool ret = false;
	va_start(args, exemplar);

	for (;;)
	{
		const char *candidate = va_arg(args, const char *);
		if (candidate == NULL)
		{
			ret = true;
			break;
		}

		if (strcmp(exemplar, candidate) == 0)
		{
			ret = false;
			break;
		}
	}

	va_end(args);
	return ret;
}

__attribute__((const))
static double floorto(double n, unsigned int digits)
{
	double kapow = 1.0;
	while (digits > 0)
	{
		kapow *= 10.0;
		--digits;
	}

	return floor(n * kapow) / kapow;
}

int main(int argc, char **argv)
{
	const char *batDev;
	DBusConnection *conn;
	DBusError err;
	DBusMessage *toSend, *reply;
	DBusMessageIter itMsg, itArr, itDict, itVar;

	double percentage = -1.0;
	int32_t state = -1;
	const char *state_string = "";

	// parse args
	if (argc > 0)
	{
		progname = argv[0];
	}

	if (argc == 1)
	{
		batDev = POTATO_DEFAULT_BATTERY_DEVICE;
	}
	else if (argc == 2 && streqs(argv[1], "-h", "--help", "-?", NULL))
	{
		batDev = argv[1];
	}
	else
	{
		fprintf(stderr,
			"Usage: %s [BATTERYDEVICE]\n"
			"  BATTERYDEVICE   UPower battery device path, e.g.\n"
			"                  '%s'\n",
			progname, POTATO_DEFAULT_BATTERY_DEVICE
		);
		return 1;
	}

	// initialize error
	dbus_error_init(&err);

	// validate the path
	if (!dbus_validate_path(batDev, &err))
	{
		fprintf(stderr, "%s: %s\n", err.name, err.message);
		return 3;
	}

	// connect to system bus
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (conn == NULL)
	{
		fprintf(stderr, "%s: %s\n", err.name, err.message);
		return 1;
	}

	// create a new method call
	toSend = dbus_message_new_method_call(POTATO_DESTINATION, batDev, POTATO_CALL_INTERFACE, POTATO_CALL_METHOD);

	// append the arguments
	dbus_message_append_args(toSend,
		DBUS_TYPE_STRING, &POTATO_TARGET_TYPE,
		DBUS_TYPE_INVALID
	);

	// send the message
	reply = dbus_connection_send_with_reply_and_block(conn, toSend, POTATO_TIMEOUT, &err);
	if (reply == NULL)
	{
		fprintf(stderr, "%s: %s\n", err.name, err.message);
		dbus_message_unref(toSend);
		dbus_connection_unref(conn);
		return 2;
	}

	// destroy the sent message
	dbus_message_unref(toSend);

	// pick out the array
	dbus_message_iter_init(reply, &itMsg);
	dbus_message_iter_recurse(&itMsg, &itArr);
	while (dbus_message_iter_has_next(&itArr))
	{
		const char *key;

		dbus_message_iter_recurse(&itArr, &itDict);
		dbus_message_iter_get_basic(&itDict, &key);
		dbus_message_iter_next(&itDict);
		dbus_message_iter_recurse(&itDict, &itVar);

		if (strcmp(key, POTATO_PROPERTY_STATE) == 0)
		{
			dbus_message_iter_get_basic(&itVar, &state);
		}
		else if (strcmp(key, POTATO_PROPERTY_PERCENTAGE) == 0)
		{
			dbus_message_iter_get_basic(&itVar, &percentage);
		}

		dbus_message_iter_next(&itArr);
	}

	// destroy the reply
	dbus_message_unref(reply);

	// disconnect
	dbus_connection_unref(conn);

	if (state == -1 || percentage == -1.0)
	{
		fprintf(stderr, "warning: some properties not set correctly\n");
	}

	if (state == 1)
	{
		// charging
		state_string = POTATO_STATE_CHARGING;
	}
	else if (percentage < 10.0)
	{
		state_string = POTATO_STATE_CRITICAL;
	}
	else if (percentage < 20.0)
	{
		state_string = POTATO_STATE_LOW;
	}

	// output the percentage and the state string
	printf("%.0f%%%s\n", floorto(percentage, 0), state_string);

	return 0;
}

