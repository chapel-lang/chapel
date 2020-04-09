
class Grandparent {
  var g : int;

  proc init() {
    writeln("Grandparent.init");
  }
  proc postinit() {
    writeln("Grandparent.postinit");
  }
}

class Parent : Grandparent {
  type eltType;
  var x : eltType;

  proc init(type t) {
    this.eltType = t;
    writeln("Parent.init");
  }

  proc postinit() where eltType == int {
    writeln("Special Parent.postinit");
  }
}

class Child : Parent {
  var y : 2*eltType;

  proc init(type t) {
    super.init(t);
    writeln("Child.init");
  }
  proc postinit() {
    writeln("Child(",eltType:string,").postinit");
  }
}

var c = new unmanaged Child(real);
delete c;
writeln();

var i = new unmanaged Child(int);
delete i;
