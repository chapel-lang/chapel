
//
// Ensure that param/type variables from grandparent classes (and beyond) can
// be used by child classes.
//

class GrandParent {
  param rank : int;
  type idxType;
}

class Parent : GrandParent {
  var x : int;
}

class Child : Parent {
  var indices : index(rank, idxType);
}

var ownC = new owned Child(1, int);
var c = ownC.borrow();
writeln("c = ", c);
