
//
// Ensure that param/type variables from grandparent classes (and beyond) can
// be used by child classes.
//

class GrandParent {
  param rank : int;
  type idxType;

  proc init(param rank : int, type idxType) {
    this.rank = rank;
    this.idxType = idxType;
  }
}

class Parent : GrandParent {
  proc init(param rank : int, type idxType) {
    super.init(rank, idxType);
  }
}

class Child : Parent {
  var indices : index(rank, idxType);

  proc init(param rank : int, type idxType) {
    super.init(rank, idxType);
  }
}

var c = new unmanaged Child(1, int);
writeln("c = ", c);
delete c;
