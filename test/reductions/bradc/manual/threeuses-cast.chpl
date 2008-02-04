class mysumreduce {
  type t;
  
  def ident() {
    return 0:t;
  }

  def combine(x: t, y: t): t {
    return x + y;
  }
}

config var n: int = 10;

var D: domain(1) = [1..n];

var A: [D] int;
var B: [D] real;
var C: [D] string;

forall i in D {
  A(i) = i;
  B(i) = i;
  C(i) = i;
}

{
  var myreduce = new mysumreduce(t = int);
  var state: int = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, A(i));
  }
  var result = state;

  writeln("result is: ", result);
}

{
  var myreduce = new mysumreduce(t = real);
  var state: real = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, B(i));
  }
  var result = state;

  writeln("result is: ", result);
}

{
  var myreduce = new mysumreduce(t = string);
  var state: string = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, C(i));
  }
  var result = state;

  writeln("result is: ", result);
}
