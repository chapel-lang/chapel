class C {
  var a: int;
  proc init(n: int) {
    a = n;
    writeln("construct C");
  }
}

proc foo(x) type {
  var nnObj = new C(x); var nn = nnObj.borrow();
  for i in 1..x do
    writeln(i);
  return nn.type;
}

proc main {
  var a: foo(3)?;
  writeln(a.type:string);
}
