use SpinlockGuard;

proc test1() {
  const r = 1..5;

  var sum1: atomic int = 0;
  forall i in r do sum1.add(i);

  var sum2 = new spinlock(int);

  // Should produce a compiler error because sum2 is const in the forall.
  forall i in r do
    manage sum2 as count do
      count += i;

  assert(sum1.read() == sum2.nonLockingRead());
}
test1();

