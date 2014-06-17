typedef struct _mys {
    int x;
} _mys;

typedef _mys* mys;

static inline
void return_mys(mys* x)
{
    (*x)->x = 7;
    *x = NULL;
}

