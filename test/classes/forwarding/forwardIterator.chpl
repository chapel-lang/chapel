record Foo {
  var b: shared Bar?;
  forwarding b!;

  proc init(num: int) {
    b = new shared Bar(num);
  }
}

class Bar {
  var r: range;

  proc init(num: int) {
    r = 0..#num;
  }

  iter all() {
    for i in r {
      yield i;
    }
  }
}

proc main() {
  var f = new Foo(10);

  for i in f.all() {
    writeln(i);
  }
}
