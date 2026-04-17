use DistributedDeque;

config const hi = 32;

proc test0() {
  const D = {0..#hi};;
  var deque = new DistDeque(int);

  forall i in D {
    if i % 2 == 0 then deque.pushFront(i);
    else deque.pushBack(i);
  }

  var A: [D] bool;
  for i in deque.these(Ordering.LIFO) do A[i] = true;

  for x in A do write(if x then 'T' else 'F');
  writeln();
}
test0();
