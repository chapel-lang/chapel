class mysumreduce {
  type intype;
  type statetype;
  type outtype;
  
  function ident(): statetype {
    return 0;
  }

  function combine(x: statetype, y: intype): statetype {
    return x + y;
  }

  function result(x: statetype): outtype {
    return x;
  }

  constructor mysumreduce(type in_t) {
    intype = in_t;
    statetype = in_t;
    outtype = in_t;
  }
}

config var n: integer = 10;

var D: domain(1) = (1..n);

var A: [D] integer;

forall i in D {
  A(i) = i;
}

var myreduce = mysumreduce(in_t = integer);
var state: myreduce.statetype = myreduce.ident();
for i in D {
  state = myreduce.combine(state, A(i));
}
var result = state;

writeln("result is: ", result);
