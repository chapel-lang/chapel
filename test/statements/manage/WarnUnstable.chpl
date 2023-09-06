record man : contextManager {
  proc enterContext(): int { return 0; }
  proc exitContext(in err: owned Error?) {
    if err then try! { throw err; }
  }
}

proc test() {
  manage new man() as num do
    writeln(num);
}
test();

