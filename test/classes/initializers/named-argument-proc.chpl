proc helper(bar = 5) {
  return bar;
}

class Foo {
  var B = helper(bar=10);

  proc init() {
  }
}
