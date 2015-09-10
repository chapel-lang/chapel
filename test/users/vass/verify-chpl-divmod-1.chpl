var vtestcount = 0;
var ptestcount = 0;
var errcount = 0;
config const verbose = true;

proc verify(n, d)
{
  // n - dividend
  // d - divisor
  // q - quotient
  // r - remainder
  const q = n / d;
  const r = n % d;

  proc report(message) { errcount += 1; writeln((n,d,q,r), ": ", message); }

  // The identity q * d + r = n should always be true.
  if q * d + r != n then
    report("Integer division identity does not hold.");

  // We want the remainder to obey two additional properties:
  // 1. Its sign is the same as that of the dividend. (Unless it's 0.)
  // 2. Its absolute value is less than that of the divisor.
  // This guarantees that the quotient is as large as it can be in absolute value
  // but q * d is still closer to zero than n.
  if r != 0 && sgn(r) != sgn(n) then
    report("The sign of the remainder does not match the sign of the dividend.");
  if abs(r) >= abs(d) then
    report("The remainder is not less than the divisor.");

  // Write out the results, so they can be matched.
 if abs(n) == 11 && abs(d) == 3 {
  writeln(n, " / ", d, " = ", q);
  writeln(n, " % ", d, " = ", r);
 }
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
      verify(a:ta, b:tb);
    }

  if verbose then
    writeln("%{####}".format(vtestcount - tcv), " var tests ",
            typeToString(ta), ",", typeToString(tb), " -> ",
            typeToString((a2/b2).type), " ",
            typeToString((a2%b2).type));
}

// the combinations commented out below have typechecking issues

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
