

function runea() {
  writeln("in runea() from module e");
}

module e2 {
  function runea() {
    writeln("in runea() from module e2");
  }

  function runeb() {
    writeln("in runeb() from module e2");
  }
}


function runeb() {
  writeln("in runeb() from module e");
}
