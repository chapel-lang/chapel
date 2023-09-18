use List;
use MasonSearch;

proc main() {
  var args: list(string);
  args.pushBack("search");
  args.pushBack("--no-update");
  masonSearch(args);
}
