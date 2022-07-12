var r = 1..10;

proc testit(type t) {
  writeln("testing ", t:string);
  const count = 3: t;
  writeln(r#count);
}

for param i in 0..3 {
  testit(int(8 * 2**i));
  testit(uint(8 * 2**i));
}

writeln(r#11);

