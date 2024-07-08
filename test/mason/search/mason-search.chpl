use List;
use MasonSearch;

config const pattern = "";

proc main() {
  var args: list(string);
  args.pushBack("search");
  args.pushBack("--no-update");
  for arg in pattern.split() {
    if arg != "" then args.pushBack(arg);
  }

  masonSearch(args);
}
