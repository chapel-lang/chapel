class foo {
  var x : int;
  fun bar() {
    writeln("method bar ", x);
  }
}

fun bar(f : foo) {
  writeln("function bar ", f.x);
}

var f : foo = foo();

f.bar();
bar(f);
