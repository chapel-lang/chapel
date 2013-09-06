#ifndef QT_INT_CEIL_H
#define QT_INT_CEIL_H

#define QT_CEIL_RATIO(x, y) (1 + (((x) - 1) / (y)))
#define QT_CEIL_POW2(x, y) (((x) + ((1<<(y))-1)) >> (y))
#define QT_CEIL_DIV8(x) (((x) + 7) >> 3)

#endif
/* vim:set expandtab: */
