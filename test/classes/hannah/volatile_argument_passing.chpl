
proc foo_bool_v	(b  : volatile bool) { writeln("b = ", b); }
proc foo_int_v	(i  : volatile int) { writeln("i = ", i); }
proc foo_uint_v	(ui : volatile uint) { writeln("ui = ", ui); }
proc foo_real_v	(r  : volatile real) { writeln("r = ", r); }
proc foo_bool	(b  : bool) { writeln("b = ", b); }
proc foo_int	(i  : int) { writeln("i = ", i); }
proc foo_uint	(ui : uint) { writeln("ui = ", ui); }
proc foo_real	(r  : real) { writeln("r = ", r); }


proc main() {
  var w : volatile bool = false;
  var x : volatile int = 1;
  var y : volatile uint = 2;
  var z : volatile real = 3.14;

  // formal is volatile and actual is volatile.  base type is same.
  foo_bool_v(w);
  foo_int_v(x);
  foo_uint_v(y);
  foo_real_v(z);

  // formal is volatile and actual is non-volatile. base type is same.
  foo_bool_v(false);
  foo_int_v(1);
  foo_uint_v(2);
  foo_real_v(3.14);

  // formal is non-volatile and actual is volatile. base type is same.
  foo_bool(w);
  foo_int(x);
  foo_uint(y);
  foo_real(z);
}
