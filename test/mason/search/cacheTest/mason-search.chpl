use List;
use MasonSearch;

proc main() {
  var args: list(string);
  args.append("search");
  args.append("--no-update");
  masonSearch(args);
}
