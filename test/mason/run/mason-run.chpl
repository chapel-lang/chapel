use MasonRun;

proc main() {
  const args: [0..3] string = ["run", "--build", "--force", "--no-checksum"];
  masonRun(args);
}
