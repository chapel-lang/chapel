

fun runga() {
  writeln("in runga() from module g");
}

module g2 {
  fun runga() {
    writeln("in runga() from module g2");
  }

  fun rungb() {
    writeln("in rungb() from module g3");
  }
}

module g3 {
  fun runga() {
    writeln("in runga() from module g2");
  }

  fun rungb() {
    writeln("in rungb() from module g3");
  }
}


fun rungb() {
  writeln("in rungb() from module g");
}
