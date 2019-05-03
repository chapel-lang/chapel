class C {
  var x: int;
}

proc foo(c: C = new C()) {
  writeln(c);
}

proc bar(c = new C()) {
  writeln(c);
}

writeln("Supplied bar");
bar(new C());
