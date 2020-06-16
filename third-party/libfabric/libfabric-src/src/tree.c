/*
 * Copyright (c) 2015 Cray Inc. All rights reserved.
 * Copyright (c) 2018 Intel Corp, Inc.  All rights reserved.
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
 */

/*
 * Copied from http://oopweb.com/Algorithms/Documents/Sman/VolumeFrames.html?/Algorithms/Documents/Sman/Volume/RedBlackTrees_files/s_rbt.htm
 *
 * Disclosure from the author's main page:
 * (http://oopweb.com/Algorithms/Documents/Sman/VolumeFrames.html?/Algorithms/Documents/Sman/Volume/RedBlackTrees_files/s_rbt.htm)
 *
 *     Source code when part of a software project may be used freely
 *     without reference to the author.
 *
 */

// reentrant red-black tree

#include <assert.h>

#include <ofi_tree.h>
#include <ofi_osd.h>
#include <rdma/fi_errno.h>


static struct ofi_rbnode *ofi_rbnode_alloc(struct ofi_rbmap *map)
{
	struct ofi_rbnode *node;

	if (!map->free_list)
		return malloc(sizeof(*node));

	node = map->free_list;
	map->free_list = node->right;
	return node;
}

static void ofi_rbnode_free(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	node->right = map->free_list ? map->free_list : NULL;
	map->free_list = node;
}

void ofi_rbmap_init(struct ofi_rbmap *map,
		int (*compare)(struct ofi_rbmap *map, void *key, void *data))
{
	map->compare = compare;

	map->root = &map->sentinel;
	map->sentinel.left = &map->sentinel;
	map->sentinel.right = &map->sentinel;
	map->sentinel.parent = NULL;
	map->sentinel.color = BLACK;
	map->sentinel.data = NULL;
}

struct ofi_rbmap *
ofi_rbmap_create(int (*compare)(struct ofi_rbmap *map, void *key, void *data))
{
	struct ofi_rbmap *map;

	map = calloc(1, sizeof *map);
	if (map)
		ofi_rbmap_init(map, compare);
	return map;
}

static void ofi_delete_tree(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	if (node == &map->sentinel)
		return;

	ofi_delete_tree(map, node->left);
	ofi_delete_tree(map, node->right);
	free(node);
}

void ofi_rbmap_cleanup(struct ofi_rbmap *map)
{
	ofi_delete_tree(map, map->root);
}

void ofi_rbmap_destroy(struct ofi_rbmap *map)
{
	struct ofi_rbnode *node;

	ofi_rbmap_cleanup(map);
	while (map->free_list) {
		node = map->free_list;
		map->free_list = node->right;
		free(node);
	}
	free(map);
}

int ofi_rbmap_empty(struct ofi_rbmap *map)
{
	return map->root == &map->sentinel;
}

static void ofi_rotate_left(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	struct ofi_rbnode *y = node->right;

	node->right = y->left;
	if (y->left != &map->sentinel)
		y->left->parent = node;

	if (y != &map->sentinel)
		y->parent = node->parent;
	if (node->parent) {
		if (node== node->parent->left)
			node->parent->left = y;
		else
			node->parent->right = y;
	} else {
		map->root = y;
	}

	y->left = node;
	if (node != &map->sentinel)
		node->parent = y;
}

static void ofi_rotate_right(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	struct ofi_rbnode *y = node->left;

	node->left = y->right;
	if (y->right != &map->sentinel)
		y->right->parent = node;

	if (y != &map->sentinel)
		y->parent = node->parent;
	if (node->parent) {
		if (node == node->parent->right)
			node->parent->right = y;
		else
			node->parent->left = y;
	} else {
		map->root = y;
	}

	y->right = node;
	if (node != &map->sentinel)
		node->parent = y;
}

static void
ofi_insert_rebalance(struct ofi_rbmap *map, struct ofi_rbnode *x)
{
	struct ofi_rbnode *y;

	while (x != map->root && x->parent->color == RED) {
		if (x->parent == x->parent->parent->left) {
			y = x->parent->parent->right;
			if (y->color == RED) {
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				if (x == x->parent->right) {
					x = x->parent;
					ofi_rotate_left(map, x);
				}

				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				ofi_rotate_right(map, x->parent->parent);
			}
		} else {
			y = x->parent->parent->left;
			if (y->color == RED) {
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			} else {
				if (x == x->parent->left) {
					x = x->parent;
					ofi_rotate_right(map, x);
				}
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				ofi_rotate_left(map, x->parent->parent);
			}
		}
	}
	map->root->color = BLACK;
}

int ofi_rbmap_insert(struct ofi_rbmap *map, void *key, void *data,
		     struct ofi_rbnode **ret_node)
{
	struct ofi_rbnode *current, *parent, *node;
	int ret;

	current = map->root;
	parent = NULL;

	while (current != &map->sentinel) {
		ret = map->compare(map, key, current->data);
		if (ret == 0)
			return -FI_EALREADY;

		parent = current;
		current = (ret < 0) ? current->left : current->right;
	}

	node = ofi_rbnode_alloc(map);
	if (!node)
		return -FI_ENOMEM;

	node->parent = parent;
	node->left = &map->sentinel;
	node->right = &map->sentinel;
	node->color = RED;
	node->data = data;

	if (parent) {
		if (map->compare(map, key, parent->data) < 0)
			parent->left = node;
		else
			parent->right = node;
	} else {
		map->root = node;
	}

	ofi_insert_rebalance(map, node);
	if (ret_node)
		*ret_node = node;
	return 0;
}

static void ofi_delete_rebalance(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	struct ofi_rbnode *w;

	while (node != map->root && node->color == BLACK) {
		if (node == node->parent->left) {
			w = node->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				node->parent->color = RED;
				ofi_rotate_left(map, node->parent);
				w = node->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				node = node->parent;
			} else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					ofi_rotate_right(map, w);
					w = node->parent->right;
				}
				w->color = node->parent->color;
				node->parent->color = BLACK;
				w->right->color = BLACK;
				ofi_rotate_left(map, node->parent);
				node = map->root;
			}
		} else {
			w = node->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				node->parent->color = RED;
				ofi_rotate_right(map, node->parent);
				w = node->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				node = node->parent;
			} else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					ofi_rotate_left(map, w);
					w = node->parent->left;
				}
				w->color = node->parent->color;
				node->parent->color = BLACK;
				w->left->color = BLACK;
				ofi_rotate_right(map, node->parent);
				node = map->root;
			}
		}
	}
	node->color = BLACK;
}

static void ofi_rbmap_replace_node_ptr(struct ofi_rbmap *map,
		struct ofi_rbnode *old_node, struct ofi_rbnode *new_node)
{
	if (new_node == old_node)
		return;

	*new_node = *old_node;

	if (!old_node->parent)
		map->root = new_node;
	else if (old_node == old_node->parent->left)
		old_node->parent->left = new_node;
	else
		old_node->parent->right = new_node;

	if (old_node->left != &map->sentinel)
		old_node->left->parent = new_node;
	if (old_node->right != &map->sentinel)
		old_node->right->parent = new_node;
}

void ofi_rbmap_delete(struct ofi_rbmap *map, struct ofi_rbnode *node)
{
	struct ofi_rbnode *x, *y;

	if (node->left == &map->sentinel) {
		y = node;
		x = y->right;
	} else if (node->right == &map->sentinel) {
		y = node;
		x = y->left;
	} else {
		y = node->right;
		while (y->left != &map->sentinel)
			y = y->left;
		x = y->right;
	}

	x->parent = y->parent;
	if (y->parent) {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	} else {
		map->root = x;
	}

	if (y != node)
		node->data = y->data;

	if (y->color == BLACK)
		ofi_delete_rebalance(map, x);

	/* swap y in for node, so we can free node */
	ofi_rbmap_replace_node_ptr(map, node, y);
	ofi_rbnode_free(map, node);
}

struct ofi_rbnode *ofi_rbmap_find(struct ofi_rbmap *map, void *key)
{
	struct ofi_rbnode *node;
	int ret;

	node = map->root;
	while (node != &map->sentinel) {
		ret = map->compare(map, key, node->data);
		if (ret == 0)
			return node;

		node = (ret < 0) ? node->left : node->right;
	}
	return NULL;
}

int ofi_rbmap_find_delete(struct ofi_rbmap *map, void *key)
{
	struct ofi_rbnode *node;

	node = ofi_rbmap_find(map, key);
	if (!node)
		return -FI_ENODATA;

	ofi_rbmap_delete(map, node);
	return 0;
}

struct ofi_rbnode *ofi_rbmap_search(struct ofi_rbmap *map, void *key,
		int (*compare)(struct ofi_rbmap *map, void *key, void *data))
{
	struct ofi_rbnode *node;
	int ret;

	node = map->root;
	while (node != &map->sentinel) {
		ret = compare(map, key, node->data);
		if (ret == 0)
			return node;

		node = (ret < 0) ? node->left : node->right;
	}
	return NULL;
}
