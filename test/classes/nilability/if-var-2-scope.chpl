// Error: accessing the if-var variable outside its scope

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
      cNN.show();
    else
      writeln(cNN);  // error
  }
}

showCs();
