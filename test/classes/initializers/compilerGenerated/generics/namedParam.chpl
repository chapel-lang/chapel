
record Dummy {
  type idxType;
  param stridable : bool;

  proc init(type idxType, param stridable : bool) {
    this.idxType = idxType;
    this.stridable = stridable;
  }
}

class A {
  type idxType;
}

class Z : A {
  param stridable;

  var d : Dummy(int, stridable=stridable);
}

var z = new owned Z(int, false);
writeln("z = ", z);
