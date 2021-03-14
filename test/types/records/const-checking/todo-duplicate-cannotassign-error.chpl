// The assignment should generate one error, not three.

record REC {
  const F1, F2, F3: int;
}

var RR = new REC();
var QQ = new REC();
RR = QQ;
