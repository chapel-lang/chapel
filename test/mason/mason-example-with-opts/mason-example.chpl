use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["--build", "--example", "--force", "--no-checksum"]);

  // run each example
  // over 3 arguments runs all examples
  var runArgs: [0..3] string = ["run", "--example", "--force", "--no-checksum"];
  masonRun(runArgs);

}
