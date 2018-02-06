use ReplicatedDist, ReplicatedVar;

proc writeReplicands(x, locs) {
  for loc in locs {
    on loc {
      writeln(loc, ":");
      writeln(x);
    }
  }
}

proc writeReplicands(x) {
  writeReplicands(x, Locales);
}

{
  writeln("\nsimple case");
  var x: [rcDomain] real;
  writeln("\ninitially");
  writeReplicands(x);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate");
  writeReplicands(x);
  on (if numLocales >= 3 then Locales[2] else Locales[0]) do
    x[1] = 33;
  writeln("\nafter 'on'");
  writeReplicands(x);
  var c: [LocaleSpace] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}

if numLocales >= 4 {
  var myL = Locales(1..3 by 2);
//  myL[1] = myL[2];  // this makes myL violate the "consistency" requirement
  writeln("\nadvanced case: ", myL);
  var x: [rcDomainBase dmapped Replicated(myL)] real;
  writeln("\ninitially");
  writeReplicands(x, myL);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate");
  writeReplicands(x, myL);
  on Locales[3] do
    x[1] = 33;
  writeln("\nafter 'on'");
  writeReplicands(x, myL);
  var c: [myL.domain] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}
