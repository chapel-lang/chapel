// writing to an array field of a 'const'

const RNG = 1..4;

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

const rec1 = new QQ();
rec1.w1(10);   // illegal because w1() takes the receiver by ref 
rec1.r1(10);
writeln();

const rec2 = new QQ();
rec2.w3();     // illegal because w3() takes the receiver by ref 
rec2.r1(100);
