use common;

var D = createDom({1..10});
var A: [D] int;

{
  writeln("Iterand is a different symbol");
  const DInner = D.expand(-1);
  forall i in DInner with (ref A) {
    A[i] = i;
  }

  writeln(A);
  writeln();
}

{
  writeln("Iterand is a call");
  forall i in D.expand(-1) with (ref A) {
    A[i] = i;
  }

  writeln(A);
  writeln();
}

{
  writeln("Local subdomain patterns");

  coforall l in Locales with (ref A) do on l {

    forall i in D.localSubdomain() with (ref A) {
      A[i] = i;
    }

    const localD = D.localSubdomain();

    forall i in localD  with (ref A){
      A[i] = i;
    }

    const innerLocalD = localD.expand(-1);
    forall i in innerLocalD with (ref A) {
      A[i] += i;
    }
  }

  writeln(A);
  writeln();
}

{
  writeln("Count based domain slices");

  forall i in D#D.size-1 with (ref A) {
    A[i] = i;
  }

  var DExceptLast = D#D.size-1;

  forall i in DExceptLast with (ref A) {
    A[i] = i;
  }

  writeln(A);
  writeln();
}
