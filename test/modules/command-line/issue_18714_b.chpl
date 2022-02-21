module issue_18714_b {
  writeln("in b");

  proc deinit() {
    writeln("in b.deinit");
  }
}
