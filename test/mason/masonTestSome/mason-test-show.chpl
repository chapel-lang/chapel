use MasonTest;

proc main() {

  const args = ["test", "--show", "test/compilererror.chpl"];
  masonTest(args);
}
