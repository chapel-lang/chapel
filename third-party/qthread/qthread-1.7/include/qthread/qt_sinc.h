#ifndef QT_SINC_H
#define QT_SINC_H

typedef struct qt_sinc_s {
    aligned_t *values;
    aligned_t *counts;
    size_t     factor;
    syncvar_t  result;
} qt_sinc_t;

qt_sinc_t *qt_sinc_create(void);
void       qt_sinc_destroy(qt_sinc_t *sinc);
void       qt_sinc_willspawn(qt_sinc_t *sinc, size_t count);
void       qt_sinc_submit(qt_sinc_t *sinc, aligned_t *value);
void       qt_sinc_wait(qt_sinc_t *sinc, aligned_t *target);

#endif
