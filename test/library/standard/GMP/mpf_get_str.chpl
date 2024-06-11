use CTypes, GMP;

extern proc printf(args...);

var x: mpf_t;
mpf_init(x);

mpf_set_d(x, 0.123456789);

{ // simple test from @s-timcheck
  var s: c_ptr(c_char);
  var exp = 1;
  s = mpf_get_str(s, exp, 10, 4, x);
  printf("%s\n", s);
  writeln("exp = ", exp);
  deallocate(s);
}

mpf_set_d(x, 123.456789);

{ // try a different decimal location
  var s: c_ptr(c_char);
  var exp = 1;
  s = mpf_get_str(s, exp, 10, 4, x);
  printf("%s\n", s);
  writeln("exp = ", exp);
  deallocate(s);
}

mpf_set_d(x, 3.1415926535);

{ // try example from GMP docs
  var s: c_ptr(c_char);
  var exp = 1;
  s = mpf_get_str(s, exp, 10, 5, x);
  printf("%s\n", s);
  writeln("exp = ", exp);
  deallocate(s);
}


{ // try using a pre-allocated 'char' buffer
  var a: c_array(c_char, 10);
  var s = c_ptrTo(a[0]);
  var exp = 1;
  s = mpf_get_str(s, exp, 10, 9, x);
  printf("%s\n", s);
  writeln("exp = ", exp);
}

mpf_clear(x);
