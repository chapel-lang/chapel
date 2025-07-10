//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use DistributedDeque;

const maxElem = 1024;
const ref ourLocales = Locales;
const N = 2048;

proc test0() {
  /* START_EXAMPLE_0 */
  var deque = new DistDeque(int, cap=maxElem, targetLocales=ourLocales);
  /* STOP_EXAMPLE_0 */

  /* START_EXAMPLE_1 */
  deque.enqueue(1);
  var (hasElem, elem) = deque.dequeue();
  /* STOP_EXAMPLE_1 */
}

proc test1() {
  var deque = new DistDeque(int, cap=maxElem, targetLocales=ourLocales);

  /* START_EXAMPLE_2 */
  deque.push(1);
  var (hasElem, elem) = deque.pop();
  /* STOP_EXAMPLE_2 */
}

proc test2() {
  /* START_EXAMPLE_3 */
  var deque = new DistDeque(int);
  forall i in 1..N {
    if i % 2 == 0 then deque.pushFront(i);
    else deque.pushBack(i);
  }
  /* STOP_EXAMPLE_3 */

  /* START_EXAMPLE_4 */
  for elt in deque.these(Ordering.FIFO) {
    // ...
  }
  /* STOP_EXAMPLE_4 */

  // TODO: DistDeque seems to halt when iterating with LIFO ordering.
  //       When #27505 is fixed this can be moved back into example 4.
  /*
  for elt in deque.these(Ordering.LIFO) {
    // ...
  }
  */

  /* START_EXAMPLE_5 */
  deque.addBulk(1..100);
  var result = + reduce deque;
  /* STOP_EXAMPLE_5 */

  // TODO: Shouldn't this hold?
  // assert(result == +reduce (1..100));
}

proc main() {
  test0();
  test1();
  test2();
}
