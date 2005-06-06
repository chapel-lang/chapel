class mysumreduce {
  type t;
  var state: t;
  
  function init() {
    state = 0;
  }

  function combine(x: t) {
    state += x;
  }

  function finalize(): t {
    return state;
  }
}

config var n: integer = 10;

var D: domain(1) = (1..n);

var A: [D] integer;

forall i in D {
  A(i) = i;
}

var myreduce = mysumreduce(t = integer);
myreduce.init();
for i in D {
  myreduce.combine(A(i));
}
var result = myreduce.finalize();

writeln("result is: ", result);
