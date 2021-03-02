use Time;

var s: single int;

begin {
  var t: int;
  writeln( "thread2: Hello?");
  t = s.readFF();  // should wait here for thread1's write of s
  writeln( "thread2: Wake up!");
}

sleep( 3);
writeln( "thread1: zzzzzzzzz");
sleep( 2);
s.writeEF(1);
