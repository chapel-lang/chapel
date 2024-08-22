/*
 * Copyright (c) 2011-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cray Inc.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _OFI_LIST_H_
#define _OFI_LIST_H_

#include "config.h"

#include <sys/types.h>
#include <stdlib.h>

#include <rdma/fabric.h>

#include <ofi_signal.h>
#include <ofi_lock.h>


enum ofi_list_end {
	OFI_LIST_TAIL,
	OFI_LIST_HEAD
};

/*
 * Double-linked list
 */
struct dlist_entry {
	struct dlist_entry	*next;
	struct dlist_entry	*prev;
};

#define DLIST_INIT(addr) { addr, addr }
#define DEFINE_LIST(name) struct dlist_entry name = DLIST_INIT(&name)

static inline void dlist_init(struct dlist_entry *head)
{
	head->next = head;
	head->prev = head;
}

static inline int dlist_empty(struct dlist_entry *head)
{
	return head->next == head;
}

static inline void
dlist_insert_after(struct dlist_entry *item, struct dlist_entry *head)
{
	item->next = head->next;
	item->prev = head;
	head->next->prev = item;
	head->next = item;
}

static inline void
dlist_insert_before(struct dlist_entry *item, struct dlist_entry *head)
{
	dlist_insert_after(item, head->prev);
}

#define dlist_insert_head dlist_insert_after
#define dlist_insert_tail dlist_insert_before

static inline void dlist_remove(struct dlist_entry *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;
}

static inline void dlist_remove_init(struct dlist_entry *item)
{
	dlist_remove(item);
	dlist_init(item);
}

#define dlist_first_entry_or_null(head, type, member) ({	\
	struct dlist_entry *pos = (head)->next;				\
	pos != (head) ? container_of((pos), type, member) : NULL;	\
})

#define dlist_pop_front(head, type, container, member)			\
	do {								\
		container = container_of((head)->next, type, member);	\
		dlist_remove((head)->next);				\
	} while (0)

#define dlist_foreach(head, item) 						\
	for ((item) = (head)->next; (item) != (head); (item) = (item)->next)

#define dlist_foreach_reverse(head, item) 					\
	for ((item) = (head)->prev; (item) != (head); (item) = (item)->prev)

#define dlist_foreach_container(head, type, container, member)			\
	for ((container) = container_of((head)->next, type, member);		\
	     &((container)->member) != (head);					\
	     (container) = container_of((container)->member.next,		\
					type, member))

#define dlist_foreach_container_reverse(head, type, container, member)		\
	for ((container) = container_of((head)->prev, type, member);		\
	     &((container)->member) != (head);					\
	     (container) = container_of((container)->member.prev,		\
					type, member))

#define dlist_foreach_safe(head, item, tmp)					\
	for ((item) = (head)->next, (tmp) = (item)->next; (item) != (head);	\
             (item) = (tmp), (tmp) = (item)->next)

#define dlist_foreach_reverse_safe(head, item, tmp)				\
	for ((item) = (head)->prev, (tmp) = (item)->prev; (item) != (head);	\
             (item) = (tmp), (tmp) = (item)->prev)

#define dlist_foreach_container_safe(head, type, container, member, tmp)	\
	for ((container) = container_of((head)->next, type, member),		\
	     (tmp) = (container)->member.next;					\
	     &((container)->member) != (head);					\
	     (container) = container_of((tmp), type, member),			\
	     (tmp) = (container)->member.next)

#define dlist_foreach_container_reverse_safe(head, type, container, member, tmp)\
	for ((container) = container_of((head)->prev, type, member),		\
	     (tmp) = (container)->member.prev;					\
	     &((container)->member) != (head);					\
	     (container) = container_of((tmp), type, member),			\
	     (tmp) = (container)->member.prev)

typedef int dlist_func_t(struct dlist_entry *item, const void *arg);

static inline struct dlist_entry *
dlist_find_first_match(struct dlist_entry *head, dlist_func_t *match,
		       const void *arg)
{
	struct dlist_entry *item;

	dlist_foreach(head, item) {
		if (match(item, arg))
			return item;
	}

	return NULL;
}

static inline struct dlist_entry *
dlist_remove_first_match(struct dlist_entry *head, dlist_func_t *match,
			 const void *arg)
{
	struct dlist_entry *item;

	item = dlist_find_first_match(head, match, arg);
	if (item)
		dlist_remove(item);

	return item;
}

static inline void dlist_insert_order(struct dlist_entry *head, dlist_func_t *order,
				      struct dlist_entry *entry)
{
	struct dlist_entry *item;

	item = dlist_find_first_match(head, order, entry);
	if (item)
		dlist_insert_before(entry, item);
	else
		dlist_insert_tail(entry, head);
}

/* splices list at the front of the list 'head'
 *
 * BEFORE:
 * head:      HEAD->a->b->c->HEAD
 * to_splice: HEAD->d->e->HEAD
 *
 * AFTER:
 * head:      HEAD->d->e->a->b->c->HEAD
 * to_splice: HEAD->HEAD (empty list)
 */
static inline void dlist_splice_head(struct dlist_entry *head,
				     struct dlist_entry *to_splice)
{
	if (dlist_empty(to_splice))
		return;

	/* hook first element of 'head' to last element of 'to_splice' */
	head->next->prev = to_splice->prev;
	to_splice->prev->next = head->next;

	/* put first element of 'to_splice' as first element of 'head' */
	head->next = to_splice->next;
	head->next->prev = head;

	/* set list to empty */
	dlist_init(to_splice);
}

/* splices list at the back of the list 'head'
 *
 * BEFORE:
 * head:      HEAD->a->b->c->HEAD
 * to_splice: HEAD->d->e->HEAD
 *
 * AFTER:
 * head:      HEAD->a->b->c->d->e->HEAD
 * to_splice: HEAD->HEAD (empty list)
 */
static inline void dlist_splice_tail(struct dlist_entry *head,
				     struct dlist_entry *to_splice)
{
	dlist_splice_head(head->prev, to_splice);
}

/*
 * Multi-threaded Double-linked list
 */
struct dlist_ts {
	struct dlist_entry	head;
	ofi_spin_t		lock;
};

static inline void dlist_ts_init(struct dlist_ts *list)
{
	ofi_spin_init(&list->lock);
	dlist_init(&list->head);
}

static inline int dlist_ts_empty(struct dlist_ts *list)
{
	return dlist_empty(&list->head);
}

static inline void
dlist_ts_insert_after(struct dlist_ts *list, struct dlist_entry *item,
		      struct dlist_entry *head)
{
	ofi_spin_lock(&list->lock);
	dlist_insert_after(item, head);
	ofi_spin_unlock(&list->lock);
}

static inline void
dlist_ts_insert_before(struct dlist_ts *list, struct dlist_entry *item,
		       struct dlist_entry *head)
{
	dlist_ts_insert_after(list, item, head->prev);
}

#define dlist_ts_insert_head(list, item) dlist_ts_insert_after(list, item, &(list)->head)
#define dlist_ts_insert_tail(list, item) dlist_ts_insert_before(list, item, &(list)->head)

static inline void
dlist_ts_remove(struct dlist_ts *list, struct dlist_entry *item)
{
	ofi_spin_lock(&list->lock);
	dlist_remove(item);
	ofi_spin_unlock(&list->lock);
}

#define dlist_ts_pop_front(list, type, container, member)		\
	do {								\
		ofi_spin_lock(&(list)->lock);			\
		if (dlist_ts_empty(list)) {				\
			container = NULL;				\
		} else {						\
			dlist_pop_front(&(list)->head, type,		\
					container, member);		\
		}							\
		ofi_spin_unlock(&(list)->lock);			\
	} while (0)

#define dlist_ts_foreach_end(list)				\
		ofi_spin_unlock(&(list)->lock);		\
	} while (0)

#define dlist_ts_foreach(list, head, item)			\
	{							\
		ofi_spin_lock(&(list)->lock);		\
		dlist_foreach(list, head, item)

#define dlist_ts_foreach_reverse(list, head, item)		\
	{							\
		ofi_spin_lock(&(list)->lock);		\
		dlist_foreach_reverse(list, head, item)

#define dlist_ts_foreach_container(list, head, type, container, member)		\
	{									\
		ofi_spin_lock(&(list)->lock);				\
		dlist_foreach_container(type, container, member)

#define dlist_ts_foreach_container_reverse(list, head, type, container, member)\
	{									\
		ofi_spin_lock(&(list)->lock);				\
		dlist_foreach_container_reverse(type, container, member)

#define dlist_ts_foreach_safe(list, head, item, tmp)				\
	{									\
		ofi_spin_lock(&(list)->lock);				\
		dlist_foreach_safe(head, item, tmp)

#define dlist_ts_foreach_reverse_safe(list, head, item, tmp)			\
	{									\
		ofi_spin_lock(&(list)->lock);				\
		dlist_foreach_reverse_safe(head, item, tmp)

#define dlist_ts_foreach_container_safe(list, head, type, container,	\
					member, tmp)			\
	{								\
		ofi_spin_lock(&(list)->lock);			\
		dlist_foreach_container_safe(head, type, container,	\
					     member, tmp)

#define dlist_ts_foreach_container_reverse_safe(list, head, type, container,\
					member, tmp)				\
	{									\
		ofi_spin_lock(&(list)->lock);				\
		dlist_foreach_container_reverse_safe(head, type, container,	\
					     member, tmp)

static inline struct dlist_entry *
dlist_ts_find_first_match(struct dlist_ts *list, struct dlist_entry *head,
			  dlist_func_t *match, const void *arg)
{
	struct dlist_entry *item;

	ofi_spin_lock(&list->lock);
	item = dlist_find_first_match(head, match, arg);
	ofi_spin_unlock(&list->lock);

	return item;
}

static inline struct dlist_entry *
dlist_ts_remove_first_match(struct dlist_ts *list, struct dlist_entry *head,
			    dlist_func_t *match, const void *arg)
{
	struct dlist_entry *item;

	ofi_spin_lock(&list->lock);
	item = dlist_remove_first_match(head, match, arg);
	ofi_spin_unlock(&list->lock);

	return item;
}

#define dlist_ts_splice_head(list, head, to_splice)	\
	{						\
		ofi_spin_lock(&(list)->lock);	\
		dlist_splice_head(head, to_splice);	\
		ofi_spin_unlock(&list->lock);		\
	} while(0)

#define dlist_ts_splice_tail(list, head, to_splice)		\
	{							\
		dlist_ts_splice_head(head->prev, to_splice);	\
	} while(0)

/*
 * Single-linked list
 */
struct slist_entry {
	struct slist_entry	*next;
};

struct slist {
	struct slist_entry	*head;
	struct slist_entry	*tail;
};

static inline void slist_init(struct slist *list)
{
	list->head = list->tail = NULL;
}

static inline int slist_empty(struct slist *list)
{
	return !list->head;
}

static inline void slist_insert_head(struct slist_entry *item, struct slist *list)
{
	if (slist_empty(list)) {
		list->tail = item;
		item->next = NULL;
	} else {
		item->next = list->head;
	}

	list->head = item;
}

static inline void slist_insert_tail(struct slist_entry *item, struct slist *list)
{
	if (slist_empty(list))
		list->head = item;
	else
		list->tail->next = item;

	item->next = NULL;
	list->tail = item;
}

static inline struct slist_entry *slist_remove_head(struct slist *list)
{
	struct slist_entry *item;

	item = list->head;
	if (list->head == list->tail)
		slist_init(list);
	else
		list->head = item->next;
#if ENABLE_DEBUG
	if (item) {
		item->next = NULL;
	}
#endif
	return item;
}

#define slist_foreach(list, item, prev)				\
	for ((prev) = NULL, (item) = (list)->head; (item); 	\
			(prev) = (item), (item) = (item)->next)

#define slist_remove_head_container(list, type, container, member)	\
	do {								\
		if (slist_empty(list)) {				\
			container = NULL;				\
		} else {						\
			container = container_of((list)->head, type,    \
					member);			\
			slist_remove_head(list);			\
		}							\
	} while (0)

typedef int slist_func_t(struct slist_entry *item, const void *arg);

static inline struct slist_entry *
slist_find_first_match(const struct slist *list, slist_func_t *match,
			const void *arg)
{
	struct slist_entry *item;
	for (item = list->head; item; item = item->next) {
		if (match(item, arg))
			return item;
	}

	return NULL;
}

static inline void
slist_insert_before_first_match(struct slist *list, slist_func_t *match,
				struct slist_entry *entry)
{
	struct slist_entry *cur, *prev;

	slist_foreach(list, cur, prev) {
		if (match(cur, entry)) {
			if (!prev) {
				slist_insert_head(entry, list);
			} else {
				entry->next = prev->next;
				prev->next = entry;
			}
			return;
		}
	}
	slist_insert_tail(entry, list);
}

static inline void slist_remove(struct slist *list,
		struct slist_entry *item, struct slist_entry *prev)
{
	if (prev)
		prev->next = item->next;
	else
		list->head = item->next;

	if (!item->next)
		list->tail = prev;
}

static inline struct slist_entry *
slist_remove_first_match(struct slist *list, slist_func_t *match, const void *arg)
{
	struct slist_entry *item, *prev;

	slist_foreach(list, item, prev) {
		if (match(item, arg)) {
			slist_remove(list, item, prev);
			return item;
		}
	}

	return NULL;
}

static inline void slist_swap(struct slist *dst, struct slist *src)
{
	struct slist_entry *dst_head = dst->head;
	struct slist_entry *dst_tail = dst->tail;

	dst->head = src->head;
	dst->tail = src->tail;

	src->head = dst_head;
	src->tail = dst_tail;
}

/* splices src list at the front of the dst list
 *
 * BEFORE:
 * dst: HEAD->a->b->c->TAIL
 * src: HEAD->d->e->TAIL
 *
 * AFTER:
 * dst: HEAD->d->e->a->b->c->TAIL
 * src: HEAD->TAIL (empty list)
 */
static inline struct slist *
slist_splice_head(struct slist *dst, struct slist *src)
{
	if (slist_empty(src))
		return dst;

	if (slist_empty(dst)) {
		slist_swap(dst, src);
		return dst;
	}

	src->tail->next = dst->head;
	dst->head = src->head;

	slist_init(src);

	return dst;
}

/* splices src list at the back of the dst list
 *
 * BEFORE:
 * dst: HEAD->a->b->c->TAIL
 * src: HEAD->d->e->TAIL
 *
 * AFTER:
 * dst: HEAD->a->b->c->d->e->TAIL
 * src: HEAD->TAIL (empty list)
 */
static inline struct slist *
slist_splice_tail(struct slist *dst, struct slist *src)
{
	if (slist_empty(src))
		return dst;

	if (slist_empty(dst)) {
		slist_swap(dst, src);
		return dst;
	}

	dst->tail->next = src->head;
	dst->tail = src->tail;

	slist_init(src);

	return dst;
}

/*
 * Singly-linked list with blocking wait-until-avail support
 */

struct slistfd {
	struct slist 		list;
	struct fd_signal	signal;
};

static inline int slistfd_init(struct slistfd *list)
{
	slist_init(&list->list);
	return fd_signal_init(&list->signal);
}

static inline void slistfd_free(struct slistfd *list)
{
	fd_signal_free(&list->signal);
}

static inline int slistfd_empty(struct slistfd *list)
{
	return slist_empty(&list->list);
}

static inline void
slistfd_insert_head(struct slist_entry *item, struct slistfd *list)
{
	slist_insert_head(item, &list->list);
	fd_signal_set(&list->signal);
}

static inline void
slistfd_insert_tail(struct slist_entry *item, struct slistfd *list)
{
	slist_insert_tail(item, &list->list);
	fd_signal_set(&list->signal);
}

static inline struct slist_entry *slistfd_remove_head(struct slistfd *list)
{
	struct slist_entry *entry = slist_remove_head(&list->list);
	if (entry)
		fd_signal_reset(&list->signal);
	return entry;
}

static inline int slistfd_wait_avail(struct slistfd *list, int timeout)
{
	int ret;

	if (!slistfd_empty(list))
		return 1;

	ret = fd_signal_poll(&list->signal, timeout);
	return ret ? ret : !slistfd_empty(list);
}

static inline int slistfd_get_fd(struct slistfd *list)
{
	return fd_signal_get(&list->signal);
}

/*
 * Double-linked list with blocking wait-until-avail support
 */

struct dlistfd_head {
	struct dlist_entry list;
	struct fd_signal   signal;
};

static inline int dlistfd_head_init(struct dlistfd_head *head)
{
	dlist_init(&head->list);
	return fd_signal_init(&head->signal);
}

static inline void dlistfd_head_free(struct dlistfd_head *head)
{
	fd_signal_free(&head->signal);
}

static inline int dlistfd_empty(struct dlistfd_head *head)
{
	return dlist_empty(&head->list);
}

static inline void dlistfd_signal(struct dlistfd_head *head)
{
	fd_signal_set(&head->signal);
}

static inline void dlistfd_reset(struct dlistfd_head *head)
{
	if (dlistfd_empty(head))
		fd_signal_reset(&head->signal);
}

static inline void
dlistfd_insert_head(struct dlist_entry *item, struct dlistfd_head *head)
{
	dlist_insert_after(item, &head->list);
	dlistfd_signal(head);
}

static inline void
dlistfd_insert_tail(struct dlist_entry *item, struct dlistfd_head *head)
{
	dlist_insert_before(item, &head->list);
	dlistfd_signal(head);
}

static inline void dlistfd_remove(struct dlist_entry *item, struct dlistfd_head *head)
{
	dlist_remove(item);
	dlistfd_reset(head);
}

static inline int dlistfd_wait_avail(struct dlistfd_head *head, int timeout)
{
	int ret;

	if (!dlistfd_empty(head))
		return 1;

	ret = fd_signal_poll(&head->signal, timeout);
	return ret ? ret : !dlistfd_empty(head);
}

static inline struct dlist_entry *
dlistfd_remove_first_match(struct dlistfd_head *head, dlist_func_t *match,
			   const void *arg)
{
	struct dlist_entry *entry =
		dlist_remove_first_match(&head->list, match, arg);
	if (entry)
		dlistfd_reset(head);
	return entry;
}

#endif /* _OFI_LIST_H_ */
