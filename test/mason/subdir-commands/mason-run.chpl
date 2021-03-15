use MasonRun;

proc main() {
  const args: [0..4] string = ["mason", "run", "--build", "--force", "--no-checksum"];
  masonRun(args);
}
