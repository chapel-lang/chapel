/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*****************************************************************************

This code is based on list.h developed by John Plevyak and released as
part of his iterative flow analysis package available at SourceForge
(http://sourceforge.net/projects/ifa/).  The original code is:

Copyright (c) 1992-2006 John Plevyak.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/


/*
  Simple list classes including ones using internal links and ones using external cons cells.

  NOTE: SLL/DLL/Queue do not support use with certain types of multiple-inheritance.
    if you get from g++: 
      warning: invalid access to non-static data member `A::link' of NULL object
      warning: (perhaps the `offsetof' macro was used incorrectly)
    use -Wno-invalid-offsetof with version 3.3+ of GCC
*/

#ifndef _list_H_
#define _list_H_

#include <cstddef>

#include <stdint.h>

template <class C> class SLink {
 public:
  C *next;
  SLink() : next(NULL) {};
};

#define GetSLinkNext(_c, _e, _o) (((SLink<_c> *)(void*)(((intptr_t)(void*)_e) + _o))->next)

template <class C> struct Link : SLink<C> {
  C *prev;
  Link() : prev(NULL) {}
};

#define GetLinkNext(_c, _e, _o) (((Link<_c> *)(void*)(((intptr_t)(void*)_e) + _o))->next)
#define GetLinkPrev(_c, _e, _o) (((Link<_c> *)(void*)(((intptr_t)(void*)_e) + _o))->prev)

template <class C, int o> class SLL {
 public:
  C *head;
  inline void push(C *e);
  inline C *pop();
  void clear() { head = NULL; }
  C *next_link(C *e) { return GetLinkNext(C, e, o); }

  SLL() : head(NULL) {}
};

#define SList(_c, _f)  SLL<_c, offsetof(_c, _f)>
#define forl_LL(_c, _p, _l) for (_c *_p = (_l).head; _p; _p = (_l).next_link(_p))

template <class C, int o> class DLL {
 public:
  C *head;
  inline void push(C *e);
  inline C *pop();
  inline void remove(C *e);
  inline void insert(C *e, C *after);
  bool in(C *e);
  void clear() { head = NULL; }
  C *next_link(C *e) { return GetLinkNext(C, e, o); }
  C *prev_link(C *e) { return GetLinkPrev(C, e, o); }

  DLL() : head(NULL) {}
};

#define DList(_c, _f)  DLL<_c, offsetof(_c, _f)>

template <class C, int o> class Queue : public DLL<C, o> {
 public:
  using DLL<C, o>::head;
  C *tail;
  inline void push(C *e);
  inline C *pop();
  inline void enqueue(C *e);
  inline C *dequeue();
  inline void remove(C *e);
  inline void insert(C *e, C *after);
  inline void append(Queue<C, o> q);
  bool in(C *e);
  void clear() { head = NULL; tail = NULL; }
  
  Queue() : tail(NULL) {}
};

#define Que(_c, _f) Queue<_c, offsetof(_c, _f)>

template <class C, int o>
class CountQueue : public Queue<C, o> {
 public:
  int size; 
  inline CountQueue(void) : size(0) {}
  inline void push(C *e);
  inline C *pop();
  inline void enqueue(C *e);
  inline C *dequeue();
  inline void remove(C *e);
  inline void insert(C *e, C *after);
  inline void append(CountQueue<C, o> &q);
  inline void append_clear(CountQueue<C, o> &q);
};

#define CountQue(_c, _f) CountQueue<_c, offsetof(_c, _f)>

template <class C>
class ConsCell {
 public:
  C             car;
  ConsCell      *cdr;
  ConsCell(C acar, ConsCell *acdr) : car(acar), cdr(acdr) {}
  ConsCell(C acar) : car(acar), cdr(NULL) {}
  ConsCell(ConsCell *acdr) : cdr(acdr) {}
};

template <class C>
class List {
 public:
  ConsCell<C> *head;
  C first() { if (head) return head->car; else return 0; }
  C car() { return first(); }
  ConsCell<C> *rest() { if (head) return head->cdr; else return 0; }
  ConsCell<C> *cdr() { return rest(); }
  void push(C a) { head = new ConsCell<C>(a, head); }
  void push() { head = new ConsCell<C>(head); }
  C pop() { C a = car(); head = cdr(); return a; }
  void clear() { head = NULL; }
  void reverse();
  List(C acar) : head(new ConsCell<C>(acar)) {}
  List(C a, C b) : head(new ConsCell<C>(a, new ConsCell<C>(b))) {}
  List(C a, C b, C c) : head(new ConsCell<C>(a, new ConsCell<C>(b, new ConsCell<C>(c)))) {}
  List() : head(0) {}
};
#define forc_List(_c, _p, _l) if ((_l).head) for (ConsCell<_c> *_p  = (_l).head; _p; _p = _p->cdr)


/* IMPLEMENTATION */

template <class C, int o> inline void 
SLL<C, o>::push(C *e) {
  GetSLinkNext(C, e, o) = head;
  head = e;
}

template <class C, int o> inline C *
SLL<C, o>::pop() {
  C *ret = head;
  if (head)
    head = GetSLinkNext(C, head, o);
  return ret;
}

template <class C, int o> inline void 
DLL<C, o>::push(C *e) {
  if (head)
    GetLinkPrev(C, head, o) = e;
  GetLinkNext(C, e, o) = head;
  head = e;
}

template <class C, int o> inline C *
DLL<C, o>::pop() {
  C *ret = head;
  if (ret) {
    head = GetLinkNext(C, ret, o);
    if (head)
      GetLinkPrev(C, head, o) = NULL;
    GetLinkNext(C, ret, o) = NULL;
    return ret;
  } else
    return NULL;
}

template <class C, int o> inline void
DLL<C, o>::remove(C *e) {
  if (!head) return;
  if (e == head) head = GetLinkNext(C, e, o);
  if (GetLinkPrev(C, e, o)) GetLinkNext(C, GetLinkPrev(C, e, o), o) = GetLinkNext(C, e, o);
  if (GetLinkNext(C, e, o)) GetLinkPrev(C, GetLinkNext(C, e, o), o) = GetLinkPrev(C, e, o);
  GetLinkPrev(C, e, o) = NULL;
  GetLinkNext(C, e, o) = NULL;
}

template <class C, int o> inline void
DLL<C, o>::insert(C *e, C *after) {
  if (!after) { push(e); return; }
  GetLinkPrev(C, e, o) = after; 
  GetLinkNext(C, e, o) = GetLinkNext(C, after, o);
  GetLinkNext(C, after, o) = e;
  if (GetLinkNext(C, e, o)) GetLinkPrev(C, GetLinkNext(C, e, o), o) = e;
}

template <class C, int o> inline void 
Queue<C, o>::push(C *e) {
  DLL<C, o>::push(e);
  if (!tail) tail = head;
}

template <class C, int o> inline C *
Queue<C, o>::pop() {
  C *ret = DLL<C, o>::pop();
  if (!head) tail = NULL;
  return ret;
}

template <class C, int o> inline void
Queue<C, o>::insert(C *e, C *after) {
  DLL<C, o>::insert(e, after);
  if (!tail)
    tail = head;
  else if (tail == after)
    tail = e;
}

template <class C, int o> inline void
Queue<C, o>::remove(C *e) {
  if (tail == e)
    tail = GetLinkPrev(C, e, o);
  DLL<C, o>::remove(e);
}

template <class C, int o> inline void
Queue<C, o>::append(Queue<C, o> q) {
  if (!head) {
    head = q.head;
    tail = q.tail;
  } else {
    if (q.head) {
      GetLinkNext(C, tail, o) = q.head;
      GetLinkPrev(C, q.head, o) = tail;
      tail = q.tail;
    }
  }
}

template <class C, int o> inline void 
Queue<C, o>::enqueue(C *e) {
  if (tail)
    insert(e, tail);
  else
    push(e);
}

template <class C, int o> inline C *
Queue<C, o>::dequeue() {
  return pop();
}

template <class C, int o> inline void 
CountQueue<C, o>::push(C *e) {
  Queue<C, o>::push(e);
  size++;
}

template <class C, int o> inline C *
CountQueue<C, o>::pop() {
  C *ret = Queue<C, o>::pop();
  if (ret)
    size--;
  return ret;
}

template <class C, int o> inline void 
CountQueue<C, o>::remove(C *e) {
  Queue<C, o>::remove(e);
  size--;
}

template <class C, int o> inline void 
CountQueue<C, o>::enqueue(C *e) {
  Queue<C, o>::enqueue(e);
  size++;
}

template <class C, int o> inline C *
CountQueue<C, o>::dequeue() {
  return pop();
}

template <class C, int o> inline void
CountQueue<C, o>::insert(C *e, C *after) {
  Queue<C, o>::insert(e, after);
  size++;
}

template <class C, int o> inline void
CountQueue<C, o>::append(CountQueue<C, o> &q) {
  Queue<C, o>::append(q);
  size += q.size;
}

template <class C, int o> inline void
CountQueue<C, o>::append_clear(CountQueue<C, o> &q) {
  append(q);
  q.head = q.tail = 0;
  q.size = 0;
}

template <class C> void
List<C>::reverse() {
  ConsCell<C> *n, *t;
  for (ConsCell<C> *p = head; p; p = n) {
    n = p->cdr;
    p->cdr = t;
    t = p;
  }
  head = t;
}

void test_list();

#endif
