module Library {
}

module Main {
  var e: [1..8] int = 1..8;
  {
    use Library;

    writeln(e[1]);
  }
}
