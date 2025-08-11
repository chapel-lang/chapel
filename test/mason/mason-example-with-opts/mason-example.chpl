use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["build", "--example", "--force"]);

  // run each example
  // over 3 arguments runs all examples
  var runArgs = ["run", "--example", "--force"];
  masonRun(runArgs);

}
