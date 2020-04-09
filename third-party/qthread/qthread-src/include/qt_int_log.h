#ifndef QT_INT_LOG_H
#define QT_INT_LOG_H

/* This public-domain algorithm from
 * http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog */

static const char LogTable256[256] = {
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};

static inline uint32_t QT_INT_LOG(uint32_t v)
{
    uint32_t          r;
    uint32_t t;
    uint32_t tt;

    if ((tt = (v >> 16))) {
        r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
    } else {
        r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
    }
    return r;
}

#endif // ifndef QT_INT_LOG_H
/* vim:set expandtab: */
