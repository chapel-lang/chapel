class MyClass {
  var x:int;
}

proc main() {
  var x = new owned MyClass();
  var b = x.borrow();
  var u = b:unmanaged;
  var bb = u:borrowed;

  writeln(b.type:string, " ", b);
  writeln(u.type:string, " ", u);
  writeln(bb.type:string, " ", bb);
}

