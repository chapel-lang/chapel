config const reps = 20000000;
proc main(){
  param x = 1;
  var k : int(64);
  var sum = 0;
  for a in 1..reps{
        k = a % 30;
        k = k + 31;
        sum += k;
        k = x << k;
        if k < 5 then writeln("less than 5");
  }
  writeln("DONE");
}
