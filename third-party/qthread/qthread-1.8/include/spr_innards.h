#ifndef SPR_INNARDS_H
#define SPR_INNARDS_H

/* Action ID Category Offsets */
enum {
    spr_non_action     = 0,
    spr_internals_base = 1,  /* Internal SPR actions */
    spr_init_base      = 64, /* User actions (via spr_init() call) */
};

typedef struct spr_get_s {
    qt_sinc_t *sinc;
    void      *dest_addr;
    int32_t    size;
} spr_get_t;

struct spr_get_handle_s {
    qt_sinc_t  sinc;
    spr_get_t *get_objs;
};

#endif /* SPR_INNARDS_H */
/* vim:set expandtab: */
