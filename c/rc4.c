/*
 * Script to log in to YK Pao School Songjiang Campus student WiFi
 * Written by Runxi Yu <me@runxiyu.org>
 *
 * This program is public domain, or under the terms of Creative Commons
 * Zero 1.0 Universal, at your choice. In addition, a Waiver of Patent
 * Rights apply. See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

void rc4(const char *plain, const char *rc4key, char *output)
{
	int sbox[256];
	int key_length = strlen(rc4key);
	int i, j = 0, a = 0, b = 0, c;
	size_t plain_length = strlen(plain);
	for (i = 0; i < 256; i++)
		sbox[i] = i;
	for (i = 0; i < 256; i++) {
		j = (j + sbox[i] + rc4key[i % key_length]) % 256;
		int temp = sbox[i];
		sbox[i] = sbox[j];
		sbox[j] = temp;
	}
	for (i = 0; i < (int)plain_length; i++) {
		a = (a + 1) % 256;
		b = (b + sbox[a]) % 256;
		int temp = sbox[a];
		sbox[a] = sbox[b];
		sbox[b] = temp;
		c = (sbox[a] + sbox[b]) % 256;
		sprintf(output + i * 2, "%02x", plain[i] ^ sbox[c]);
	}
	output[plain_length * 2] = '\0';
}
