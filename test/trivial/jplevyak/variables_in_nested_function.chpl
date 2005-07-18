function baz() {
  var y = 2.0;
  function foo() {
   var x = 1;
   function bar() {
     writeln(x);
     writeln(y);
   }
   bar();
  }
  foo();
}
baz();
