use ReplicatedDist, UtilReplicatedVar;

{
  writeln("\nsimple case");
  var x: [rcDomain] real;
  writeln("\ninitially");
  x._value.writeReplicands(sorted=true);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate");
  x._value.writeReplicands(sorted=true);
  on (if numLocales >= 3 then Locales[2] else Locales[0]) do
    x[1] = 33;
  writeln("\nafter 'on'");
  x._value.writeReplicands(sorted=true);
  var c: [LocaleSpace] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}

if numLocales >= 4 {
  var myL = Locales(1..3 by 2);
//  myL[1] = myL[2];  // this makes myL violate the "consistency" requirement
  writeln("\nadvanced case: ", myL);
  var x: [rcDomainBase dmapped ReplicatedDist(myL)] real;
  writeln("\ninitially");
  x._value.writeReplicands(sorted=true);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate");
  x._value.writeReplicands(sorted=true);
  on Locales[3] do
    x[1] = 33;
  writeln("\nafter 'on'");
  x._value.writeReplicands(sorted=true);
  var c: [myL.domain] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}
