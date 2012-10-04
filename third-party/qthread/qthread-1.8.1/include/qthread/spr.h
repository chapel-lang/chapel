#ifndef SPR_H
#define SPR_H

#include <qthread/qthread.h>

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

int spr_spawn(qthread_f   f,
              void       *arg,
              aligned_t  *ret,
              int         tgt,
              size_t      arg_len);

/******************************************************************************
 * Data Movement                                                              *
 ******************************************************************************/

typedef struct spr_get_handle_s spr_get_handle_t;

int spr_get(void   *dest_addr,
            int     src_loc,
            void   *src_addr,
            size_t  size);
int spr_get_nb(void             *dest_addr,
               int               src_loc,
               void             *src_addr,
               size_t            size,
               spr_get_handle_t *hand);
int spr_get_wait(spr_get_handle_t * const hand);

typedef struct spr_put_handle_s {
    qt_sinc_t sinc;
} spr_put_handle_t;

int spr_put(int    dest_loc,
            void  *dest_addr,
            void  *src_addr,
            size_t size);
int spr_put_nb(int               dest_loc,
               void             *dest_addr,
               void             *src_addr,
               size_t            size,
               spr_put_handle_t *hand);
int spr_put_wait(spr_put_handle_t * const hand);

Q_ENDCXX /* */

#endif // ifndef SPR_H
/* vim:set expandtab: */
