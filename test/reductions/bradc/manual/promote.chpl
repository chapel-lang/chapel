class mysumreduce {
  type t;
  var state: sync t;

  proc initState() {
    state = 0;
  }

  proc combine(x: t) {
    state += x;
  }

  proc finalize(): t {
    return state;
  }
}

config var n: int = 10;

var D: domain(1) = {1..n};

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce = new mysumreduce(t = int);

myreduce.initState();
myreduce.combine(A);

var result = myreduce.finalize();

writeln("result is: ", result);

delete myreduce;
