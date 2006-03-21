class mysumreduce {
  type t;
  
  fun ident() {
    return 0;
  }

  fun combine(x: t, y: t): t {
    return x + y;
  }
}

config var n: int = 10;

var D: domain(1) = [1..n];

var A: [D] int;
var B: [D] float;

forall i in D {
  A(i) = i;
  B(i) = i;
}

{
  var myreduce = mysumreduce(t = int);
  var state: int = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, A(i));
  }
  var result = state;

  writeln("result is: ", result);
}

{
  var myreduce = mysumreduce(t = float);
  var state: float = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, B(i));
  }
  var result = state;

  writeln("result is: ", result);
}
