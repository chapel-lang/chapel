#ifndef QT_DICTIONARY_H
#define QT_DICTIONARY_H
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"

Q_STARTCXX /* */

#define DELETE_SUPPORT
#define ERROR      ((void *)(-1))
#define BKT_POW    20
#define NO_BUCKETS (1 << BKT_POW)

struct tlrw_lock;
typedef int (*qt_dict_key_equals_f)(void *,
                                    void *);
typedef int (*qt_dict_hash_f)(void *);
typedef void (*qt_dict_cleanup_f)(void *,
                                  void *);

struct list_entry {
    void              *value;
    void              *key;
    uint64_t           hashed_key;
    struct list_entry *next;
};

typedef struct list_entry list_entry;
typedef struct qt_dictionary qt_dictionary;
typedef struct qt_dictionary_iterator qt_dictionary_iterator;

/*
 *      Creates a dictionary, with the key comparison function parameter
 * and a hashcode function
 *
 * the signature of the key comparsion function is: int my_key_equals(void* key1, void* key2)
 *              and it should return 1 ("true") if the keys are equal and 0 otherwise
 *
 * the signature of the qt_dict_hash_f function is: int my_hashcode(void* string)
 *              and it can return any integer value.
 *
 * if my_key_equals (A, B) = 1, then my_hashcode(A) == my_hashcode(B)
 */
qt_dictionary *qt_dictionary_create(qt_dict_key_equals_f eq,
                                    qt_dict_hash_f       hash,
                                    qt_dict_cleanup_f    cleanup);

/*
 *      Destroys the dictionary d
 *      d must be empty (keys and values must have been cleaned up already (or else, leaks)
 *
 */
void qt_dictionary_destroy(qt_dictionary *d);

/*
 *      Inserts a key, value pair in the dictionary
 *      returns void*:
 *                      ADDR - address of item found in the hashmap after the current put
 *                      NULL - if the insert failed because of an error
 *
 */
void *qt_dictionary_put(qt_dictionary *dict,
                        void          *key,
                        void          *value);

/*
 *      Inserts a key, value pair in the dictionary
 *      returns void*:
 *                      ADDR - address of item found in the hashmap after the current put
 *                      (if "key" was present in the dictionary, the address of the old
 *                      entry is returned, as opposed to "value")
 *                      NULL - if the insert failed because of an error
 *
 */
void *qt_dictionary_put_if_absent(qt_dictionary *dict,
                                  void          *key,
                                  void          *value);

/*
 *      Gets a value from the dictionary for a given key
 *      returns:
 *                      item - if get was successful and
 *                              item was present in the dictionary
 *                      NULL - if item was not present in the dictionary or error
 *
 */
void *qt_dictionary_get(qt_dictionary *dict,
                        void          *key);

/*
 *      Removes a key,value pair from the dictionary
 *      returns:
 *                      item - if the item was present and successfully removed
 *                      NULL - if the item identified by key was not in the hash
 *
 */
void *qt_dictionary_delete(qt_dictionary *dict,
                           void          *key);

/*
 *      Creates a new iterator on the dictionary dict
 *      returns:
 *                      addr - address of new iterator if creation was successful
 *                      ERROR - if an error occurred
 *
 */
qt_dictionary_iterator *qt_dictionary_iterator_create(qt_dictionary *dict);

/*
 *      Destroys the iterator it
 *
 */
void qt_dictionary_iterator_destroy(qt_dictionary_iterator *it);

/*
 *      Advances the iterator and retrieves the next entry
 *      returns:
 *                      addr - address of the next entry in the hash, if one was found
 *                      NULL -  if there are no more elements in hash
 *                      ERROR -  if an error occurred (e.g. iterator was destroyed)
 *      Note: For handling concurrency, if an item was deleted while using the iterator,
 *                the behavior is undefined (can return an error, signal empty hash or
 *                correctly return the next entry)
 */
list_entry *qt_dictionary_iterator_next(qt_dictionary_iterator *it);

/*
 *      Retrieves the curent entry in iterator it
 *      returns:
 *                      addr - address of the next entry in the hash, if one was found
 *                      NULL -  if there are no more elements in hash
 *                      ERROR -  if an error occurred (e.g. iterator was destroyed)
 *      Note: For handling concurrency, if an item was deleted while using the iterator,
 *                the behavior is undefined (can return an error, signal empty hash or
 *                correctly return the next entry)
 */
list_entry *qt_dictionary_iterator_get(const qt_dictionary_iterator *it);

/*
 *      Creates a new iterator, pointing to the item past the end of the list (which is NULL)
 *      returns:
 *              addr - address of the newly created iterator
 *              NULL - if dictionary is null or not allocated properly
 */
qt_dictionary_iterator *qt_dictionary_end(qt_dictionary *dict);

/*
 *      Tests equality for two iterators
 *      requires that they are both walking the same dictionaries and are located on the same positions
 */
int qt_dictionary_iterator_equals(qt_dictionary_iterator *a,
                                  qt_dictionary_iterator *b);

/*
 *      Creates a copy of the iteartoor given as parameter.
 *      Returns new iterator if successfull and NULL otherwise
 */
qt_dictionary_iterator *qt_dictionary_iterator_copy(qt_dictionary_iterator *b);

/*
 * Displays debugging info for dictionary buckets
 */
void qt_dictionary_printbuckets(qt_dictionary *dict);

Q_ENDCXX /* */
#endif // QT_DICTIONARY_H
/* vim:set expandtab: */
