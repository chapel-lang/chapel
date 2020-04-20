config param doDense = false;

record Wrap {
  var _value;
}

proc Wrap.init=(other: Wrap) {
  this._value = other._value;
}

proc =(ref lhs:Wrap, rhs) {
  lhs._value.clearHelp();
}

class Abstract {
  proc clear() {
    halt("This class has not implemented clear");
  }

}

class Arithmetic : Abstract {
}

class AbsDense: Arithmetic {
  override proc clear() {
    halt("Can't clear a dense domain");
  }

  proc clearHelp() {
    halt("Illegal assignment to a dense domain");
  }
}

class AbsSparse: Arithmetic {
  proc clearHelp() {
    writeln("In abstract sparse domain clearHelp");
    clear();
  }
}

class AbsAssoc: Abstract {
  proc clearHelp() {
    writeln("In abstract associative domain clearHelp");
    clear();
  }
}

class Dense : AbsDense {
}

class Sparse : AbsSparse {
  override proc clear() {
    writeln("Clearing a sparse domain");
  }
}

class Assoc : AbsAssoc {
  override proc clear() {
    writeln("Clearing an associative domain");
  }
}


var d = new Wrap(_value = new unmanaged Dense());
var s = new Wrap(_value = new unmanaged Sparse());
var a = new Wrap(_value = new unmanaged Assoc());

if doDense {
  d = 1;
}

s = 1;
a = 1;

delete a._value;
delete s._value;
delete d._value;
