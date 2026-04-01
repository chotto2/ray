/**
 * @file ray.c
 * @brief Ray-based divisor visualization
 * @author N.Arai
 * @date 2026-01-26
 *
 * Visualizes divisor distribution as rays of light with slopes 1/n (n>0).
 * Multiple rays are emitted simultaneously from the origin.
 * Divisors exist only on the ray lines - no divisors in VOID regions.
 *
 * @note v1.1.0 (2026-03-31): Up to 1,000,000
 *       1. Extended the upper limit of integers from 128 to 1,000,000
 *       2. Removed GMP bit operations
 *       3. Virtualized the existence of all divisors at x=0
 *       4. Improved memory efficiency by storing divisor values in an array
 *          - Note: d(720720)=240 is the maximum within the range 0–1,000,000, so the array size is set to 256. (approx. 1GB)
 *          - Note: Stored in ascending order, with NULL(0) as the terminator
 *
 * @note v1.0.0 (2026-01-30): Add ray command
 *       1. Plots the divisors of integers from 0 to 128
 *       2. Data retention by GMP bit operation
 *       3. No divisors exist in the VOID region.
 *
 * @author chotto2
 * @project Prime Oasis
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define D_MAX (256)
#define X_MAX (1000000)	
#define Y_MAX (X_MAX)
#define A_MAX (X_MAX)
#define DSP_MAX (128)

typedef struct {
	uint32_t div[D_MAX];
	uint32_t cnt;
} DIVS, *pDIVS;
DIVS divs[X_MAX+1] = {0};

int benchmark_mode = 0;

/**
 * @brief Main entry point
 */
int32_t main(int argc, char *argv[])
{
	uint32_t n;
	uint32_t x;
	uint32_t y;
	int32_t ret = 0;
	uint32_t ofs;
	uint32_t count;
	uint32_t pre;

	/*--- check argv ---*/
	for (int i = 1; i < argc; i++) {
    	if (strcmp(argv[i], "--benchmark") == 0) {
        	benchmark_mode = 1;
 	   }
	}

	/*--- other ---*/
	for (n = 1; n <= A_MAX; n++) {			// y = x/n
		for (x = n, y = 1; x <= X_MAX; x += n) {
			divs[x].div[divs[x].cnt++] = y++;
		}
	}

	/*--- for printing ---*/
	if (!benchmark_mode) {
		printf("      n:   d(n):divisors2(n, %d)\n", DSP_MAX);
		printf("%7d:%7d:", 0, Y_MAX);
		for (y = 1; y <= DSP_MAX; y++) {
			printf("*");
		}
		printf("...\n");

		for (x = 1; x <= X_MAX; x++) {
			printf("%7d:%7d:", x, divs[x].cnt);
			pre = 0;
			for (count = divs[x].cnt; count > 0; count--) {
				ofs = count - 1;
				if (ofs >= D_MAX) continue;
				y = divs[x].div[ofs];
				if (y > DSP_MAX) continue;
				if (pre) {
					for (int i = 0; i < (y-pre-1); i++) {
						printf(" ");
					}
				}
				printf("*");
				pre = y;
			}
			printf("\n");
		}
	}

	return ret;
}


