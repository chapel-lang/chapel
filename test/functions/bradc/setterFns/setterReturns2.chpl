record myarr {
  var x: real;

  def =this(i:int, j: int, v:real) {
    return x;
  }
}

var a: myarr;

a(1,1) = 1.0;

writeln("Shouldn't compile, I think.");
