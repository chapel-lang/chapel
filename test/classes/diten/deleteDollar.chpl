class c$ {
  proc writeme() {
    writeln("Hi from $");
  }
  proc ~c$() {
    writeln("Destroy dollar");
  }
}

proc main {
  var c$$ = new unmanaged c$();
  c$$.writeme();
  delete c$$;
}
