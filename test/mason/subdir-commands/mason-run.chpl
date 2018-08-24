use MasonRun;

proc main() {
  const args: [0..3] string = ["mason", "run", "--build", "--force"];
  masonRun(args);
}
