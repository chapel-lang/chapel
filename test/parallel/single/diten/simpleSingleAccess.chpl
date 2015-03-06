use Time;

var a: single bool;

proc foo() {
  writeln("Going to sleep for a bit.");
  sleep(2);
  writeln("That was a nice nap. I'll do my work now.");
  a = true;
}

proc bar() {
  a; // wait for single var "a" to get set;
  writeln("'a' got set. It is: ", a.readFF());
}

cobegin {
  bar();
  foo();
}
