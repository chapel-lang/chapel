
record X {
  param p = 0;
  def initialize() { 
    writeln("X has p=", p); 
  }
}


// // this works
// def main() {
//  type t = X;
//  var y : t;
//}

// but this doesn't 
type t = X;
var y : t;
