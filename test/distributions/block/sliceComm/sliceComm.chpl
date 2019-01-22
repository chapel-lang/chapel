use BlockDist, CommDiagnostics;

config const n = 11;

config const aggregate = false;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

startCommDiagnostics();
A[4..4, ..] = 1;
A[.., 11..11] = 2;
A[8, ..] = 3;
A[.., 2] = 4;
A[4..8, 4..8] = 5;
stopCommDiagnostics();

writeln(A);
if (aggregate) then
  writeln(summarize(getCommDiagnostics()));
else
  writeln(getCommDiagnostics());

proc summarize(xs: [] chpl_commDiagnostics()) {
  var res: chpl_commDiagnostics;
  res.get = + reduce [x in xs] x.get;
  res.get_nb = + reduce [x in xs] x.get_nb;
  res.put = + reduce [x in xs] x.put;
  res.put_nb = + reduce [x in xs] x.put_nb;
  res.test_nb = + reduce [x in xs] x.test_nb;
  res.wait_nb = + reduce [x in xs] x.wait_nb;
  res.try_nb = + reduce [x in xs] x.try_nb;
  res.execute_on = + reduce [x in xs] x.execute_on;
  res.execute_on_fast = + reduce [x in xs] x.execute_on_fast;
  res.execute_on_nb = + reduce [x in xs] x.execute_on_nb;

  return res;
}
