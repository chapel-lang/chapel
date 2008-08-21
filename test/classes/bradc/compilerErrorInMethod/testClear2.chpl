config param doDense = false;

record Wrap {
  var _value;
}

def =(lhs:Wrap, rhs) {
  lhs._value.clearHelp();
  return lhs;
}

class Abstract {
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
  def clearHelp() {
    writeln("In abstract sparse domain clearHelp");
    clear();
  }

  def clear() {
    halt("This distribution does not implement clear");
  }
}

class AbsAssoc: Abstract {
  def clearHelp() {
    writeln("In abstract associative domain clearHelp");
    clear();
  }

  def clear() {
    halt("This distribution does not implement clear");
  }
}

class Dense : AbsDense {
}

class Sparse : AbsSparse {
  def clear() {
    writeln("Clearing a sparse domain");
  }
}

class Assoc : AbsAssoc {
  def clear() {
    writeln("Clearing an associative domain");
  }
}


var d = new Wrap(_value = new Dense());
var s = new Wrap(_value = new Sparse());
var a = new Wrap(_value = new Assoc());

if doDense {
  d = 1;
}
s = 1;
a = 1;
