use ReplicatedDist;

{
  writeln("\nsimple case");
  var x: [rcDomain] real;
  writeln("\ninitially\n", x);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate\n", x);
  on (if numLocales >= 3
      then rootLocale.getLocale(2)
        else rootLocale.getLocale(0)) do
    x[1] = 33;
  writeln("\nafter 'on'\n", x);
  var c: [rootLocale.getLocaleSpace()] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}

if numLocales >= 4 {
  var myL = rootLocale.getLocale(1..3 by 2);
//  myL[1] = myL[2];  // this makes myL violate the "consistency" requirement
  writeln("\nadvanced case: ", myL);
  var x: [rcDomainBase dmapped ReplicatedDist(myL)] real;
  writeln("\ninitially\n", x);
  rcReplicate(x, 5);
  writeln("\nafter rcReplicate\n", x);
  on rootLocale.getLocale(3) do
    x[1] = 33;
  writeln("\nafter 'on'\n", x);
  var c: [myL.domain] real;
  rcCollect(x, c);
  writeln("\ncollected:\n", c);
}
