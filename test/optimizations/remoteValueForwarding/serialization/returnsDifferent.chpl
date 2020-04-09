
record Foo {
  var x : 3*int;

  proc chpl__serialize() {
    return 1;
  }

  proc type chpl__deserialize(data) {
    return data;
  }
}
