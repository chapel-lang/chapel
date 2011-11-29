proc foo() {
  var s = "aaa";
  for i in 1..100 {
    s += "bbbbbb";
    s += "aaa";
  }
  return s;
}

var b = foo();

writeln(b);

var a: [1..8] string;

cobegin {
  a[1] = foo();
  a[2] = foo();
  a[3] = foo();
  a[4] = foo();
  a[5] = foo();
  a[6] = foo();
  a[7] = foo();
  a[8] = foo();
}

for i in 1..8 {
  if (a[i] != b) then
    writeln("FAILURE!!");
  writeln(a[i]);
}
