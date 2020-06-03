pragma "safe"
module tz {



class MyClass {
  var x:int;
}

private const nilTZ : shared MyClass?;

record R {
  var tz:shared MyClass?;
  proc init() {
    this.tz = nilTZ;
  }
  proc init(in tz:shared MyClass?) {
    this.tz = tz;
  }
}

proc makeNilR() {
  return new R(nilTZ);
}

proc R.replace(in tzinfo:shared MyClass? = this.tz) {
  return new R(tzinfo);
}

proc type R.now(in tz:shared MyClass? = nilTZ) {
  if tz.borrow() == nil {
    return new R();
  } else {
    const x = makeNilR();
    return x.replace(tzinfo=tz);
  }
}



proc test() {
  var c = new shared MyClass(1);
  var x = R.now();
  var y = R.now(c);

  writeln(x.tz.borrow());
  writeln(y.tz.borrow());
}

test();
}
