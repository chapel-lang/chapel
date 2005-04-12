/*
  Copyright 2003 John Plevyak
*/

#include <stddef.h>
#include "geysa.h"

#ifdef TEST_LIB
class A {
 public:
  int a;
  SLink<A> link;
  A(int aa) : a(aa) {}
};

class B {
 public:
  int b;
  Link<B> link;
  B(int bb) : b(bb) {}
};

void
test_list() {
  SList(A, link)  sll;
  DList(B, link)  dll;
  Que(B, link)    q, qq;
  int tt = 2 * 99 * 50, t = 0;

  // push/pop

  for (int i = 0; i < 100; i++)
    sll.push(new A(i));
  for (int i = 0; i < 100; i++)
    t += sll.pop()->a;
  for (int i = 0; i < 100; i++)
    dll.push(new B(i));
  for (int i = 0; i < 100; i++)
    t += dll.pop()->b;
  assert(t == tt);

  // insert/remove

  B *b[100]; 
  b[0] = new B(0);
  dll.push(b[0]);
  for (int i = 1; i < 100; i++)
    dll.insert((b[i] = new B(i)), b[0]);
  for (int i = 0; i < 100; i++)
    dll.remove(b[i]);
  assert(dll.head == NULL);

  // enqueue/dequeue

  t = 0;
  for (int i = 0; i < 100; i++)
    q.enqueue(b[i]);
  for (int i = 0; i < 100; i++)
    qq.enqueue(new B(i));
  q.append(qq);
  for (int i = 0; i < 200; i++)
    t += q.dequeue()->b;
  assert(t == tt);
  assert(q.head == NULL);
}
#endif

