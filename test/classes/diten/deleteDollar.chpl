class c$ {
  def writeme() {
    writeln("Hi from $");
  }
  def ~c$() {
    writeln("Destroy dollar");
  }
}

def main {
  var c$$ = new c$();
  c$$.writeme();
  delete c$$;
}
