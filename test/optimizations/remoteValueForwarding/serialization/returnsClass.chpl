
class Bar {
  var y : int;
}

record Foo {
  var x : 3*int;

  proc chpl__serialize() {
    return new unmanaged Bar(5);
  }

  proc type chpl__deserialize(data) {
    var f : Foo;
    f.x(1) = data.y;
    return f;
  }
}

proc main() {
  const f = new Foo();

  on Locales.tail() {
    if f.x(1) == 100 then halt();
  }
}
