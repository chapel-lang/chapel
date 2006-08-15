

def runea() {
  writeln("in runea() from module e");
}

module e2 {
  def runea() {
    writeln("in runea() from module e2");
  }

  def runeb() {
    writeln("in runeb() from module e2");
  }
}


def runeb() {
  writeln("in runeb() from module e");
}
