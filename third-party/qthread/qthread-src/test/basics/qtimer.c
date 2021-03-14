#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include <sys/time.h>
#include <math.h> /* for sqrt() */
#include "argparsing.h"

const unsigned int ITER = 100;

static int dcmp(const void *a,
                const void *b)
{
    if (*(double *)a > *(double *)b) {
        return 1;
    } else if (*(double *)a < *(double *)b) {
        return -1;
    } else { return 0; }
}

static double bang(int i)
{
    double product = 1.0;

    for (; i > 1; i--) product *= i;
    return product;
}

static double chisquare_alphafive(int v /* degfreedom */)
{   /*{{{*/
    switch (v) {
        case 1: return 3.84;

        case 2: return 5.99;

        case 3: return 7.81;

        case 4: return 9.49;

        case 5: return 11.1;

        case 6: return 12.6;

        case 7: return 14.1;

        case 8: return 15.5;

        case 9: return 16.9;

        case 10: return 18.3;

        case 11: return 19.7;

        case 12: return 21.0;

        case 13: return 22.4;

        case 14: return 23.7;

        case 15: return 25.0;

        case 16: return 26.3;

        case 17: return 27.6;

        case 18: return 28.9;

        case 19: return 30.1;

        case 20: return 31.4;

        case 21: return 32.7;

        case 22: return 33.9;

        case 23: return 35.2;

        case 24: return 36.4;

        case 25: return 37.7;

        case 26: return 38.9;

        case 27: return 40.1;

        case 28: return 41.3;

        case 29: return 42.6;

        case 30: return 43.8;

        default:
            if ( v <= 40 ) { return (55.8 - 43.8) / 10.0 * (v - 30); } else if ( v <= 50 ) {
                return (67.5 - 55.8) / 10.0 * (v - 40);
            } else if ( v <= 60 ) {
                return (79.1 - 67.5) / 10.0 * (v - 50);
            } else if ( v <= 70 ) {
                return (90.5 - 79.1) / 10.0 * (v - 60);
            } else if ( v <= 80 ) {
                return (101.9 - 90.5) / 10.0 * (v - 70);
            } else if ( v <= 90 ) {
                return (113.1 - 101.9) / 10.0 * (v - 80);
            } else { return (124.3 - 113.1) / 10.0 * (v - 90); }
    }
} /*}}}*/

static void runs (void)
{   /*{{{*/
    unsigned int     a       = 0;
    int              n1      = 0, n2 = 0, meanruns = 0;
    int              lastdir = 0;
    double           mua, sigmaa2, Zzeroa, Zzerob, mub, sigmab2;
    double           sum    = 0.0, mean, chisquare;
    unsigned int     *runlengths, maxrun = 0;
    int             rl;
    double         **oi;
    size_t           i;
    double *restrict A = malloc(sizeof(double) * ITER);

    iprintf("\nRuns Test:\n");
    for (i = 0; i < ITER; ++i) {
        A[i] = qtimer_fastrand();
        sum += A[i];
        if (i > 0) {
            if (A[i] > A[i - 1]) {
                if (lastdir == -1) {
                    ++a;
                }
                lastdir = 1;
            } else {
                if (lastdir == 1) {
                    ++a;
                }
                lastdir = -1;
            }
        }
    }
    mean = sum / (double)ITER;
    for (i = 0; i < ITER; ++i) {
        if (A[i] > mean) {
            n1++;
        } else {
            n2++;
        }
    }
    lastdir = (A[0] > mean) ? 1 : -1;
    for (i = 1; i < ITER; ++i) {
        if (A[i] > mean) {
            if (lastdir == -1) {
                meanruns++;
            }
            lastdir = 1;
        } else {
            if (lastdir == 1) {
                meanruns++;
            }
            lastdir = -1;
        }
    }
    mua     = ((2.0 * ITER) - 1.0) / 3.0;
    sigmaa2 = ((16.0 * ITER) - 29.0) / 90.0;
    Zzeroa  = (a - mua) / sigmaa2;
    mub     = ((2.0 * n1 * n2) / (double)ITER) + 0.5;
    sigmab2 = (2.0 * n1 * n2 * ((2.0 * n1 * n2) - ITER)) / (double)(ITER * ITER * (ITER - 1));
    Zzerob  = (meanruns - mub) / sqrt(sigmab2);

    runlengths     = (unsigned int *)calloc(sizeof(unsigned int), ITER);
    rl             = 0;
    runlengths[rl] = 1;
    lastdir        = 1;
    maxrun         = 0;
    for (i = 1; i < ITER; ++i) {
        if (A[i] > A[i - 1]) {
            if (lastdir == -1) {
                if (maxrun < runlengths[rl]) {
                    maxrun = runlengths[rl];
                }
                rl++;
            }
            lastdir = 1;
            runlengths[rl]++;
        } else {
            if (lastdir == 1) {
                if (maxrun < runlengths[rl]) {
                    maxrun = runlengths[rl];
                }
                ++rl;
            }
            lastdir = -1;
            runlengths[rl]++;
        }
    }
    oi = malloc(sizeof(double *) * 3);
    assert(oi);
    oi[0] = calloc(sizeof(double), maxrun);
    oi[1] = calloc(sizeof(double), maxrun);
    oi[2] = calloc(sizeof(double), maxrun);
    /* count the observed runs */
    for (i = 0; i < a; ++i) {
        oi[0][runlengths[i] - 1]++;
    }
    /* expected run calculation */
    for (i = 0; i < maxrun; ++i) {
        if (i <= ITER - 2) {
            oi[1][i] = (2.0 / bang(i + 3.0)) * (ITER * (i * i + 3 * i + 1) - (i * i * i + 3 * i * i - i - 4));
        } else {
            oi[1][i] = 2.0 / bang(ITER);
        }
    }
    /* insert expectation value combining here, start from high-end */
    if (maxrun > 2) {
        for (i = maxrun - 1; i > 0; --i) {
            if (oi[1][i] < 5) {
                size_t j;
                oi[1][i - 1] += oi[1][i];
                // oi[2][i-1] += oi[2][i];
                for (j = i; j + 1 < maxrun; ++j) {
                    oi[1][j] = oi[1][j + 1];
                    // oi[2][j] = oi[1][j+1];
                }
                maxrun--;
            }
        }
    }
    /* calc chisquare parts */
    for (i = 0; i < maxrun; ++i) {
        oi[2][i] = ((oi[0][i] - oi[1][i]) * (oi[0][i] - oi[1][i])) / oi[1][i];
    }
    /* count up chisquare sum */
    chisquare = 0.0;
    for (i = 0; i < maxrun; ++i) {
        chisquare += oi[2][i];
    }

    iprintf("       runs with alpha 0.05: %s\n", (Zzeroa >= -1.96 && Zzeroa <= 1.96) ? "Passed" : "Failed");
    iprintf("  mean runs with alpha 0.05: %s\n", (Zzerob >= -1.96 && Zzerob <= 1.96) ? "Passed" : "Failed");
    iprintf("run lengths with alpha 0.05: %s\n", (chisquare < chisquare_alphafive(maxrun - 1)) ? "Passed" : "Failed");
    free(oi[2]);
    free(oi[1]);
    free(oi[0]);
    free(oi);
    free(runlengths);
    free(A);
} /*}}}*/

static void ks_test(void)
{   /*{{{*/
    double  Dplus, Dminus, D, Dalpha1, Dalpha2, Dalpha3;
    double *A[2];

    iprintf("Kolmogorov-Smirnov test:\n");
    A[0] = malloc(sizeof(double) * ITER);
    A[1] = malloc(sizeof(double) * ITER);
    for (size_t i = 0; i < ITER; ++i) {
        A[0][i] = qtimer_fastrand();
        A[1][i] = (double)i / (double)ITER;
    }
    qsort(A[0], ITER, sizeof(double), dcmp);
    Dplus  = A[1][0] - A[0][0];
    Dminus = A[0][0] - (-1.0 / (double)ITER);
    for (size_t i = 1; i < ITER; ++i) {
        if (A[1][i] - A[0][i] > Dplus) {
            Dplus = A[1][i] - A[0][i];
        }
        if (A[0][i] - ((i - 1) / (double)ITER) > Dminus) {
            Dminus = A[0][i] - ((i - 1) / (double)ITER);
        }
    }
    if (Dplus > Dminus) {
        D = Dplus;
    } else {
        D = Dminus;
    }
    Dalpha1 = 1.22 / sqrt(ITER);
    Dalpha2 = 1.36 / sqrt(ITER);
    Dalpha3 = 1.63 / sqrt(ITER);
    iprintf("alpha of 0.10: %s\n", (D <= Dalpha1) ? "Passed" : "Failed");
    iprintf("alpha of 0.05: %s\n", (D <= Dalpha2) ? "Passed" : "Failed");
    iprintf("alpha of 0.01: %s\n", (D <= Dalpha3) ? "Passed" : "Failed");
    free(A[0]);
    free(A[1]);
} /*}}}*/

static void autocorrelation(void)
{   /*{{{*/
    int    eye = 3, m = 5, M;
    int    k;
    double pim, sigmapim, Zzero;
    double A[ITER];

    iprintf("\nAutocorrelation:\n");
    for (size_t i = 0; i < ITER; ++i) {
        A[i] = qtimer_fastrand();
    }

    for (eye = 1; eye < 51; ++eye) {
        for (m = 1; m < 11; ++m) {
            M   = floor((ITER - eye) / (double)m) - 1;
            pim = 0.0;
            for (k = 0; k <= M; ++k) {
                pim += A[eye + k * m] * A[eye + (k + 1) * m];
            }
            pim     *= (1.0 / (M + 1.0));
            pim     -= 0.25;
            sigmapim = sqrt(13 * M + 7) / (12 * (M + 1));

            Zzero = pim / sigmapim;
            iprintf("i=%i, m=%i, M=%i: %s\n", eye, m, M, (Zzero >= -1.96 && Zzero <= 1.96) ? "Passed" : "Failed");
        }
    }
} /*}}}*/

int main(int   argc,
         char *argv[])
{
    qtimer_t t;

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();

    t = qtimer_create();
    assert(t);
    qtimer_start(t);
    qtimer_stop(t);
    if (qtimer_secs(t) == 0) {
        fprintf(stderr, "qtimer_secs(t) reported zero length time.\n");
    } else if (qtimer_secs(t) < 0) {
        fprintf(stderr, "qtimer_secs(t) thinks time went backwards (%g).\n",
                qtimer_secs(t));
    }
    iprintf("time to find self and assert it: %g secs\n", qtimer_secs(t));

    qtimer_start(t);
    qtimer_stop(t);
    assert(qtimer_secs(t) >= 0.0);
    if (qtimer_secs(t) == 0.0) {
        iprintf("inlining reduces calltime to zero (apparently)\n");
    } else {
        iprintf("smallest measurable time: %g secs\n", qtimer_secs(t));
    }

    qtimer_destroy(t);

    // Now to test fastrand
    ks_test();
    runs();
    autocorrelation();

    qthread_finalize();

    return 0;
}

/* vim:set expandtab */
