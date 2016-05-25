/*   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/gasnet_portals4_hash.h $
 * Description: GASNet portals4-conduit lock-free hash table implementation
 * Copyright 2012, Sandia National Laboratories
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETC_HASH_H
#define GASNETC_HASH_H

#include <stddef.h>      /* for size_t (according to C89) */

#ifdef __cplusplus
extern "C" {
#endif

typedef const void *gasnetc_key_t;
typedef struct gasnetc_hash_s *gasnetc_hash;
typedef void (*gasnetc_hash_callback_fn)(const gasnetc_key_t, void *, void *);
typedef void (*gasnetc_hash_deallocator_fn)(void *);

void gasnetc_hash_initialize_subsystem(void);

/*!
 * @fn gasnetc_hash_create(int needSync)
 * @brief Allocate and initialize a hash map
 */
gasnetc_hash gasnetc_hash_create();

/*!
 * @fn gasnetc_hash_destroy(gasnetc_hash h)
 * @brief Deallocates a hash map and any mappings that remain in it.
 */
void  gasnetc_hash_destroy(gasnetc_hash h);

/*!
 * @fn gasnetc_hash_destroy_deallocate(gasnetc_hash                h,
 *                                gasnetc_hash_deallocator_fn f)
 * @brief Deallocates a hash map and any mappings that remain in it. For each
 *	mapping, it applies the deallocator function to deallocate any attached
 *	memory.
 */
void  gasnetc_hash_destroy_deallocate(gasnetc_hash                h,
                                         gasnetc_hash_deallocator_fn f);
/*!
 * @fn gasnetc_hash_put(gasnetc_hash        h,
 *                 const gasnetc_key_t key,
 *                 void          *value)
 * @brief Insert a mapping from <key> to <value> into the hash map. This
 *      function returns 1 if the insertion succeeded, or 0 is the insertion
 *      failed.
 */
int  gasnetc_hash_put(gasnetc_hash  h,
                         gasnetc_key_t key,
                         void    *value);

/*!
 * @fn gasnetc_hash_put_find(gasnetc_hash        h,
 *                 const gasnetc_key_t key,
 *                 void          *value)
 *                 void          **found_p)
 * @brief Insert a mapping from <key> to <value> into the hash map or
 *      find the existing mapping. This function returns NULL if the
 *      insertion succeeded, or the value of the existing mapping on
 *      failure.
 */
void*  gasnetc_hash_put_find(gasnetc_hash  h,
                         gasnetc_key_t key,
                         void    *value);

/*!
 * @fn gasnetc_hash_remove(gasnetc_hash        h,
 *                    const gasnetc_key_t key)
 * @brief Remove the mapping for <key> from the hash map.
 */
int  gasnetc_hash_remove(gasnetc_hash        h,
                            const gasnetc_key_t key);

/*!
 * @fn gasnetc_hash_get(gasnetc_hash        h,
 *                 const gasnetc_key_t key)
 * @brief Return the <value> associated with <key> in the hash map.
 */
void  *gasnetc_hash_get(gasnetc_hash        h,
                           const gasnetc_key_t key);

/*!
 * @fn gasnetc_hash_count(gasnetc_hash h)
 * @brief Return the number of key/value pairs stored in the hash map.
 */
size_t  gasnetc_hash_count(gasnetc_hash h);

/*!
 * @fn gasnetc_hash_callback(gasnetc_hash             h,
 *                      gasnetc_hash_callback_fn f,
 *                      void               *arg)
 * @brief Execute the function <f> on all of the key/value pairs stored in the
 *	hash map.
 */
void  gasnetc_hash_callback(gasnetc_hash             h,
                               gasnetc_hash_callback_fn f,
                               void               *arg);

#ifdef __cplusplus
}
#endif

#endif /* ifndef GASNETC_HASH_H */
/* vim:set expandtab: */
