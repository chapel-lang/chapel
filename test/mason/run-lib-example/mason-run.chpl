use MasonRun;

config const testcase = 0;

proc main() {
  if testcase == 0 {
    masonRun(["run", "--build", "--force", "--example", "ex.chpl"]);
  } else if testcase == 1 {
    try {
      masonRun(["run", "--build", "--force"]); // should error, not an application
    } catch e {
      writeln(e.message());
    }
  }
}
