def foo_int_v	(x : volatile int) { writeln("x = ", x); }
def foo_uint_v	(x : volatile uint) { writeln("x = ", x); }
def foo_real_v	(x : volatile real) { writeln("x = ", x); }
def foo_int	(x : int) { writeln("x = ", x); }
def foo_uint	(x : uint) { writeln("x = ", x); }
def foo_real	(x : real) { writeln("x = ", x); }


def main() {
  var x : volatile int = 1;
  var y : volatile uint = 2;
  var z : volatile real = 3.14;

  // formal is volatile and actual is volatile.  base type is same.
  foo_int_v(x);
  foo_uint_v(y);
  foo_real_v(z);

  // formal is volatile and actual is non-volatile. base type is same.
  foo_int_v(1);
  foo_uint_v(2);
  foo_real_v(3.14);

  // formal is non-volatile and actual is volatile. base type is same.
  foo_int(x);
  foo_uint(y);
  foo_real(z);
}
