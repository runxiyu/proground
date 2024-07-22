#include <argon2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/random.h>

#define HASHLEN 32
#define SALTLEN 16
#define PWD "password"

int main(void)
{
	uint8_t hash1[HASHLEN];

	uint8_t salt[SALTLEN];
	getrandom(salt, SALTLEN, 0);

	uint8_t *pwd = (uint8_t *) strdup(PWD);
	uint32_t pwdlen = strlen((char *)pwd);

	uint32_t t_cost = 2;	// 2-pass computation
	uint32_t m_cost = (1 << 16);	// 64 mebibytes memory usage
	uint32_t parallelism = 1;	// number of threads and lanes

	argon2id_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, salt,
			  SALTLEN, hash1, HASHLEN);

	free(pwd);

	for (int i = 0; i < HASHLEN; ++i)
		printf("%02x", hash1[i]);
	puts("");
	return 0;
}
