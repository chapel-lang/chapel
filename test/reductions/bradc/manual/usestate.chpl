class mysumreduce {
  type t;
  var state: t;
  
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
myreduce.state = myreduce.ident();
for i in D {
  myreduce.state = myreduce.combine(myreduce.state, A(i));
}
var result = myreduce.state;

writeln("result is: ", result);
