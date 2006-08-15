def baz() {
  var y = 2.0;
  def foo() {
   var x = 1;
   def bar() {
     writeln(x);
     writeln(y);
   }
   bar();
  }
  foo();
}
baz();
