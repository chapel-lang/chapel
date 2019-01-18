config var n = 10;

proc run() {
  var RD: domain(3) = {1..n, 1..n, 1..n};
  writeln(RD);
  var SSD: sparse subdomain(RD);

  writeln("SSD:", SSD); // Initially empty.

  writeln("Adding (1,2,3) to SSD");
  SSD += (1,2,3);

  writeln("SSD:", SSD);

  writeln("Setting RD to 1..1, 1..1, 1..1");
  RD = {1..1, 1..1, 1..1};
  writeln("SSD:", SSD);
  writeln("SSD parent ", SSD._value.parentDom);

  writeln("Adding (4,4,4) to SSD");
  SSD += (4,4,4);
  writeln("SSD:", SSD);
}

run();
