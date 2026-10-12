use MasonRun;

proc main() {
  const args: [0..2] string = ["run", "--build", "--force"];
  masonRun(args);
}
