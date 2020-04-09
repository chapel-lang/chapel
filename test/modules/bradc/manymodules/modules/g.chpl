

proc runga() {
  writeln("in runga() from module g");
}

module g2 {
  proc runga() {
    writeln("in runga() from module g2");
  }

  proc rungb() {
    writeln("in rungb() from module g3");
  }
}

module g3 {
  proc runga() {
    writeln("in runga() from module g2");
  }

  proc rungb() {
    writeln("in rungb() from module g3");
  }
}


proc rungb() {
  writeln("in rungb() from module g");
}
