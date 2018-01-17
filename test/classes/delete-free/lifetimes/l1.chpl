pragma "safe"
proc identity(ref x:int) ref return x;

pragma "safe"
proc bad() ref {
  var x:int;
  return identity(x);
}

pragma "safe"
proc badder(inout x:int) ref {
  return identity(x);
}

record R {
  var x:int;
  pragma "safe"
  proc get() ref { return x; }
}

pragma "safe"
proc baddest() ref {
  var r:R;
  return r.get();
}

pragma "safe"
proc nested_bad() ref {
  var x: int;
  pragma "safe"
  proc inner() ref {
    return x;
  }
  return inner();
}

var global = 1;
pragma "safe"
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

pragma "safe"
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
