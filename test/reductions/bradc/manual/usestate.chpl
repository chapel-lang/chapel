class mysumreduce {
  type t;
  var  state: t;

  proc ident() {
    return 0;
  }

  proc combine(x: t, y: t): t {
    return x + y;
  }
}

config var n: int = 10;

var D: domain(1) = {1..n};

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce   = new unmanaged mysumreduce(t = int);
myreduce.state = myreduce.ident();

for i in D {
  myreduce.state = myreduce.combine(myreduce.state, A(i));
}

var result = myreduce.state;

writeln("result is: ", result);

delete myreduce;
