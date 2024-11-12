class C {
  var x: int;
}

record R {
  var c: owned C?;  // unmanaged works

  proc init() { }

  proc init(val: int) {
    this.c = new C(val);  // unmanaged works
  }
}

var r = for i in 0..2 do new R(i);
compilerWarning("elt.type: ", r.eltType : string);
writeln(r);

