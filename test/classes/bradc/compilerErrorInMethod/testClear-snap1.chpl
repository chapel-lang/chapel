config param doDense = false;

record Wrap {
  var _value;
}

proc Wrap.init=(other: Wrap) {
  this._value = other._value;
}
proc Wrap.init=(other) {
  this._value = other;
}

proc =(ref lhs:Wrap, rhs) {
  lhs._value.clearHelp();
}

operator :(rhs, type t:Wrap) {
  var tmp: t = rhs;
  return tmp;
}

class Abstract {
  proc clear() {
    halt("This class has not implemented clear");
  }

}

class Arithmetic : Abstract {
}

class Dense: Arithmetic {
  proc clear() {
    compilerError("Can't clear a dense domain");
  }

  proc clearHelp() {
    compilerError("Illegal assignment to a dense domain");
  }
}

class Sparse: Arithmetic {
  override proc clear() {
    writeln("Clearing a sparse domain");
  }

  proc clearHelp() {
    writeln("In sparse domain clearHelp");
    clear();
  }
}

class Assoc: Abstract {
  override proc clear() {
    writeln("Clearing an associative domain");
  }

  proc clearHelp() {
    writeln("In associative domain clearHelp");
    clear();
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

