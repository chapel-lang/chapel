use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["build", "--example", "--force"]);

  // run each example
  // over 3 arguments runs all examples
  var runArgs: [0..2] string = ["run", "--example", "--force"];
  masonRun(runArgs);

}
