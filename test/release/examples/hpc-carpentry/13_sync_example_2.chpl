var x: sync int, a: int;
writeln("This is the main task launching a new task");
begin {
  for i in 1..10 do writeln("This is new task working: ",i);
  x = 2;
  writeln("New task finished");
}

writeln("This is the main task after launching new task... I will wait until it is done");
a = x;  // don't run this line until the variable x is written in another task
writeln("and now it is done");
