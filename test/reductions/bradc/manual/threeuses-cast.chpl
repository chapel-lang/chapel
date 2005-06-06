class mysumreduce {
  type t;
  
  function ident() {
    return 0:t;
  }

  function combine(x: t, y: t): t {
    return x + y;
  }
}

config var n: integer = 10;

var D: domain(1) = (1..n);

var A: [D] integer;
var B: [D] float;
var C: [D] string;

forall i in D {
  A(i) = i;
  B(i) = i;
  C(i) = i;
}

{
  var myreduce = mysumreduce(t = integer);
  var state: integer = myreduce.ident();
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

{
  var myreduce = mysumreduce(t = string);
  var state: string = myreduce.ident();
  for i in D {
    state = myreduce.combine(state, C(i));
  }
  var result = state;

  writeln("result is: ", result);
}
