use Time;

config var n: int = 5;

var A: [1..n] int;

class syncStack {
  var v: sync bool;
  var next: syncStack;
}

proc pushSyncStack(s: syncStack) return new syncStack(next=s);

var ss: syncStack;

for i in 1..n {
  var me = pushSyncStack(ss);
  begin {
    sleep(i:uint);
    A(i) = i;
    me.v = true;
  }
  ss = me;
}

var ptr0 = ss;

while ptr0 != nil {
  ptr0.v.readFE();
  ptr0 = ptr0.next;
}

writeln(A);
sleep(2);
writeln(A);


var ptr1 = ss;

while ptr1 != nil {
  var p = ptr1;

  ptr1 = ptr1.next;

  delete p;
}
