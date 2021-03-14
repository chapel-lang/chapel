
record Dummy {
  type idxType;
  param stridable : bool;

  proc init(type idxType, param stridable : bool) {
    this.idxType = idxType;
    this.stridable = stridable;
  }
}

class Parent {
  type idxType;

  proc init(type idxType) {
    this.idxType = idxType;
  }
}

class Child : Parent {
  param stridable;

  // Trouble here if compiler does not replace 'stridable' with instantiation
  var d : Dummy(idxType, stridable=stridable);

  proc init(type idxType, param stridable) {
    super.init(idxType);
    this.stridable = stridable;
  }
}

var c = new unmanaged Child(int, false);
writeln("c = ", c);
delete c;
