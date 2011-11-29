record foo {
  var x : int;
  proc bar() {
    writeln("method bar ", x);
  }
}

proc bar(f : foo) {
  writeln("function bar ", f.x);
}

var f : foo = new foo();

f.bar();
bar(f);
