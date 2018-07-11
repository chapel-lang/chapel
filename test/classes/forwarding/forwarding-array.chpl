record Foo {
  var data: [1..2] int;
  forwarding data;
  proc chpl__promotionType() type {
    return int;
  }
}

var foo = new Foo();
