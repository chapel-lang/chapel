use MasonExample;
use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonExample(["--no-run", "--force"]);

  // run info list of tests to run
  masonRun(["mason", "run", "--example"]);

  // run each example
  masonExample(["depExample.chpl", "withOpts.chpl", "examplesubdir/subdirExample.chpl",
                "example.chpl", "--no-build"]);

}
