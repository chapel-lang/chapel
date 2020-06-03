
record Vector {
  type T;
  var D : domain(1);
  var A : [D] T;

  proc init(type T) {
    writeln("regular init from type");
    this.T = T;
  }

  proc init(data : []) {
    writeln("regular init from array");
    this.T = data.eltType;
    this.D = data.domain;
    this.A = data;
  }

  // Classic copy-initializer
  proc init=(other : this.type) {
    writeln("classic copy init");
    this.T = other.T;
    this.D = other.D;
    this.A = other.A;
  }
}

writeln("----- Classic Initialization -----");
{
  var A : Vector(int);
  var B = A; A; // don't copy-elide
  var C = new Vector([1, 2, 3]);
}

writeln("\n\n\n");

// Initialize a particular Vector type from an array of the same eltType
proc Vector.init=(data : [] this.type.T) {
  writeln("typed init= from array");
  this.T = this.type.T;
  this.D = data.domain;
  this.A = data;
}

writeln("----- Initialize Typed Generic from Expr -----");
{
  var A : Vector(int) = [1, 2, 3];
  writeln(A);
}
