
record Dummy {
  type idxType;
  param stridable : bool;

  proc init(type i, param s : bool) {
    this.idxType = i;
    this.stridable = s;
  }
}

class A {
  type idxType;
}

class Z : A {
  param stridable;

  var d : Dummy(int, stridable=stridable);
}

var z = new Z(int, false);
writeln("z = ", z);
delete z;
