proc checkLog(x, baseLog2, expect) {

  var ok = true;

  if baseLog2 == 1 {
    ok &&= (log2(x) == expect);
  }
  ok &&= (logBasePow2(x, baseLog2) == expect);

  // Check that it matches the old way of computing it.
  var result = -1;
  var val = x;
  while (val != 0) {
    val >>= baseLog2;
    result += 1;
  }
  ok &&= (result == expect);

  if !ok {
    writeln("Failed with x=", x, " :", x.type:string,
            " baseLog2=", baseLog2,
            " expect=", expect);
  }
}

proc checkLogs(type t) {
  var nbits = if isUint(t) then numBits(t) else numBits(t)-1;

  for baseLog2 in 1..nbits {
    for i in 0..#nbits {
      if baseLog2 < nbits && baseLog2*i < nbits {
        var val = (1:t) << (baseLog2*i);
        /*
        writeln("t=", t:string,
                " val=", val,
                " baseLog2=", baseLog2,
                " i=", i);
         */

        checkLog(val, baseLog2, i);
        if val >= 2 {
          checkLog(val+1, baseLog2, i);
        }
        if val >= 4 {
          checkLog(val-1, baseLog2, i-1);
        }
      }
    }
  }
}

checkLogs(uint(8));
checkLogs(uint(16));
checkLogs(uint(32));
checkLogs(uint(64));
checkLogs(int(8));
checkLogs(int(16));
checkLogs(int(32));
checkLogs(int(64));
