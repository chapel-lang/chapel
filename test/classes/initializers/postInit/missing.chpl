
// Parent does not have postinit, but child does

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

  proc postinit() {
    writeln("Z.postinit");
  }
}

var z = new unmanaged Z();
delete z;
writeln();

// Parent does not have postinit, but child and grandparent do

class A {
  var a : int;

  proc init() {
    writeln("A.init");
  }

  proc postinit() {
    writeln("A.postinit");
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

  proc postinit() {
    writeln("C.postinit");
  }
}

var c = new unmanaged C();
delete c;
writeln();

// Parent has postinit, but child does not

class Parent {
  var p : int;

  proc init() {
    writeln("Parent.init");
  }

  proc postinit() {
    writeln("Parent.postinit");
  }
}

class Child : Parent {
  var c : int;

  proc init() {
    writeln("Child.init");
  }
}

var child = new unmanaged Child();
delete child;
