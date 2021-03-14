#if defined NDEBUG
#define eassert(X)	0
#else
static inline int eassert_function(int fact, int line, char *fact_text)
{
  if (!fact)
    {
      fprintf(stderr, "assertion failed, line %d: %s\n", line, fact_text);
      exit(1);
    }
  else
    return 0;
}
#define eassert(X)	eassert_function(X, __LINE__, "X")
#endif
