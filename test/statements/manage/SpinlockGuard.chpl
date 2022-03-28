// TODO: Change me to standard module when we create one.
use ChapelLocks;

record spinlock {
  type T;
  var _resource: T;
  var _lock: chpl_LocalSpinlock;

  proc init(type T) {
    this.T = T;
  }

  proc init(in resource: ?T) {
    this.T = T;
    this._resource = resource;
  }

  proc ref enterThis() ref: T {
    _lock.lock();
    return _resource;
  }

  proc ref leaveThis(in err: owned Error?) {
    defer _lock.unlock();
    if err then try! { throw err; }
  }

  proc const nonLockingRead() ref: T {
    return _resource;
  }
}

proc test1() {
  const r = 1..5;

  var sum1: atomic int = 0;
  forall i in r do sum1.add(i);

  var sum2 = new spinlock(int);
  forall i in r with (ref sum2) do
    manage sum2 as count do
      count += i;

  assert(sum1.read() == sum2.nonLockingRead());
}
test1();


