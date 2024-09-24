#ifndef QT_RNG_BRG_H
#define QT_RNG_BRG_H

#include "brg_sha1.h"

#define POS_MASK    0x7fffffff
#define HIGH_BITS   0x80000000

/**********************************/
/* random number generator state  */
/**********************************/
struct state_t {
  uint8_t state[20];
};

typedef uint8_t RNG_state;

/***************************************/
/* random number generator operations  */
/***************************************/
void   rng_init(RNG_state *state, int seed);
void   rng_spawn(RNG_state *mystate, RNG_state *newstate, int spawnNumber);
int    rng_rand(RNG_state *mystate);
int    rng_nextrand(RNG_state *mystate);
char * rng_showstate(RNG_state *state, char *s);
int    rng_showtype(char *strBuf, int ind);

#endif /* QT_RNG_BRG_H */
