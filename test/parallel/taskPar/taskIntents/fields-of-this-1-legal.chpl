// writing to an array field of a 'var'

const RNG = 1..4;

var s$: sync int;

class CT {
  var fieldd: int;
}

record QQ {
  var data: [RNG] int;
  var number: int;
  var pointer = new owned CT();

  proc init(tag:int) {
    number = tag;
    pointer = new owned CT(-tag);
  }
}

proc QQ.w1(factor: int) {
  coforall i in RNG {
    data[i] = i * factor;
    writeln(number, pointer.fieldd);
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

var rec1 = new QQ(1);
rec1.w1(10);
rec1.r1(10);
writeln();

var rec2 = new QQ(2);
rec2.w3();
rec2.r1(100);
writeln();

proc QQ.bgn() {
  begin {
    data[2] = 234;
    writeln(number, pointer.fieldd);
    s$ = 1;
  }
}

var rec3 = new QQ(3);
rec3.bgn();
s$;
writeln(rec3);
writeln();

proc QQ.cob() {
  cobegin {
    data[1] = 123;
    data[3] = 321;
    writeln(number, pointer.fieldd);
  }
}

rec3.cob();
writeln(rec3);
