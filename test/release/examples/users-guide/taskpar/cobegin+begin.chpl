cobegin {
  writeln("Hi!");
  printHellos();
}
writeln("Sorry, I've gotta leave...");

proc printHellos() {
  writeln("Beginning...");
  begin {
    for i in 1..10 do
      writeln("Hello!");
  }
  writeln("I'm moving on...");
}
