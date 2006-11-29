use Time;

var s: single int;

begin {
  var t: int;
  writeln( "thread2: Hello?");
  t = s;  // should wait here for thread1's write of s
  writeln( "thread2: Wake up!");
}

sleep( 3);
writeln( "thread1: zzzzzzzzz");
sleep( 2);
s = 1;
