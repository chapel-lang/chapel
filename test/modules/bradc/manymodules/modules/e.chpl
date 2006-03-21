

fun runea() {
  writeln("in runea() from module e");
}

module e2 {
  fun runea() {
    writeln("in runea() from module e2");
  }

  fun runeb() {
    writeln("in runeb() from module e2");
  }
}


fun runeb() {
  writeln("in runeb() from module e");
}
