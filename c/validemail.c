/*
 * This was copied from atheme/libathemecore/email.c
 *
 * Copyright (c) 2021 Aaron Jones <me@aaronmdjones.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHORS DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>

int validemail(const char *email)
{
	int i, valid = 1, chars = 0, atcnt = 0, dotcnt1 = 0;
	char c;
	const char *lastdot = NULL;

	/* sane length */
	if (strlen(email) > 50)
		return 0;

	/* RFC2822 */
#define EXTRA_ATEXTCHARS "!#$%&'*+-/=?^_`{|}~"

	/* note that we do not allow domain literals or quoted strings */
	for (i = 0; email[i] != '\0'; i++) {
		c = email[i];
		if (c == '.') {
			dotcnt1++;
			lastdot = &email[i];
			/* dot may not be first or last, no consecutive dots */
			if (i == 0 || email[i - 1] == '.' ||
			    email[i - 1] == '@' ||
			    email[i + 1] == '\0' || email[i + 1] == '@')
				return 0;
		} else if (c == '@') {
			atcnt++;
			dotcnt1 = 0;
		} else if ((c >= 'a' && c <= 'z') ||
			   (c >= 'A' && c <= 'Z') ||
			   (c >= '0' && c <= '9') ||
			   strchr(EXTRA_ATEXTCHARS, c))
			chars++;
		else
			return 0;
	}

	/* must have exactly one @, and at least one . after the @ */
	if (atcnt != 1 || dotcnt1 == 0)
		return 0;

	/* no mail to IP addresses, this should be done using [10.2.3.4]
	 * like syntax but we do not allow that either
	 */
	if (isdigit((unsigned char)lastdot[1]))
		return 0;

	/* make sure there are at least 4 characters besides the above
	 * mentioned @ and .                                                          
	 */
	if (chars < 4)
		return 0;

	return valid;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("no\n");
		return 1;
	}
	printf("%d\n", validemail(argv[1]));
}
