/*
 * Copyright (c) 2015 Cray Inc. All rights reserved.
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

#ifndef RBTREE_H_
#define RBTREE_H_

typedef enum {
    RBT_STATUS_OK,
    RBT_STATUS_MEM_EXHAUSTED,
    RBT_STATUS_DUPLICATE_KEY,
    RBT_STATUS_KEY_NOT_FOUND
} RbtStatus;

typedef void *RbtIterator;
typedef void *RbtHandle;

RbtHandle rbtNew(int(*compare)(void *a, void *b));
// create red-black tree
// parameters:
//     compare  pointer to function that compares keys
//              return 0   if a == b
//              return < 0 if a < b
//              return > 0 if a > b
// returns:
//     handle   use handle in calls to rbt functions


void rbtDelete(RbtHandle h);
// destroy red-black tree

RbtStatus rbtInsert(RbtHandle h, void *key, void *value);
// insert key/value pair

RbtStatus rbtErase(RbtHandle h, RbtIterator i);
// delete node in tree associated with iterator
// this function does not free the key/value pointers

RbtIterator rbtNext(RbtHandle h, RbtIterator i);
// return ++i

RbtIterator rbtBegin(RbtHandle h);
// return pointer to first node

RbtIterator rbtEnd(RbtHandle h);
// return pointer to one past last node

void rbtKeyValue(RbtHandle h, RbtIterator i, void **key, void **value);
// returns key/value pair associated with iterator

void ** rbtValuePtr(RbtHandle h, RbtIterator it);
// returns pointer to the value associated with iterator

RbtIterator rbtFindLeftmost(RbtHandle h, void *key,
		int(*compare)(void *a, void *b));
// returns iterator associated with left-most match. This is useful when a new
//   key might invalidate the uniqueness property of the tree.

RbtIterator rbtFind(RbtHandle h, void *key);
// returns iterator associated with key

void rbtTraversal(RbtHandle h, RbtIterator it, void *handler_arg,
          void(*handler)(void *arg, RbtIterator it));
// tree traversal that visits (applies handler()) each node in the tree data
//   strucutre exactly once.

void *rbtRoot(RbtHandle h);
// returns the root of the tree

#endif /* RBTREE_H_ */
