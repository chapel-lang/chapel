/*
 * Description: PSM list handling code
 * Copyright (c) 2014-2015 Intel Corporation. All rights reserved.
 * Terms of use are as specified in license.txt
 */


#ifndef _GASNET_CORE_LIST_H
#define _GASNET_CORE_LIST_H

#include <gasnet_core_internal.h>


static void gasnetc_list_init(gasnetc_list_t* list,
        unsigned int num_items, size_t item_size)
{
    gasneti_spinlock_init(&list->lock);

    if(num_items == 0) {
        list->head.next = NULL;
        list->tail = &list->head;
    } else {
        uintptr_t items;
        gasnetc_item_t *item;
        int i;

        items = (uintptr_t)gasneti_malloc(num_items * item_size);
        gasneti_leak((void*)items);

        for(i = 0; i < num_items - 1; i++) {
            item = (gasnetc_item_t *)(items + (i * item_size));
            item->next = (gasnetc_item_t *)
                    (items + ((i + 1) * item_size));
        }

        item = (gasnetc_item_t *)
            (items + ((num_items - 1) * item_size));
        item->next = NULL;
        list->head.next = (gasnetc_item_t *)items;
        list->tail = item;
    }
}

static gasnetc_item_t *gasnetc_list_remove_alloc_inner(gasnetc_list_t *list,
        unsigned int num_items, size_t item_size)
{
    /* Allocate new items.  There's a minor race condition here:
       The branch above is performed without the lock held, so it is
       possible for multiple processes to enter and allocate a new block of
       items.  The new items are added safely, so it's not much of a
       problem. */
    uintptr_t slab;
    uintptr_t cur_slab;
    gasnetc_item_t *cur_item;
    int i;

    gasneti_assert(num_items > 0);
    gasneti_assert(item_size > 0);

    /* Skip initializing and adding the first item allocated.  That
       item will be returned to the caller. */
    slab = (uintptr_t)gasneti_malloc(num_items * item_size);
    gasneti_leak((void*)slab);

    cur_slab = slab + item_size;
    cur_item = (gasnetc_item_t *)cur_slab;
    for(i = 1; i < num_items - 1; i++) {
        cur_slab += item_size;
        cur_item->next = (gasnetc_item_t *)cur_slab;
        cur_item = (gasnetc_item_t *)cur_slab;
    }

    /* At this point, cur_item points to the last new slab item. */
    gasneti_spinlock_lock(&list->lock);
    cur_item->next = list->head.next;
    list->head.next = (gasnetc_item_t *)(slab + item_size);
    if(list->tail == &list->head) {
        list->tail = cur_item;
    }
    gasneti_spinlock_unlock(&list->lock);

    return (gasnetc_item_t *)slab;
}

/* Try to remove an item from the list.  If the list is empty, allocate
   a slab of num_items and return a new item. */
GASNETI_INLINE(gasnetc_list_remove_alloc)
gasnetc_item_t *gasnetc_list_remove_alloc(gasnetc_list_t *list,
        unsigned int num_items, size_t item_size)
{
    gasnetc_item_t *item;

    if(list->head.next == NULL) {
        return gasnetc_list_remove_alloc_inner(list,
                num_items, item_size);
    } else {
        gasneti_spinlock_lock(&list->lock);
        item = list->head.next;
        if(item != NULL) {
            list->head.next = item->next;
            if(item->next == NULL) {
                list->tail = &list->head;
            }
        }
        gasneti_spinlock_unlock(&list->lock);
    }

    return item;
}

GASNETI_INLINE(gasnetc_list_add_head)
void gasnetc_list_add_head(gasnetc_list_t *list,
        gasnetc_item_t *item)
{
    gasneti_spinlock_lock(&list->lock);
    if(list->head.next == NULL)
        list->tail = item;
    item->next = list->head.next;
    list->head.next = item;
    gasneti_spinlock_unlock(&list->lock);
}

GASNETI_INLINE(gasnetc_list_add_tail)
void gasnetc_list_add_tail(gasnetc_list_t *list,
        gasnetc_item_t *item)
{
    item->next = NULL;

    gasneti_spinlock_lock(&list->lock);
    list->tail->next = item;
    list->tail = item;
    gasneti_spinlock_unlock(&list->lock);
}

GASNETI_INLINE(gasnetc_list_remove)
gasnetc_item_t *gasnetc_list_remove(gasnetc_list_t *list)
{
    gasnetc_item_t *item;

    if(list->head.next == NULL) {
        return NULL;
    }

    gasneti_spinlock_lock(&list->lock);
    item = list->head.next;
    if(item != NULL) {
        list->head.next = item->next;
        if(item->next == NULL) {
            list->tail = &list->head;
        }
    }
    gasneti_spinlock_unlock(&list->lock);

    return item;
}

GASNETI_INLINE(gasnetc_list_drain)
gasnetc_item_t *gasnetc_list_drain(gasnetc_list_t* list)
{
    gasnetc_item_t *head;

    gasneti_spinlock_lock(&list->lock);
    head = list->head.next;
    list->head.next = NULL;
    list->tail = &list->head;
    gasneti_spinlock_unlock(&list->lock);

    return head;
}

#endif
