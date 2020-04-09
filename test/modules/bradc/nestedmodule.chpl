module a {

  proc runa() {
    writeln("in runa() in module a");
  }

  module aSubB {

    proc runab() {
      writeln("in runab() in module aSubB");
    }

    proc main() {
      import a.runa;
      runa();
      runab();
    }  
  }
}

