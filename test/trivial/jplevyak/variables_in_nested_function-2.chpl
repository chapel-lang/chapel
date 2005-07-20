function baz() {
  var y = 2.0;
  function foo() {
   var x = 1;
   function bar() {
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
