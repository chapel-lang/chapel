config param doDense = false;

record Wrap {
  var _value;
}

def =(lhs:Wrap, rhs) {
  lhs._value.clearHelp();
  return lhs;
}

class Abstract {
  def clear() {
    halt("This class has not implemented clear");
  }

}

class Arithmetic : Abstract {
}

class AbsDense: Arithmetic {
  def clear() {
    compilerError("Can't clear a dense domain");
  }

  def clearHelp() {
    compilerError("Illegal assignment to a dense domain");
  }
}

class AbsSparse: Arithmetic {
  def clear() {
    writeln("Clearing a sparse domain");
  }

  def clearHelp() {
    writeln("In abstract sparse domain clearHelp");
    clear();
  }
}

class AbsAssoc: Abstract {
  def clear() {
    writeln("Clearing an associative domain");
  }

  def clearHelp() {
    writeln("In abstract associative domain clearHelp");
    clear();
  }
}

class Dense : AbsDense {
}

class Sparse : AbsSparse {
}

class Assoc : AbsAssoc {
}


var d = new Wrap(_value = new Dense());
var s = new Wrap(_value = new Sparse());
var a = new Wrap(_value = new Assoc());

if doDense {
  d = 1;
}
s = 1;
a = 1;
