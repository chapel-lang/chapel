
function foo() {
  var x = 0;
  label lab1 writeln(1);
  x = x + 1;
  if (x < 5) then goto lab1;
}
foo();
