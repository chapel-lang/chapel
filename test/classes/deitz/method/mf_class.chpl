class foo {
  var x : int;
  proc bar() {
    writeln("method bar ", x);
  }
}

proc bar(f : borrowed foo) {
  writeln("function bar ", f.x);
}

var f : unmanaged foo = new unmanaged foo();

f.bar();
bar(f);

delete f;
