

function runga() {
  writeln("in runga() from module g");
}

module g2 {
  function runga() {
    writeln("in runga() from module g2");
  }

  function rungb() {
    writeln("in rungb() from module g3");
  }
}

module g3 {
  function runga() {
    writeln("in runga() from module g2");
  }

  function rungb() {
    writeln("in rungb() from module g3");
  }
}


function rungb() {
  writeln("in rungb() from module g");
}
