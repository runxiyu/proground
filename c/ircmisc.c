#include <unistd.h>
#include <stdio.h>
#include "haxstring.h"
#include "haxstring_utils.h"

#define CASEMAP(x) (casemap[(unsigned char)x])
const unsigned char casemap[] = {
	0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa,
	0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14,
	0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
	0x1e, 0x1f,
	' ', '!', '"', '#', '$', '%', '&', 0x27, '(', ')',
	'*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	':', ';', '<', '=', '>', '?',
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
	'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
	0x5f,
	'`', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
	'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
	0x7f,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
	0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9,
	0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9,
	0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
	0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9,
	0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
	0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
	0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};

int ircmisc_mask_match(const struct string *mask, const struct string *name)
{
	// Copyright (c) 1990 Jarkko Oikarinen
	// SPDX-License-Identifier: GPL-1.0-or-later
	const char *m_origin = mask->data, *n_origin = name->data;
	const char *m = mask->data, *n = name->data;
	const char *m_tmp = mask->data, *n_tmp = name->data;
	int star_p;

	for (;;) {
		if ((size_t)(m - m_origin + 1) > mask->len) {
			if ((size_t)(n - n_origin + 1) > name->len)
				return 1;
			goto backtrack;
		} else {
			goto after_backtrack;
		}
 backtrack:
		if (m_tmp == m_origin)
			return 0;
		m = m_tmp;
		n = ++n_tmp;
		break;
 after_backtrack:
		switch (*m) {
		case '*':
		case '?':
			for (star_p = 0;; m++) {
				if (*m == '*')
					star_p = 1;
				else if (*m == '?') {
					if ((size_t)(n - n_origin + 1) >
					    name->len) {
						++n;
						goto backtrack;
					} else {
						++n;
					}
				} else
					break;
			}
			if (star_p) {
				if ((size_t)(m - m_origin + 1) > mask->len)
					return 1;
				else {
					m_tmp = m;
					for (n_tmp = n;
					     *n && CASEMAP(*n) != CASEMAP(*m);
					     n++) ;
				}
			}
			/* FALLTHROUGH */
		default:
			if ((size_t)(n - n_origin + 1) > name->len)
				return (*m != '\0' ? 0 : 1);
			if (CASEMAP(*m) != CASEMAP(*n))
				goto backtrack;
			m++;
			n++;
			break;
		}
	}
	return 1;
}

void test(const struct string *mask, const struct string *target, int expected)
{
	WRITES(2, *mask);
	WRITES(2, STRING("\t"));
	WRITES(2, *target);
	WRITES(2, STRING("\t"));
	if (expected == ircmisc_mask_match(mask, target)) {
		WRITES(2, STRING("OK"));
	} else {
		WRITES(2, STRING("FAIL"));
	}
	WRITES(2, STRING("\n"));
}

int main(void)
{
	struct string *s = &STRING("*");
	test(s, &STRING("abc"), 1);
	test(&STRING("abc"), &STRING("abc"), 1);
	test(&STRING("b*cd"), &STRING("abc"), 0);
	test(&STRING("?991"), &STRING("x991"), 1);
	test(&STRING("{"), &STRING("["), 1);
	test(&STRING("*.runxiyu.org"), &STRING("JELLYFISH.RUNXIYU.ORG"), 1);
	return 0;
}
