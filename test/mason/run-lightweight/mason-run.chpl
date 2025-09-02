use MasonRun;

proc main() {
  const args = ["run", "--build", "--force", "--example", "ex.chpl"];
  masonRun(args);
}
