// Error: assigning to const variable

class C {
  var x: int;
  proc show() { writeln("C(", x, ")"); }
}

var nums = [2, 5, 0, 3];

var Cs = [i in nums] if i then new C[i] else nil: owned C?;

const globalC = new C(11);

proc showCs() {
  for c in Cs {
    if const cNN = c then
      cNN = globalC.borrow();  // error
    else
      writeln("nil");
  }
}

showCs();
