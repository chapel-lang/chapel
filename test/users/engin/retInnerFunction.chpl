proc foo() {
  var dummy = 10;
  return lambda (x:int) { return x*dummy; };
}
writeln(foo()(5));
