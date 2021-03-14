use samples;

proc run() {
  writeln(one); // this ought not to copy the record
}

run();
