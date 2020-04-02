
var globDmn = {1..3};
var globAry = [111, 222, 333];

record RRR {
  var DDD;
  var ARR: [DDD] int = 5;
}

proc procp(type TTT) {
  var VVV: TTT;
  writeln(VVV);
}

record QQQ {
  var ARR;
}

proc procq(type UUU) {
  var WW: UUU;
  writeln(WW);
  writeln(WW.ARR.size);
}

proc main {
  var MMM = new RRR(globDmn);
  writeln(MMM);
  procp(MMM.type);
  procp(RRR(domain(1)));

  var NNN = new QQQ(globAry);
  writeln(NNN);
  procq(NNN.type);
  procq(QQQ(globAry.type));
}
