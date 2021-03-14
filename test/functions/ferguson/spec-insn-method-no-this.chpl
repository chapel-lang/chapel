record R {
  var x;
}

proc (R(int)).foo() {
  var tmp = this.x;
  writeln("in specific R(int).foo, x=", tmp);
}

proc (R(int)).bar() {
  var tmp = x; // as above, x refers to field, `this` is implicit
  writeln("in specific R(int).bar, x=", tmp);
}


var x = new R(1);
x.foo();
x.bar();
