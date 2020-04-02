config const beNoisy = false;
{
  writeln("Locales array iteration");
  for loc in Locales {
    const A = (loc._value:LocaleModel).getChildArray();
    forall a in A do
      if beNoisy then writeln((a, chpl_getSubloc()));
  }
}

config const n = 4;
{
  writeln("range iteration");
  var R = 1..n;
  forall r in R do
    if beNoisy then writeln((r, chpl_getSubloc()));
}

{
  writeln("zippered range iteration");
  var R0 = 1..n;
  var R1 = n+1..2*n;
  forall (r0,r1) in zip(R0,R1) do
    if beNoisy then writeln((r0, r1, chpl_getSubloc()));
}

{
  writeln("domain iteration");
  var D = {1..n};
  forall d in D do
    if beNoisy then writeln((d, chpl_getSubloc()));
}

{
  writeln("zippered domain iteration");
  var D0 = {1..n};
  var D1 = {n+1..2*n};
  forall (d0,d1) in zip(D0,D1) do
    if beNoisy then writeln((d0, d1, chpl_getSubloc()));
}

{
  writeln("array iteration");
  var A: [1..n] int = 1..n;
  forall a in A do
    if beNoisy then writeln((a, chpl_getSubloc()));
}

{
  writeln("zippered array iteration");
  var A: [1..n] int = 1..n;
  var B: [1..n] int = n+1..2*n;
  forall (a,b) in zip(A,B) do
    if beNoisy then writeln((a, b, chpl_getSubloc()));
}

writeln("done");
