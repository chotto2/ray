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
 * @note v1.3.0 (2026-04-07): Up to 3,000,000
 *       1. Extended the upper limit of integers from 2,000,000 to 3,000,000
 *       2. Expanded the memory that holds divisors
 *          - Note: d(2882880)=336 is the maximum within the range 0–3,000,000, so the array size is set to 344. (approx. 4.2GB)
 *
 * @note v1.2.0 (2026-04-04): Up to 2,000,000
 *       1. Extended the upper limit of integers from 1,000,000 to 2,000,000
 *       2. Expanded the memory that holds divisors
 *          - Note: d(1441440)=288 is the maximum within the range 0–2,000,000, so the array size is set to 296. (approx. 2.4GB)
 *          - Note: Change to dynamic memory(calloc)
 *
 * @note v1.1.0 (2026-03-31): Up to 1,000,000
 *       1. Extended the upper limit of integers from 128 to 1,000,000
 *       2. Removed GMP bit operations
 *       3. Virtualized the existence of all divisors at x=0
 *       4. Improved memory efficiency by storing divisor values in an array
 *          - Note: d(720720)=240 is the maximum within the range 0–1,000,000, so the array size is set to 256. (approx. 1GB)
 *          - Note: Stored in descending order, with NULL(0) as the terminator
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
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#define X_MAX (4000000)	
#define Y_MAX (X_MAX)
#define A_MAX (X_MAX)
#define DSP_MAX (128)

uint32_t *divs_pool;

typedef struct {
	uint32_t pool_ofs;
	uint32_t pool_cnt;
	uint32_t cnt;
} DIVS, *pDIVS;
pDIVS divs;

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
	struct rusage r_start, r_end;
	struct timeval wall_start, wall_end;

	/*--- check argv ---*/
	for (int i = 1; i < argc; i++) {
    		if (strcmp(argv[i], "--benchmark") == 0) {
        		benchmark_mode = 1;
 		}
	}

	/*--- alloc divs ---*/
	divs = calloc(X_MAX+1, sizeof(DIVS));
	if (divs == NULL) {
		printf("ERR: divs(0) = calloc(%d, %ld)\n", X_MAX+1, sizeof(DIVS));
		return -1;
	}

	/*--- make pool_cnt ---*/
	for (n = 1; n <= A_MAX; n++) {			
		for (x = n, y = 1; x <= X_MAX; x += n) {
			divs[x].pool_cnt++;
		}
	}

	/*--- make pool_ofs ---*/
	ofs = 0;
	for (x = 1; x <= X_MAX; x++) {
		divs[x].pool_ofs = ofs;
		ofs += divs[x].pool_cnt;
		ofs++;  // for NULL padding.
	}

	/*--- alloc pool ---*/
	divs_pool = calloc(ofs, sizeof(uint32_t));
	if (divs_pool == NULL) {
		printf("ERR: divs_pool(0) = calloc(%d, %ld)\n", ofs, sizeof(uint32_t));
		return -2;
	}

	/*--- get start time ---*/
	if (benchmark_mode) {
		gettimeofday(&wall_start, NULL);
		getrusage(RUSAGE_SELF, &r_start);
	}

	/*--- ray process ---*/
	for (n = 1; n <= A_MAX; n++) {			// y = x/n
		for (x = n, y = 1; x <= X_MAX; x += n) {
			if (divs[x].cnt < divs[x].pool_cnt) {
				divs_pool[divs[x].pool_ofs+divs[x].cnt] = y++;
			}
			divs[x].cnt++;
		}
	}

	/*--- get end time ---*/
	if (benchmark_mode) {
		gettimeofday(&wall_end, NULL);
		getrusage(RUSAGE_SELF, &r_end);
	}

	if (benchmark_mode) {
		/*--- print benchmark ---*/
		double wall = (wall_end.tv_sec - wall_start.tv_sec)
			    + (wall_end.tv_usec - wall_start.tv_usec) / 1e6;
		double user = (r_end.ru_utime.tv_sec  - r_start.ru_utime.tv_sec)
			    + (r_end.ru_utime.tv_usec - r_start.ru_utime.tv_usec) / 1e6;
		double sys  = (r_end.ru_stime.tv_sec  - r_start.ru_stime.tv_sec)
			    + (r_end.ru_stime.tv_usec - r_start.ru_stime.tv_usec) / 1e6;

		printf("real %.3fs user %.3fs  sys %.3fs\n", wall, user, sys);
	}
	else {
		/*--- print divisor stars ---*/
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
				if (ofs >= divs[x].pool_cnt) continue;
				y = divs_pool[divs[x].pool_ofs+ofs];
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

	free(divs_pool);
	free(divs);

	return ret;
}


