use MasonTest;


proc main() {
  const args = ["test" , "test/test1.chpl", "test/testDir", "--no-checksum"];
  masonTest(args);
}
