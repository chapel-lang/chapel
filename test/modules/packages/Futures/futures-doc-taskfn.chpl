use Futures;

config const X = 99;

proc foo(x: int) {
  return (x:real)/3.1415926;
}

record Bar {
  proc this(x: real) {
    return x / 2;
  }
  proc retType type return real;
}
const bar: Bar;

const A = async(foo, X);
const B = A.andThen(bar);
writeln(B.get());
