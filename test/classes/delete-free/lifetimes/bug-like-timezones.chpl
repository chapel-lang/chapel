pragma "safe"
module tz {

use SharedObject;

class MyClass {
  var x:int;
}

private const nilTZ = new Shared(MyClass);

record R {
  var tz:Shared(MyClass);
  proc init() {
    this.tz = nilTZ;
  }
  proc init(tz:Shared(MyClass)) {
    this.tz = tz;
  }
}

proc makeNilR() {
  return new R(nilTZ);
}

proc R.replace(tzinfo:Shared(MyClass) = this.tz) {
  return new R(tzinfo);
}

proc type R.now(tz:Shared(MyClass) = nilTZ) {
  if tz.borrow() == nil {
    return new R();
  } else {
    const x = makeNilR();
    return x.replace(tzinfo=tz);
  }
}



proc test() {
  var c = new Shared(new MyClass(1));
  var x = R.now();
  var y = R.now(c);

  writeln(x.tz.borrow());
  writeln(y.tz.borrow());
}

test();
}
