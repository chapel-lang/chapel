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

class Dense: Arithmetic {
  def clear() {
    compilerError("Can't clear a dense domain");
  }

  def clearHelp() {
    compilerError("Illegal assignment to a dense domain");
  }
}

class Sparse: Arithmetic {
  def clear() {
    writeln("Clearing a sparse domain");
  }

  def clearHelp() {
    writeln("In sparse domain clearHelp");
    clear();
  }
}

class Assoc: Abstract {
  def clear() {
    writeln("Clearing an associative domain");
  }

  def clearHelp() {
    writeln("In associative domain clearHelp");
    clear();
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
