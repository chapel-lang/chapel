class c$ {
  proc writeme() {
    writeln("Hi from $");
  }
  proc ~c$() {
    writeln("Destroy dollar");
  }
}

proc main {
  var c$$ = new c$();
  c$$.writeme();
  delete c$$;
}
