fun baz() {
  var y = 2.0;
  fun foo() {
   var x = 1;
   fun bar() {
     writeln(x);
     writeln(y);
   }
   bar();
  }
  foo();
}
baz();
