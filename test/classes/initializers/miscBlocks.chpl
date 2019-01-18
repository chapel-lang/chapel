
class Foo {
  proc init() {
    coforall i in 1..here.maxTaskPar do writeln("Hello from task ", i);
    coforall loc in Locales do on loc do writeln("Hello from ", here);
  }
}
