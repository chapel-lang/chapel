record foo {
  var x : integer;
  function bar() {
    writeln("method bar ", x);
  }
}

function bar(f : foo) {
  writeln("function bar ", f.x);
}

var f : foo = foo();

f.bar();
bar(f);
