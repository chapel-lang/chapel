class mysumreduce {
  type intype;
  type statetype: intype;
  type outtype: intype;
  
  function ident(): statetype {
    return 0;
  }

  function combine(x: statetype, y: intype): statetype {
    return x + y;
  }

  function result(x: statetype): outtype {
    return x;
  }
}

config var n: int = 10;

var D: domain(1) = (1..n);

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce = mysumreduce(intype = int);
var state: myreduce.statetype = myreduce.ident();
for i in D {
  state = myreduce.combine(state, A(i));
}
var result = state;

writeln("result is: ", result);
