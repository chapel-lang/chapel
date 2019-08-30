// writing to an array field of a 'var'

const RNG = 1..4;

var s$: sync int;

record QQ {
  var data: [RNG] int;
}

proc QQ.w1(factor: int) {
  coforall i in RNG {
    data[i] = i * factor;
  }
}

proc QQ.w2() {
  w1(100);
}

proc QQ.w3() {
  coforall i in RNG {
    data[i] = i;
  }
  w2();
}

proc QQ.r1(factor: int) {
  coforall i in RNG {
    writeln(data[i] == i * factor);
  }
}

var rec1 = new QQ();
rec1.w1(10);
rec1.r1(10);
writeln();

var rec2 = new QQ();
rec2.w3();
rec2.r1(100);

proc QQ.bgn() {
  begin {
    data[2] = 234;
    s$ = 1;
  }
}

var rec3 = new QQ();
rec3.bgn();
s$;
writeln(rec3);

proc QQ.cob() {
  cobegin {
    data[1] = 123;
    data[3] = 321;
  }
}

rec3.cob();
writeln(rec3);
