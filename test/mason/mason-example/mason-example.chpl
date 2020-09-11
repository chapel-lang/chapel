use MasonExample;
use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["mason", "--build", "--example", "--force"]);

  // run each example
  // over 3 arguments runs all examples
  masonRun(["mason", "run", "--example", "--force", "--no-update"]);

}
