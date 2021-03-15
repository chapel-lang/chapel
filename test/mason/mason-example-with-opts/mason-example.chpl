use MasonExample;
use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["mason", "--build", "--example", "--force", "--no-checksum"]);

  // run each example
  // over 3 arguments runs all examples
  var runArgs: [0..4] string = ["mason", "run", "--example", "--force", "--no-checksum"];
  masonRun(runArgs);

}
