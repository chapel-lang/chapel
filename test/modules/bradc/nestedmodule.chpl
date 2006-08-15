module a {

  def runa() {
    writeln("in runa() in module a");
  }

  module aSubB {

    def runab() {
      writeln("in runab() in module aSubB");
    }

    def main() {
      runa();
      runab();
    }  
  }
}

