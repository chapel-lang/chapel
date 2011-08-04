var vtestcount = 0;
var ptestcount = 0;
var errcount = 0;
config const verbose = true;

proc verify(i, j, funresult) {
  const reference = ceil(i:real / j:real);
  if funresult:real != reference {
    errcount += 1;
    writeln("failed test(", i, ":", typeToString(i.type), ",",
            j, ":", typeToString(j.type), ")  expected ", reference,
            "  got ", funresult, ":", typeToString(funresult.type));
  }
}

proc launch(type ta, type tb) {
  const a1 = if _isSignedType(ta) then -11:ta else 0:ta;
  const a2 = 11:ta;
  const b1 = if _isSignedType(tb) then -11:tb else 1:tb;
  const b2 = 11:tb;
  const tcv = vtestcount;

  for b in b1..b2 do if !_isSignedType(tb) || b != 0 then
    for a in a1..a2 do {
      vtestcount += 1;
      verify(a:ta, b:tb, ceil(a:ta, b:tb));
    }

  if verbose then
    writeln(format("####", vtestcount - tcv), " var tests ",
            typeToString(ta), ",", typeToString(tb),
            "->", typeToString(ceil(a2,b2).type));

  // same as above, but test the 'param' version of ceil(n,m)
  param e1 = if _isSignedType(ta) then -7:ta else 0:ta;
  param e2 = 7:ta;
  param f1 = if _isSignedType(tb) then -7:tb else 1:tb;
  param f2 = 7:tb;
  const tcp = ptestcount;

  for param f in f1..f2 do if f != 0 then
    for param e in e1..e2 {
      ptestcount += 1;
      param funres = ceil(e, f); // ensure ceil() produces a param
      verify(e, f, funres);
    }

  if verbose then
    writeln(format("####", ptestcount - tcp), " par tests ",
            typeToString(ta), ",", typeToString(tb),
            "->", typeToString(ceil(e2,f2).type), "\n");
}

// the calls commented out below hit typechecking issues in ceil()

launch(int(32), int(32));
launch(int(32), int(64));
launch(int(32), uint(32));
//launch(int(32), uint(64));

launch(int(64), int(32));
launch(int(64), int(64));
launch(int(64), uint(32));
//launch(int(64), uint(64));

launch(uint(32), int(32));
launch(uint(32), int(64));
launch(uint(32), uint(32));
//launch(uint(32), uint(64));

//launch(uint(64), int(32));
//launch(uint(64), int(64));
//launch(uint(64), uint(32));
launch(uint(64), uint(64));

writeln("done ", vtestcount, " var tests, ", ptestcount, " param tests");

if errcount == 0 then
  writeln("SUCCESS");
else
  writeln("FAILED ", errcount, " tests");
