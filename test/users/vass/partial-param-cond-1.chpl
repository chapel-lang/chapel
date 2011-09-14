inline proc range.test() where stridable {
  proc checkAmb() return (aligned == false);
  proc checkAl(al) return aligned && (alignment == al);
  if stride > 0 {
    if hasLowBound() then return checkAl(low);
//    else                  return checkAmb();
  } else {
    if hasHighBound() then return checkAl(high);
//    else                  return checkAmb();
  }
  return checkAmb();
}

proc show(msg, r) {
  if r.test() then writeln(msg, " yes"); else writeln(msg, " no");
}

show("(1.. by -2)(0..5)",
      (1.. by -2)(0..5) );

show("(1.. by -2)(1..5)",
      (1.. by -2)(1..5) );

show("(1.. by -2)(2..5)",
      (1.. by -2)(2..5) );

show("(.. by 2)(1..5)",
      (.. by 2)(1..5) );

show("(.. by -2)(1..5)",
      (.. by -2)(1..5) );

// assuming the present "relatively prime" clause
show("(.. by -2)(1..5 by -3)",
      (.. by -2)(1..5 by -3) );

// not ambiguously aligned, to contrast the above
writeln();

show("(1.. by 2)(1..5)",
      (1.. by 2)(1..5) );

show("1..5 by 2",
      1..5 by 2 );

show("(..)(1..5 by 2)",
      (..)(1..5 by 2) );
