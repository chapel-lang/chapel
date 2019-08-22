class C {
  var x: int;

  proc init() {
    writeln("Initializing C");
  }

  proc deinit() {
    writeln("Deinitializing C");
  }
}

proc foo(c: C = new C()) {
  writeln(c);
}

proc bar(c = new C()) {
  writeln(c);
}

proc foo_borrows(c: borrowed C = new C()) {
  writeln(c);
}

proc foo_in(in c: C = new C()) {
  writeln(c);
}

proc bar_in(in c = new C()) {
  writeln(c);
}

{
  writeln("Default foo");
  foo();
}

{
  writeln("Supplied foo");
  foo(new C());
}

{
  writeln("Default bar");
  bar();
}

{
  writeln("Supplied bar");
  bar(new C());
}

{
  writeln("Default foo_borrows");
  foo_borrows();
}

{
  writeln("Supplied foo_borrows");
  foo_borrows(new C());
}

{
  writeln("Default foo_in");
  foo_in();
}

{
  writeln("Supplied foo_in");
  foo_in(new C());
}

{
  writeln("Default bar_in");
  bar_in();
}

{
  writeln("Supplied bar_in");
  bar_in(new C());
}
