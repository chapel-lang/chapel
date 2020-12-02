export proc display(in msg: string) {
  writeln(msg);
}

proc deinit() {
  writeln('Called module deinitializer!');
}

