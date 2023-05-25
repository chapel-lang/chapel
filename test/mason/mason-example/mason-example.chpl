use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonBuild(["build", "--example", "--force"], checkProj=false);

  // run each example
  // over 3 arguments runs all examples
  masonRun(["run", "--example", "--force", "--no-update"], checkProj=false);

}
