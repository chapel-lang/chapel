cobegin {
  writeln("Hi!");
  sayHello();
  {
    writeln("...Greetings...");
    writeln("...puny earthling...");
  }
}
writeln("See you later!");

proc sayHello() {
  writeln("Hello!");
}
