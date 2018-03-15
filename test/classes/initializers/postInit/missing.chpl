
// Parent does not have postInit, but child does

class Y {
  var y : int;

  proc init() {
    writeln("Y.init");
  }
}

class Z : Y {
  var z : int;

  proc init() {
    writeln("Z.init");
  }

  proc postInit() {
    writeln("Z.postInit");
  }
}

var z = new Z();
delete z;
writeln();

// Parent does not have postInit, but child and grandparent do

class A {
  var a : int;

  proc init() {
    writeln("A.init");
  }

  proc postInit() {
    writeln("A.postInit");
  }
}

class B : A {
  var b : int;

  proc init() {
    writeln("B.init");
  }
}

class C : B {
  var c : int;

  proc init() {
    writeln("C.init");
  }

  proc postInit() {
    writeln("C.postInit");
  }
}

var c = new C();
delete c;
writeln();

// Parent has postInit, but child does not

class Parent {
  var p : int;

  proc init() {
    writeln("Parent.init");
  }

  proc postInit() {
    writeln("Parent.postInit");
  }
}

class Child : Parent {
  var c : int;

  proc init() {
    writeln("Child.init");
  }
}

var child = new Child();
delete child;
