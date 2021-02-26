private use List;
use MasonTest;

proc main(args: [] string) {
  var masonArgs = ["mason", "--recursive", args[1]];
  masonTest(masonArgs);
}
