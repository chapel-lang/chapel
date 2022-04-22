/*
This is a sketch implementation of support for eurekas.
It is currently overly-simplistic for readability.

Here is a typical use of this feature:

    use Eureka;
    var e: eureka;

    forall ..... {
      e.check(); // this could be inserted automatically by the compiler
      doWork();
      if checkForEureka() then
        e.notify()
    }

Within the loop body 'e' carries a task-private flag.

'e.notify()' sets this flag for all tasks created by the forall loop,
then (or concurrently) throws a EurekaNotified error.

'e.check()' checks if the current task's flag has been set.
If so, it throws a EurekaChecked error.
Otherwise, it is a cheap operation -- read and branch on an atomic bool.

Using this feature in a task-parallel construct requires an explicit
in intent, due to a shortcoming of the current implementation.
For example, the above code snippet could be switched to a coforall loop
by replacing `forall .....` with:

    coforall ..... with (in e)
*/

private use List;

/* thrown by notify() */
class EurekaNotified : Error {
}

/* thrown by check() if appropriate */
class EurekaChecked  : Error {
}

pragma "default task intent in"
record eureka {
  forwarding const e: owned EurekaImpl;
  proc init()               { e = new EurekaImpl();        }
  proc init=(other: eureka) { e = new EurekaImpl(other.e); }
}

pragma "no doc"
/*private*/ class EurekaImpl {
  var   notified: atomic bool;
  const parent:   unmanaged EurekaImpl?;
  var   children: list(unmanaged EurekaImpl);

  proc init() {
  }
  proc init(parent: borrowed EurekaImpl) {
    this.parent = parent: unmanaged;
    complete();
    // todo: do not append if this task has been _setNotified
    parent.children.append(this: unmanaged);
  }
  proc deinit() {
    // todo: ensure that _setNotified() will not be invoked on this instance,
    // esp. through parent.children, once it is deinitialized
  }

  proc check() throws {
    if notified.read() then
      throw new EurekaChecked();
  }
  proc notify() throws {
    _setNotified();
    throw new EurekaNotified();
  }
  /*private*/ proc _setNotified() {
    const success = notified.compareAndSwap(false, true);
    if !success then
      return; // has already been _setNotified; do not repeat the actions below

    if const parentNN = parent then
      parentNN._setNotified(); // todo: 'parent' not to _setNotified this child
    for child in children do
      child._setNotified(); // todo: 'child' not to _setNotified its parent
  }
}

operator =(ref lhs: eureka, rhs: eureka) {
  compilerError("assignment between eurekas is not allowed");
}
