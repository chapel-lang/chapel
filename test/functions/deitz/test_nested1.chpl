proc foo() {
  var x = 1;
  proc bar() {
    var y = 2;
    writeln(x);
    writeln(y);
  }
  bar();
}

foo();
