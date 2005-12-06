var a = 2;
var b = 3;
var c = 5;
function foo() {
  var x = a * b * c;
  function bar() {
    var y = a * b;
    return x * y;
  }
  return bar();
}
writeln(foo());
