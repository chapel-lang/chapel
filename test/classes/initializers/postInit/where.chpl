
class Grandparent {
  var g : int;

  proc init() {
    writeln("Grandparent.init");
  }
  proc postInit() {
    writeln("Grandparent.postInit");
  }
}

class Parent : Grandparent {
  type eltType;
  var x : eltType;

  proc init(type t) {
    this.eltType = t;
    writeln("Parent.init");
  }

  proc postInit() where eltType == int {
    writeln("Special Parent.postInit");
  }
}

class Child : Parent {
  var y : 2*eltType;

  proc init(type t) {
    super.init(t);
    writeln("Child.init");
  }
  proc postInit() {
    writeln("Child(",eltType:string,").postInit");
  }
}

var c = new Child(real);
delete c;
writeln();

var i = new Child(int);
delete i;
