record man {
  proc enterThis(): int { return 0; }
  proc leaveThis(in err: owned Error?) {
    if err then try! { throw err; }
  }
}

proc test() {
  manage new man() as num do
    writeln(num);
}
test();

