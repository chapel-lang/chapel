#ifndef QT_HASH_H
#define QT_HASH_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stddef.h>      /* for size_t (according to C89) */
#include "qthread-int.h" /* for uintptr_t (standin for C99's stdint.h) */

#include "qt_visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef const void *qt_key_t;
typedef struct qt_hash_s *qt_hash;
typedef void (*qt_hash_callback_fn)(const qt_key_t, void *, void *);
typedef void (*qt_hash_deallocator_fn)(void *);

void qt_hash_initialize_subsystem(void);

/*!
 * @fn qt_hash_create(int needSync)
 * @brief Allocate and initialize a hash map
 */
qt_hash INTERNAL qt_hash_create(int needSync);

/*!
 * @fn qt_hash_destroy(qt_hash h)
 * @brief Deallocates a hash map and any mappings that remain in it.
 */
void INTERNAL qt_hash_destroy(qt_hash h);

/*!
 * @fn qt_hash_destroy_deallocate(qt_hash                h,
 *                                qt_hash_deallocator_fn f)
 * @brief Deallocates a hash map and any mappings that remain in it. For each
 *	mapping, it applies the deallocator function to deallocate any attached
 *	memory.
 */
void INTERNAL qt_hash_destroy_deallocate(qt_hash                h,
                                         qt_hash_deallocator_fn f);
/*!
 * @fn qt_hash_put(qt_hash        h,
 *                 const qt_key_t key,
 *                 void          *value)
 * @brief Insert a mapping from <key> to <value> into the hash map. This
 *      function returns 1 if the insertion succeeded, or 0 is the insertion
 *      failed.
 */
int INTERNAL qt_hash_put(qt_hash  h,
                         qt_key_t key,
                         void    *value);

/*!
 * @fn qt_hash_put_locked(qt_hash        h,
 *                        const qt_key_t key,
 *                        void          *value)
 * @brief Same as qt_hash_put(), but assumes that the hash map has been
 *	locked already. */
int INTERNAL qt_hash_put_locked(qt_hash  h,
                                qt_key_t key,
                                void    *value);

/*!
 * @fn qt_hash_remove(qt_hash        h,
 *                    const qt_key_t key)
 * @brief Remove the mapping for <key> from the hash map.
 */
int INTERNAL qt_hash_remove(qt_hash        h,
                            const qt_key_t key);

/*!
 * @fn qt_hash_remove_locked(qt_hash        h,
 *                           const qt_key_t key)
 * @brief Same as qt_hash_remove(), but assumes that the hash map has been
 *	locked already.
 */
int INTERNAL qt_hash_remove_locked(qt_hash        h,
                                   const qt_key_t key);

/*!
 * @fn qt_hash_get(qt_hash        h,
 *                 const qt_key_t key)
 * @brief Return the <value> associated with <key> in the hash map.
 */
void INTERNAL *qt_hash_get(qt_hash        h,
                           const qt_key_t key);

/*!
 * @fn qt_hash_get_locked(qt_hash        h,
 *                        const qt_key_t key)
 * @brief Same as qt_hash_get(), but assumes that the hash map has been locked
 *	already.
 */
void INTERNAL *qt_hash_get_locked(qt_hash        h,
                                  const qt_key_t key);

/*!
 * @fn qt_hash_count(qt_hash h)
 * @brief Return the number of key/value pairs stored in the hash map.
 */
size_t INTERNAL qt_hash_count(qt_hash h);

/*!
 * @fn qt_hash_callback(qt_hash             h,
 *                      qt_hash_callback_fn f,
 *                      void               *arg)
 * @brief Execute the function <f> on all of the key/value pairs stored in the
 *	hash map.
 */
void INTERNAL qt_hash_callback(qt_hash             h,
                               qt_hash_callback_fn f,
                               void               *arg);

/*!
 * @fn qt_hash_lock(qt_hash h)
 * @brief Lock the hash map.
 */
void INTERNAL qt_hash_lock(qt_hash h);

/*!
 * @fn qt_hash_unlock(qt_hash h)
 * @brief Unock the hash map.
 */
void INTERNAL qt_hash_unlock(qt_hash h);

#ifdef __cplusplus
}
#endif

#endif // ifndef QT_HASH_H
/* vim:set expandtab: */
