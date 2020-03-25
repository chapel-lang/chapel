
//
// Simple case where parent has single param field
//

class Dummy {
  param stridable : bool;
}

class Parent {
  param stridable : bool;
  var x : int;
}

class Child : Parent {
  var y : unmanaged Dummy(stridable)?;
}

var c = new unmanaged Child(false, 5);
writeln('c = ', c);
delete c;

//
// A more complex case where a parent has multiple instantiations, which
// currently involves different paths in the compiler.
//

class A {
  param rank : int;
  type idxType;
  param stridable : bool;
}

class Z : A {
  var x : unmanaged Dummy(stridable)?;
}

var z = new owned Z(1, int, false);
writeln("z = ", z);

//
// A case replicating internal domain representations. Having two classes
// refer to each other in this manner triggers yet another path for
// instantiation.
//

class DummyBaseDom {
  param rank : int;
  type idxType;
  param stridable : bool;
}

class DummyMyDom : DummyBaseDom {
  var locs : [1..4] unmanaged DummyLocMyDom(rank, idxType, stridable)?;
}

class DummyLocMyDom {
  param rank : int;
  type idxType;
  param stridable : bool;
  var parent : unmanaged DummyMyDom(rank, idxType, stridable);
}

var dmd = new unmanaged DummyMyDom(1, int, false);
writeln("dmd = ", dmd);
delete dmd;
