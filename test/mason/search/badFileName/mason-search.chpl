use List;
use MasonSearch;

proc main() {
  var args: list(string);
  for x in ["search", "--no-update"] do args.pushBack(x);
  masonSearch(args);
}
