// Hax's string utils
//
// Written by: Test_User <hax@andrewyu.org>
//
// This is free and unencumbered software released into the public
// domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "haxstring.h"
#include "haxstring_utils.h"

size_t str_to_unsigned(struct string str, char *err)
{
	if (str.len == 0) {
		*err = 1;
		return 0;
	}

	size_t val = 0;
	while (str.len > 0) {
		switch (str.data[0]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (val > ((size_t)-1) / 10) {
				*err = 1;
				return 0;
			}
			val *= 10;
			if (val >
			    (-
			     ((size_t)((unsigned char)str.data[0] - 0x30) +
			      1))) {
				*err = 1;
				return 0;
			}
			val += (unsigned char)str.data[0] - 0x30;
			break;
		default:
			*err = 1;
			return 0;
		}

		str.data++;
		str.len--;
	}

	*err = 0;
	return val;
}

int unsigned_to_str(size_t number, struct string *str)
{
	size_t len = 0;
	{
		size_t tmp = number;
		do {
			len++;
		} while ((tmp = tmp / 10) != 0);
	}

	void *tmp = malloc(len);
	if (!tmp)
		return 1;

	str->data = tmp;
	str->len = len;

	for (size_t i = len; i > 0; i--) {
		str->data[i - 1] = (char)((number % 10) + 0x30);
		number = number / 10;
	}

	return 0;
}

int str_clone(struct string *dest, struct string source)
{
	dest->data = malloc(source.len);
	if (!dest->data)
		return 1;
	memcpy(dest->data, source.data, source.len);
	dest->len = source.len;

	return 0;
}

int str_combine(struct string *dest, size_t count, struct string *sources)
{
	size_t len = 0;
	for (size_t i = 0; i < count; i++)
		len += sources[i].len;

	dest->data = malloc(len);
	if (!dest->data && len != 0)
		return 1;
	dest->len = len;

	size_t offset = 0;
	for (size_t i = 0; i < count; i++) {
		memcpy(&(dest->data[offset]), sources[i].data, sources[i].len);
		offset += sources[i].len;
	}

	return 0;
}
