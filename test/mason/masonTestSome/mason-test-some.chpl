use MasonTest;




proc main() {
  const args = ["mason", "test" , "test/test1.chpl", "test/testDir", "--no-checksum"];
  masonTest(args);
}
