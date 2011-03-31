var x: int;

proc foo() {
  atomic x += 1;
}

atomic foo();

writeln(x);
