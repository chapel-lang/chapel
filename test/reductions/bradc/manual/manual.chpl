class mysumreduce {
  type t;
  
  function ident() {
    return 0;
  }

  function combine(x: t, y: t): t {
    return x + y;
  }
}

config var n: integer = 10;

var D: domain(1) = (1..n);

var A: [D] integer;

forall i in D {
  A(i) = i;
}

var myreduce = mysumreduce(t = integer);
var state: integer = myreduce.ident();
for i in D {
  state = myreduce.combine(state, A(i));
}
var result = state;

writeln("result is: ", result);
