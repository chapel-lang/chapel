private use List;
use MasonTest;

proc main(args: [] string) {
  var masonArgs = ["test", "--recursive", args[1]];
  masonTest(masonArgs);
}
