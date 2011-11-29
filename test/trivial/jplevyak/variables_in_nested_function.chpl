proc baz() {
  var y = 2.0;
  proc foo() {
   var x = 1;
   proc bar() {
     writeln(x);
     writeln(y);
   }
   bar();
  }
  foo();
}
baz();
