record Foo {
  var newName: int;

  proc oldName ref: int {
    return this.newName;
  }

  pragma "last resort"
  deprecated "'new Foo(oldName=val)' is deprecated, please use 'new Foo(newName=val)' or 'new Foo(val)' instead"
  proc init(oldName: int) {
    this.newName = oldName;
  }

  // Required because previous initializer prevented the generation of the
  // default initializer
  // Could also write `newName: int = 0` to avoid writing two initializers
  proc init(newName: int) {
    this.newName = newName;
  }

  // Required because first initializer prevented the generation of the
  // default initializer
  // Relies on omitted field initialization
  proc init() {
  }
}

proc main() {
  var f = new Foo(oldName=30);
  writeln(f);
  var f2 = new Foo(newName=30); // Still works without warning
  writeln(f2);
  var f3 = new Foo(30); // Still works without warning
  writeln(f3);
  var f4 = new Foo(); // Still works without warning
  writeln(f4);
}
