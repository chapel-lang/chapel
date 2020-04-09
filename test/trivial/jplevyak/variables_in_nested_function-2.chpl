proc baz() {
  var y = 2.0;
  proc foo() {
   var x = 1;
   proc bar() {
     x = x + 1;
     y = y + 1;
     writeln(x);
     writeln(y);
   }
   bar();
   writeln(x);
   writeln(y);
  }
  foo();
}
baz();
