// writing to an array field of a 'const'

const RNG = 1..4;

var s$: sync int;

record QQ {
  var data: [RNG] int;
}

var rec3 = new QQ();

proc const QQ.bgn() {
  begin {
    data[2] = 234;  // illegal because it is a field of a const
    s$.writeEF(1);
  }
}

rec3.bgn();
s$.readFE();
writeln(rec3);

proc const QQ.cob() {
  cobegin {
    data[1] = 123;  // illegal
    data[3] = 321;  // illegal
  }
}

rec3.cob();
writeln(rec3);

// writing to a const array field

record RR {
  const data: [RNG] int;
}

var rec4 = new RR();

proc RR.bgn() {
  begin {
    data[2] = 234;  // illegal because it is a const field
    s$.writeEF(1);
  }
}

rec4.bgn();
s$.readFE();
writeln(rec4);

proc RR.cob() {
  cobegin {
    data[1] = 123;  // illegal
    data[3] = 321;  // illegal
  }
}

rec4.cob();
writeln(rec4);

