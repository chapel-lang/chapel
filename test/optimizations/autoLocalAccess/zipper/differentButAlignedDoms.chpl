use common;

var D = createDom({1..10});
var A: [D] int;

{
  writeln("Iterand is a different symbol");
  const DInner = D.expand(-1);
  forall (i, j) in zip(DInner, 100..) with (ref A) {
    A[i] = j;
  }

  writeln(A);
  writeln();
}

{
  writeln("Iterand is a call");
  forall (i,j) in zip(D.expand(-1), 100..) with (ref A) {
    A[i] = j;
  }

  writeln(A);
  writeln();
}

{
  writeln("Local subdomain patterns");

  coforall l in Locales with (ref A) do on l {

    forall (i,j) in zip(D.localSubdomain(), 100..) with (ref A) {
      A[i] = j;
    }

    const localD = D.localSubdomain();

    forall (i,j) in zip(localD, 100..) with (ref A) {
      A[i] = j;
    }

    const innerLocalD = localD.expand(-1);
    forall (i,j) in zip(innerLocalD, 100..) with (ref A) {
      A[i] += j;
    }
  }

  writeln(A);
  writeln();
}

{
  writeln("Count based domain slices");

  forall (i,j) in zip(D#D.size-1, 100..) with (ref A) {
    A[i] = j;
  }

  var DExceptLast = D#D.size-1;

  forall (i,j) in zip(DExceptLast, 100..) with (ref A) {
    A[i] = j;
  }

  writeln(A);
  writeln();
}
