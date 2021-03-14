#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread-int.h"

static QINLINE size_t qt_gcd(size_t a, size_t b)
{
#ifdef QTHREAD_SHIFT_GCD
    size_t k = 0;
    if (a == 0) return b;
    if (b == 0) return a;
    while ((a & 1) == 0 && (b&1) == 0) { /* while they're both even */
	a >>= 1; /* divide by 2 */
	b >>= 1; /* divide by 2 */
	k++; /* power of 2 to the result */
    }
    /* now, one or the other is odd */
    do {
	if ((a&1) == 0)
	    a >>= 1;
	else if ((b&1) == 0)
	    b >>= 1;
	else if (a >= b) /* both odd */
	    a = (a-b) >> 1;
	else
	    b = (b-a) >> 1;
    } while (a > 0);
    return b << k;
#else /* MOD GCD */
    while (1) {
	if (a == 0) return b;
	b %= a;
	if (b == 0) return a;
	a %= b;
    }
#endif
}

static QINLINE size_t qt_lcm(size_t a, size_t b)
{				       /*{{{ */
    size_t tmp = qt_gcd(a, b);
    /* on 32 bit platforms, it's pretty easy for a * b to overflow so we force
     * 64 bit multiplication*/
    uint64_t largeA = a;
    uint64_t largeB = b;
    return (tmp != 0) ? (largeA * largeB / tmp) : 0;
}				       /*}}} */
