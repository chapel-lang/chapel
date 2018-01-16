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
proc ok() ref {
  return global;
}

pragma "safe"
proc test() {
  ref a = bad();
  var tmpInt = 1;
  ref b = badder(tmpInt);
  ref c = baddest();
  ref d = nested_bad();

  ref e = ok();
}

test();
