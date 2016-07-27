config const reps = 9223372036854775807;
proc main(){
  param x = 2;
  var k : int(64);
  for a in 1..reps{
      for b in 1 .. a {
        k = a % 50;
        k = k + 11;
        k = x ** k;
        if k < 5 then writeln("less than 5");
    }
  }
}
