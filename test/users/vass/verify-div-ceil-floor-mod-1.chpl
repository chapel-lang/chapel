var vtestcount = 0;
var ptestcount = 0;
var errcount = 0;
config const verbose = true;

proc verify1(i, j, name:string, result, reference:real) {
  if result:real != reference {
    errcount += 1;
    writeln("failed ", name, "(", i, ":", typeToString(i.type), ",",
            j, ":", typeToString(j.type), ")  expected ", reference,
            "  got ", result, ":", typeToString(result.type));
  }
}

proc verify(i, j, ceilres, floorres, modres) {
  verify1(i, j, "ceil",  ceilres,  ceil (i:real / j:real));
  verify1(i, j, "floor", floorres, floor(i:real / j:real));
  verify1(i, j, "mod",   modres,   mod  (i:real , j:real));
}

proc launch(type ta, type tb) {
  const a1 = if isIntType(ta) then -11:ta else 0:ta;
  const a2 = 11:ta;
  const b1 = if isIntType(tb) then -11:tb else 1:tb;
  const b2 = 11:tb;
  const tcv = vtestcount;

  for b in b1..b2 do if !isIntType(tb) || b != 0 then
    for a in a1..a2 do {
      vtestcount += 1;
      verify(a:ta, b:tb,
             divceil(a:ta, b:tb), divfloor(a:ta, b:tb), mod(a:ta, b:tb));
    }

  if verbose then
    writeln("%{####}".format(vtestcount - tcv), " var tests ",
            typeToString(ta), ",", typeToString(tb), " -> ",
            typeToString(divceil(a2,b2).type), " ",
            typeToString(divfloor(a2,b2).type), " ",
            typeToString(mod(a2,b2).type));

  // same as above, but test the 'param' versions
  param e1 = if isIntType(ta) then -7:ta else 0:ta;
  param e2 = 7:ta;
  param f1 = if isIntType(tb) then -7:tb else 1:tb;
  param f2 = 7:tb;
  const tcp = ptestcount;

  for param f in f1..f2 do if f != 0 then
    for param e in e1..e2 {
      ptestcount += 1;
      param cres = divceil(e, f); // ensure these produce params
      param fres = divfloor(e, f);
      param mres = mod(e, f);
      verify(e, f, cres, fres, mres);
    }

  if verbose then
    writeln("%{####}".format(ptestcount - tcp), " par tests ",
            typeToString(ta), ",", typeToString(tb), " -> ",
            typeToString(divceil(e2,f2).type), " ",
            typeToString(divfloor(e2,f2).type), " ",
            typeToString(mod(e2,f2).type), "\n");
}

// the calls commented out below hit typechecking issues in divceil()

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
