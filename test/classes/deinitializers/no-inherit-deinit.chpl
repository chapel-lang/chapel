
//
// At one time, this test failed to compile because the compiler incorrectly
// thought that 'deinit' could be virtual. See #9587 for more info.
//

class Dummy {}

class A {
  var a: unmanaged Dummy;
  proc init() { this.a = new unmanaged Dummy(); }
  proc deinit() { delete this.a; }
}

class B : A {
  proc init() { super.init(); }
  //proc deinit() {} // Adding deinit will compile without warnings.
}

class C : B {
  proc init() { super.init(); }
  //proc deinit() {}
}

proc main() {
  var c = new unmanaged C();
  delete c;
  writeln("Hello!");
}
