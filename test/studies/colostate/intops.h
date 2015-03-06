#define min2(a,b)	((a)>(b)?(b):(a))
#define min3(a,b,c)	(min2(min2(a,b), c))
#define min4(a,b,c,d)	(min2(min2(a,b), min2(c,d)))
#define min5(a,b,c,d,e)	(min2(min3(a,b,c), min2(d,e)))
#define min6(a,b,c,d,e,f)	(min2(min3(a,b,c), min3(d,e,f)))

#define max2(a,b)	((a)>(b)?(a):(b))
#define max3(a,b,c)	(max2(max2(a,b), c))
#define max4(a,b,c,d)	(max2(max2(a,b), max2(c,d)))
#define max5(a,b,c,d,e)	(max2(max3(a,b,c), max2(d,e)))
#define max6(a,b,c,d,e,f)	(max2(max3(a,b,c), max3(d,e,f)))

#define min(a,b)	min2(a,b)
#define max(a,b)	max2(a,b)


#if ! defined ASSUME_POSITIVE_INTMOD
#define ASSUME_POSITIVE_INTMOD 0
#endif

#if ASSUME_POSITIVE_INTMOD
#define intDiv(x,y)	(eassert(((x)%(y)) >= 0), ((x)/(y)))
#define intMod(x,y)	(eassert(((x)%(y)) >= 0), ((x)%(y)))
#else
#define intDiv_(x,y)	((((x)%(y))>=0) ? ((x)/(y)) : (((x)/(y)) -1))
#define intMod_(x,y)	((((x)%(y))>=0) ? ((x)%(y)) : (((x)%(y)) +y))
#define checkIntDiv(x,y) (eassert((y) > 0 && intMod_((x),(y)) >= 0 && intMod_((x),(y)) <= (y) && x==((y)*intDiv_((x),(y)) + intMod_((x),(y)))))
#define intDiv(x,y)	(checkIntDiv((x),(y)), intDiv_((x),(y)))
#define intMod(x,y)	(checkIntDiv((x),(y)), intMod_((x),(y)))
#endif

#define ceild(n, d)	intDiv((n), (d)) + ((intMod((n),(d))>0)?1:0)
#define floord(n, d)	intDiv((n), (d))

#if 0

// #define intMod(n,d)  (eassert((n)>=0 && (d)>0), ((n)%(d)))
// #define intDiv(n,d)  (eassert((n)>=0 && (d)>0), ((n)/(d)))

static inline int intDiv(int n, int d)
{
  int r = n/d - ((n%d<0)?1:0);
  assert(d*r <= n && n < d*(r+1));
  assert(d*r + n%d + ((n%d<0)?d:0) == n);
  return r;
}

static inline int intMod(int n, int d)
{
  int m = n%d + ((n%d<0)?d:0);
  assert(0 <= m && m < d);
  assert(d*(n/d - ((n%d<0)?1:0)) + m == n);
  return m;
}
#endif

