config param doDense = false;

record Wrap {
  var _value;
}

proc +=(ref lhs:Wrap, rhs) {
  lhs._value.clearHelp();
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
  proc clear() {
    writeln("Clearing a sparse domain");
  }

  proc clearHelp() {
    writeln("In sparse domain clearHelp");
    clear();
  }
}

class Assoc: Abstract {
  proc clear() {
    writeln("Clearing an associative domain");
  }

  proc clearHelp() {
    writeln("In associative domain clearHelp");
    clear();
  }
}

var d = new Wrap(_value = new Dense());
var s = new Wrap(_value = new Sparse());
var a = new Wrap(_value = new Assoc());

if doDense {
  d += 1;
}
s += 1;
a += 1;
