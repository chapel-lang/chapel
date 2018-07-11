
record Dummy {
  type idxType;
  param stridable : bool;

  proc init(type i, param s : bool) {
    this.idxType = i;
    this.stridable = s;
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

var c = new Child(int, false);
writeln("c = ", c);
delete c;
