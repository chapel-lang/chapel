use MasonExample;
use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["mason", "--build", "--example", "--force"]);

  // run each example
  // over 3 arguments runs all examples
  var runArgs: [0..3] string = ["mason", "run", "--example", "--force"];
  masonRun(runArgs);

}
