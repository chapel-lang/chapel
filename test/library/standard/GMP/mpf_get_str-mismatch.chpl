use CTypes, GMP;

extern proc printf(args...);

var x: mpf_t;
mpf_init(x);

mpf_set_d(x, 0.123456789);

{ // try using a smaller 'exp' value :S
  var a: c_array(c_char, 10);
  var s = c_ptrTo(a[0]);
  var exp = 1: int(32);
  s = mpf_get_str(s, exp, 10, 9, x);
  printf("%s\n", s);
  writeln("exp = ", exp);
}

mpf_clear(x);
