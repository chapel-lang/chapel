
record foo {
  function f() { return 1; }
}
function f(x : foo) { return 2; }

{
  function f(x : foo) { return 3; }
  {
    function f(x : foo) { return 4; }
  }
  var x = foo();
  writeln(x.f());
  writeln(f(x));
}
