use Time;

var s: single int(64);

begin {
  var t: int(64);
  writeln( "thread2: Hello?");
  t = s;  // should wait here for thread1's write of s
  writeln( "thread2: Wake up!");
}

sleep( 3);
writeln( "thread1: zzzzzzzzz");
sleep( 2);
s = 1;
