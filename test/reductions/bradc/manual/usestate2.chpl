class mysumreduce {
  type t;
  var state: t;
  
  fun init() {
    state = 0;
  }

  fun combine(x: t) {
    state += x;
  }

  fun finalize(): t {
    return state;
  }
}

config var n: int = 10;

var D: domain(1) = [1..n];

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce = mysumreduce(t = int);
myreduce.init();
for i in D {
  myreduce.combine(A(i));
}
var result = myreduce.finalize();

writeln("result is: ", result);
