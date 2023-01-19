// inspired by slice-align-bug-1.chpl

use driver;

config const n = n1;
const Dom1D: domain(1) dmapped Dist1D = Space1;

var a: [Dom1D] int;
var b: [Dom1D] int = Space1;
var c: [1..n/2] int = 1..n/2;

proc reset() { a = 100+1..100+n; }

proc main {
  writeln("=========== a[1..n by 2 align 1] = c ===========");
  reset();
  a[1..n by 2 align 1] = c;
  writeln(a);

  writeln("=========== a[1..n by -2 align 1] = c ===========");
  reset();
  a[1..n by -2 align 1] = c;
  writeln(a);

  writeln("=========== a[1..n by 2] = b[1..n-1 by 2] ===========");
  reset();
  a[1..n by 2] = b[1..n-1 by 2];
  writeln(a);

  writeln("=========== a[1..n by -2] = b[1..n-1 by -2] ===========");
  reset();
  a[1..n by -2] = b[1..n-1 by -2];
  writeln(a);

  writeln("=========== a[1..n by 2] = b[1..n-1 by -2] ===========");
  reset();
  a[1..n by 2] = b[1..n-1 by -2];
  writeln(a);

  writeln("=========== a[1..n by -2] = b[1..n-1 by 2] ===========");
  reset();
  a[1..n by -2] = b[1..n-1 by 2];
  writeln(a);

  writeln("=========== done ===========");
}

/* The above outputs can also be produced by the following:
const n2 = n/2;
for i in 1..n2 do write(i, " ", 100+2*i, " ");  writeln();
for i in 1..n2 do write(n2+1-i, " ", 100+2*i, " ");  writeln();
for i in 1..n2 do write(2*i-1, " ", 100+2*i, " ");  writeln();
for i in 1..n2 do write(100+2*i-1, " ", 2*i-1, " ");  writeln();
for i in 1..n2 do write(n+1 - i*2, " ", 100+2*i, " ");  writeln();
for i in 1..n2 do write(100+2*i-1, " ", n+1 - i*2, " ");  writeln();
*/
