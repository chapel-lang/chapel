
config param foo = false;

record Foo {
  var x : 3*int;

  proc chpl__serialize() {
    if foo {
      return 1;
    }
  }

  proc type chpl__deserialize(data) {
    var f : Foo;
    f.x(1) = data;
  }
}
