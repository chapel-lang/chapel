#ifndef _extern_H_
#define _extern_H_

#ifdef EXTERN
#define EXTERN_INIT(_x) = _x
#else
#define EXTERN_INIT(_x)
#define EXTERN extern
#endif

#endif

