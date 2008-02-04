record foo {
  var x : int;
  def bar() {
    writeln("method bar ", x);
  }
}

def bar(f : foo) {
  writeln("function bar ", f.x);
}

var f : foo = new foo();

f.bar();
bar(f);
