use common;

var D = createDom({1..10});
var A: [D] int;

// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  writeln("Custom this was called");
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: int) ref {
  writeln("Custom localAccess was called");
  return this._value.dsiLocalAccess((i:int,));
}


{
  writeln("Iterand is a different symbol");
  const DInner = D.expand(-1);
  forall i in DInner {
    A[i] = i;
  }

  writeln(A);
  writeln();
}

{
  writeln("Iterand is a call");
  forall i in D.expand(-1) {
    A[i] = i;
  }

  writeln(A);
  writeln();
}

{
  writeln("Local subdomain patterns");

  coforall l in Locales do on l {

    forall i in D.localSubdomain() {
      A[i] = i;
    }

    const localD = D.localSubdomain();

    forall i in localD {
      A[i] = i;
    }

    const innerLocalD = localD.expand(-1);
    forall i in innerLocalD {
      A[i] += i;
    }
  }

  writeln(A);
  writeln();
}

{
  writeln("Count based domain slices");

  forall i in D#D.size-1 {
    A[i] = i;
  }

  var DExceptLast = D#D.size-1;

  forall i in DExceptLast {
    A[i] = i;
  }

  writeln(A);
  writeln();
}
