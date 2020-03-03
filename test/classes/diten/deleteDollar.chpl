class c$ {
  proc writeme() {
    writeln("Hi from $");
  }
  proc deinit() {
    writeln("Destroy dollar");
  }
}

proc main {
  var c$$ = new unmanaged c$();
  c$$.writeme();
  delete c$$;
}
