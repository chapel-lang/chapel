class mysumreduce {
  type intype;
  type statetype;
  type outtype;

  proc ident(): statetype {
    return 0;
  }

  proc combine(x: statetype, y: intype): statetype {
    return x + y;
  }

  proc result(x: statetype): outtype {
    return x;
  }

  proc init(type intype, type statetype=intype, type outtype=intype) {
    this.intype = intype;
    this.statetype = statetype;
    this.outtype = outtype;
  }
}

config var n: int = 10;

var D: domain(1) = {1..n};

var A: [D] int;

forall i in D {
  A(i) = i;
}

var myreduce                  = new unmanaged mysumreduce(int);
var state: myreduce.statetype = myreduce.ident();

for i in D {
  state = myreduce.combine(state, A(i));
}

var result = state;

writeln("result is: ", result);

delete myreduce;
