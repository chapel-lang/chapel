

def runga() {
  writeln("in runga() from module g");
}

module g2 {
  def runga() {
    writeln("in runga() from module g2");
  }

  def rungb() {
    writeln("in rungb() from module g3");
  }
}

module g3 {
  def runga() {
    writeln("in runga() from module g2");
  }

  def rungb() {
    writeln("in rungb() from module g3");
  }
}


def rungb() {
  writeln("in rungb() from module g");
}
