private use List;
use MasonSearch;

use FileSystem;

proc main() {
  var args: list(string);
  for x in ["foo", "search", "--no-update"] do args.append(x);
  masonSearch(args);
}
