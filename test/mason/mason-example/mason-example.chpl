use MasonExample;
use MasonBuild;
use MasonRun;

proc main() {

  // build the examples
  masonExample(["--no-run"]);

  // run info list of tests to run
  masonRun(["mason", "run", "--example"]);

  // run each example
  masonExample(["depExample.chpl", "nomodule.chpl", "examplesubdir/subdirExample.chpl",
                "example.chpl", "--no-build"]);

}
