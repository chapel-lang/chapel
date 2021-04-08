#ifndef SPR_H
#define SPR_H

#include "qthread.h"

Q_STARTCXX /* */

/* Flags */
#define SPR_SPMD (1 << 0) /* Launch in SPMD mode (much like MPI_Init()) */

/* Return Values */
#define SPR_OK      0  /* Success! */
#define SPR_BADARGS -1 /* One or more arguments was invalid. */
#define SPR_IGN     -2 /* The call was ignored. */
#define SPR_NOINIT  -3 /* SPR environment was not initialized. */

int spr_init(unsigned int flags,
             qthread_f   *regs);
int spr_fini(void);
int spr_register_actions(qthread_f *actions,
                         size_t     count,
                         size_t     base);
int spr_unify(void);

int spr_num_locales(void);
int spr_locale_id(void);

/******************************************************************************
* Data Movement                                                              *
******************************************************************************/

typedef struct {
    uint8_t opaque_data[8];
} Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) spr_get_handle_t;

int spr_get(void *restrict       dest_addr,
            int                  src_loc,
            const void *restrict src_addr,
            size_t               size);
int spr_get_nb(void *restrict             dest_addr,
               int                        src_loc,
               const void *restrict       src_addr,
               size_t                     size,
               spr_get_handle_t *restrict hand);
int spr_get_wait(spr_get_handle_t *const hand);

typedef struct {
    uint8_t opaque_data[8];
} Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) spr_put_handle_t;

int spr_put(int                  dest_loc,
            void *restrict       dest_addr,
            const void *restrict src_addr,
            size_t               size);
int spr_put_nb(int                        dest_loc,
               void *restrict             dest_addr,
               const void *restrict       src_addr,
               size_t                     size,
               spr_put_handle_t *restrict hand);
int spr_put_wait(spr_put_handle_t *const hand);

int spr_locale_barrier(void);

Q_ENDCXX /* */

#endif // ifndef SPR_H
/* vim:set expandtab: */
