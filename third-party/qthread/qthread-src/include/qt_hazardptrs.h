#ifndef QT_HAZARDPTRS_H
#define QT_HAZARDPTRS_H

#include "qt_visibility.h"

#define HAZARD_PTRS_PER_SHEP 2

typedef struct {
    void (*freefunc)(void *);
    void *ptr;
} hazard_freelist_entry_t;

typedef struct {
    hazard_freelist_entry_t *freelist;
    unsigned int             count;
} hazard_freelist_t;

void INTERNAL initialize_hazardptrs(void);
void INTERNAL hazardous_ptr(unsigned int which,
                            void        *ptr);

typedef void (*hazardous_free_f)(void *arg);
void INTERNAL hazardous_release_node(hazardous_free_f freefunc,
                                     void            *ptr);

#endif // ifndef QT_HAZARDPTRS_H
/* vim:set expandtab: */
