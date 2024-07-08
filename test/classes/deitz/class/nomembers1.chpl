class addition {
  proc identity(x : int) : int {
    return x + 2;
  }
}

var aOwn = new owned addition();
var a : borrowed addition = aOwn.borrow();

writeln(a.identity(2));
