class mysumreduce {
  type t;
  var state: t;
  
  def init() {
    state = 0;
  }

  def combine(x: t) {
    state += x;
  }

  def finalize(): t {
    return state;
  }
}

config var n: int = 10;

var D: domain(1) = [1..n];

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce = new mysumreduce(t = int);
myreduce.init();
for i in D {
  myreduce.combine(A(i));
}
var result = myreduce.finalize();

writeln("result is: ", result);
