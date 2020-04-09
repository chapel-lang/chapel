
record R {
  var x;
}

proc R.foo() {
  writeln("in generic R.foo");
}

proc getR() type {
  return R(int);
}

proc (getR()).foo() {
  writeln("in specific R(int).foo");
}

proc R.foo()
  where this.type == R(string)
{
  writeln("in specific R(string).foo");
}

var x = new R(1.0);
x.foo();

var y = new R("test");
y.foo();

var z = new R(2);
z.foo();

