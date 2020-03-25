proc newline() {
  writeln();
}

proc world() {
  write(" world!");
}

proc hello() {
  write("Hello");
}

proc print_helloworld() {
  hello();
  world();
  newline();
}

print_helloworld();
