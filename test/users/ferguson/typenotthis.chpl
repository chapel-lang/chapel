record R {
}

proc R.do_something_helper() {
  writeln("in do something helper");
}

proc R.do_something() {
  writeln("in do something");
  R.do_something_helper();
}

var r:R;

r.do_something();

