for param i in 1..5 {
  sync {
    begin {
      write(i);
    }
  }
}
writeln();
