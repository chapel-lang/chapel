class $ {
  def writeme() {
    writeln("Hi from $");
  }
  def ~$() {
    writeln("Destroy dollar");
  }
}

def main {
  var $$ = new $();
  $$.writeme();
  delete $$;
}
