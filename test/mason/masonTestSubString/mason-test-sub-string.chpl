use MasonTest;

proc main() {
  const args = ["test" , "1", "3", "--no-update"];
  masonTest(args, checkProj=false);
}
