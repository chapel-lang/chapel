var a: single bool;

proc foo() {
  writeln("Going to sleep for a bit.");
  // no need to actually sleep() here
  writeln("That was a nice nap. I'll do my work now.");
  a.writeEF(true);
}

proc bar() {
  a.readFF(); // wait for single var "a" to get set;
  writeln("'a' got set. It is: ", a.readFF());
}

cobegin {
  bar();
  foo();
}
