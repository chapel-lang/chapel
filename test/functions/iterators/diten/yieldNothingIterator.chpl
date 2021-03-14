iter yieldNothing() {
  writeln("In yieldNothing");
}

proc main {
  for v in yieldNothing() {
    /* should not print the loop body's message at all, but should still
       print the iterator's message */
    writeln("Got a void!");
  }
}
