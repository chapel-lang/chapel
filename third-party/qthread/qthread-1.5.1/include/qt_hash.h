#ifndef QT_HASH_H
#define QT_HASH_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_CPROPS
# include <cprops/hashlist.h>
#endif

#include <stddef.h> /* for size_t (according to C89) */
#include <stdint.h> /* for uintptr_t (according to C99) */

#ifdef __cplusplus
extern "C" {
#endif

typedef const void *qt_key_t;
#ifndef HAVE_CPROPS
typedef struct qt_hash_s *qt_hash;
typedef void (*qt_hash_callback_fn)(const qt_key_t, void *, void *);
#else
typedef cp_hashlist *qt_hash;
typedef int (*qt_hash_callback_fn)(void *, void *, void *);
#endif
typedef void (*qt_hash_deallocator_fn)(void*);

qt_hash qt_hash_create(int needSync);
void qt_hash_destroy(qt_hash h);
void qt_hash_destroy_deallocate(qt_hash h, qt_hash_deallocator_fn f);
void *qt_hash_put(qt_hash h, const qt_key_t key, void *value);
void *qt_hash_put_locked(qt_hash h, const qt_key_t key, void *value);
void qt_hash_remove(qt_hash h, const qt_key_t key);
void qt_hash_remove_locked(qt_hash h, const qt_key_t key);
void *qt_hash_get(qt_hash h, const qt_key_t key);
void *qt_hash_get_locked(qt_hash h, const qt_key_t key);
size_t qt_hash_count(qt_hash h);
void qt_hash_callback(qt_hash h, qt_hash_callback_fn f, void *arg);

void qt_hash_lock(qt_hash h);
void qt_hash_unlock(qt_hash h);

#ifdef __cplusplus
}
#endif

#endif
