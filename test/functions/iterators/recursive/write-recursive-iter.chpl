iter a(n:int):int {
  yield n;
  if n!=1 then {
    for b in a(n-1) do  //replacing with for b in 1..(n-1) by -1 works
      yield b;
  }
}

writeln(a(10)); //Causes error
