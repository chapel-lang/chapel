record Impl {
  proc returnstype() type {
    return int;
  }
}
record R {
  forwarding var x:Impl;
}

proc test() {
  var r = new R(new Impl());
  type t = r.returnstype();
  writeln(t:string);
}

test();
