record R {
  var x: int;
}

proc R.init=(other: R) {
  writeln("In my init=");
  this.x = 2*other.x;
}

proc main() {
  var u3: R;
  u3.x = 3;
  var u4: R = u3;
  writeln(u3, " ", u4);
  var u5: R = u4;
  writeln(u5);
}
