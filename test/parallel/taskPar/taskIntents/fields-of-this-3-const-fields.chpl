// writing to a const array field of a 'var'
// and to fields passed by const intents

const RNG = 1..4;

record QQ {
  const xxx: [RNG] int;
  var   iii: int;
  var   ddd: domain(1);
}

proc modify(ref arg: domain(1)) {}

proc QQ.w1(factor: int) {
  coforall i in RNG {
    xxx[i] = i * factor;  // illegal because the field is const
    iii = 5;              // illegal because the field is passed by 'const' intent
    modify(ddd);          // ditto
  }
}

proc QQ.w2() {
  w1(100);
}

proc QQ.w3() {
  w2();
}

proc QQ.r1(factor: int) {
  coforall i in RNG {
    writeln(xxx[i] == i * factor);
    writeln(iii);
    writeln(ddd);
  }
}

var rec2 = new QQ();
rec2.w3();
rec2.r1(100);
