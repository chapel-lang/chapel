module M1 {
  config const name = "v1";

  writeln(name);
}

module M2 {
  use M1;
  
  config const name = "v1";

  proc main() {
    writeln(name);
  }
}

