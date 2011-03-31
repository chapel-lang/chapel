use Time;

class C {
  var x : volatile int;
}

var myC = new C();

begin {
  while (myC.x==0) {};
  writeln(myC.x);
}
sleep(1);
myC.x=1;
