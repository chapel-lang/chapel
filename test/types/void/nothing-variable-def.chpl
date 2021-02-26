var x:nothing;

class y {
  var z: nothing;
}

config param enabled = false;

proc vartype() type {
  if enabled then
    return int;
  else
    return nothing;
}

class c {
  var v: vartype();
  proc init() { this.complete(); if enabled then v = 5; }
}

var foo = new unmanaged c();
writeln(foo);
delete foo;
