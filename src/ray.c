/**
 * @file ray.c
 * @brief Ray-based divisor visualization
 *
 * Visualizes divisor distribution as rays of light with slopes 1/n (n>0).
 * Multiple rays are emitted simultaneously from the origin.
 * Divisors exist only on the ray lines - no divisors in VOID regions.
 *
 * Uses bitmap structure for efficient memory usage, scalable to large integers.
 *
 * @author chotto2
 * @project Prime Oasis
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>

#define DSP_MAX (128)
#define N_MAX (128)	
#define M_MAX (N_MAX)

typedef struct {
	mpz_t div;
	int cnt;
} DIVS, *pDIVS;
DIVS divs[N_MAX+1];

/**
 * @brief Main entry point
 */
int32_t main()
{
	uint64_t m;
	uint64_t n;
	uint64_t d;
	int32_t ret = 0;
	uint64_t ofs;

	/*--- init ---*/
	for (n = 0; n <= N_MAX; n++)
		mpz_init(divs[n].div);

	/*--- base ---*/
	for (m = 1; m <= M_MAX; m++) {
		divs[0].cnt++;
		ofs = m - 1;
		mpz_setbit(divs[0].div, ofs);
	}

	/*--- other ---*/
	for (m = 1; m <= M_MAX; m++) {
		for (n = m; n <= N_MAX; n += m) {
			divs[n].cnt++;
			if (m <= DSP_MAX) {
				ofs = m - 1;
			    	mpz_setbit(divs[n].div, ofs);
			}
		}
	}

	/*--- for printing ---*/
	printf("      n:   d(n):divisors2(n, %d)\n", M_MAX);
	for (n = 0; n <= N_MAX; n++) {
		printf("%7lu:%7d:", n, divs[n].cnt);
		for (d = 1; d <= DSP_MAX; d++) {
			ofs = d - 1;
			printf("%s", (mpz_tstbit(divs[n].div, ofs) != 0)? "*": " ");
		}
		printf("%s", (divs[n].cnt >= DSP_MAX)? "...": "");
		printf("\n");
	}

	/*--- clear ---*/
	for (n = 0; n <= N_MAX; n++)
		mpz_clear(divs[n].div);


	return ret;
}


