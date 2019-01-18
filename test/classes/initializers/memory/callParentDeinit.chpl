
//
// Simple case to ensure all deinits in a hierarchy are called
//

class Parent {
  type idxType;

  proc deinit() {
    writeln("Parent.deinit");
  }
}

class Child : Parent {
  proc init(type t) {
    super.init(t);
  }
  proc deinit() {
    writeln("Child.deinit");
  }
}

var ch = new unmanaged Child(int);
delete ch;

writeln();

//
// More complex case that mimics some BaseDom hierarchy
//

class A {
  param rank : int;
  type idxType;
  proc deinit() {
    writeln("A.deinit");
  }
}

class B : A {
  var x : int;
  proc deinit() {
    writeln("B.deinit");
  }
}

class C : B {
  proc init(param rank : int, type idxType) {
    super.init(rank, idxType);
  }
  proc deinit() {
    writeln("C.deinit");
  }
}

var c = new unmanaged C(1, int);
delete c;
