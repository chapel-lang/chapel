class C {
  var x: int;
}

proc foo(c: C = new C()) {
  writeln(c.type:string);
  writeln(c);
}
writeln("Calling foo");
foo(new C());

proc bar(c = new C()) {
  writeln(c.type:string);
  writeln(c);
}

writeln("Calling bar");
bar(new C());

proc baz(c: borrowed C = new C()) {
  writeln(c.type:string);
  writeln(c);
}
writeln("Calling baz");
baz(new owned C());
