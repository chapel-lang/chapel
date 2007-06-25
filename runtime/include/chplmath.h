#ifndef _CHPL_MATH_H_
#define _CHPL_MATH_H_

// BLC: This prototype should not be necessary, but seems to be
// required to avoid a C warning on our linux, linux64, and sunos
// boxes
#ifndef log2
double log2(double);
#endif
#ifndef exp2
double exp2(double);
#endif

#endif
