class C {
  var x: int;

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

// Shouldn't this work as well?
{
  //  writeln("Supplied bar");
  //  bar(new C());
}