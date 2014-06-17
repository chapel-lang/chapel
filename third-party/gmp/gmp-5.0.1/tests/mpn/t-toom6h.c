#define mpn_toomMN_mul mpn_toom6h_mul
#define mpn_toomMN_mul_itch mpn_toom6h_mul_itch

/* Smaller sizes not supported; may lead to recursive calls to
   toom22_mul, toom33_mul, or toom44_mul with invalid input size. */
#define MIN_AN MUL_TOOM6H_THRESHOLD
#define MIN_BN(an) (MAX ((an*3)>>3, 42) )

#include "toom-shared.h"
