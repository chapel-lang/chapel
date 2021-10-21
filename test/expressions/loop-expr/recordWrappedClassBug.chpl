class C {
  var x: int;
}

record R {
  var c: C?;  // unmanaged works

  proc init() { }

  proc init(val: int) {
    this.c = new C(val);  // unmanaged works
  }
}

var r = for i in 0..2 do new R(i);
writeln(r);

