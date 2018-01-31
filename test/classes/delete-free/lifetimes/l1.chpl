pragma "safe"
module l1 {

proc identity(ref x:int) ref return x;

proc bad() ref {
  var x:int;
  return identity(x);
}

proc badder(inout x:int) ref {
  return identity(x);
}

record R {
  var x:int;
  proc get() ref { return x; }
}

proc baddest() ref {
  var r:R;
  return r.get();
}

proc nested_bad() ref {
  var x: int;
  proc inner() ref {
    return x;
  }
  return inner();
}

var global = 1;
proc ok1() ref {
  return global;
}

proc ok2() {
  var x:int; 
  return x;
}

proc value_identity(x) { return x; }

proc ok3() {
  var x:int; 
  return value_identity(x);
}

proc ok4() {
  var x:int; 
  return identity(x);
}

proc test() {
  ref a = bad();
  var tmpInt = 1;
  ref b = badder(tmpInt);
  ref c = baddest();
  ref d = nested_bad();

  ref e = ok1();
  var f = ok2();
  var g = ok3();
  var h = ok4();
}

test();

}
