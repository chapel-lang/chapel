def baz() {
  var y = 2.0;
  def foo() {
   var x = 1;
   def bar() {
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
