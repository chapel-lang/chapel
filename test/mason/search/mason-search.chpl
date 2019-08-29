
private use List;
use MasonSearch;

use FileSystem;

config const pattern = "";

proc main() {
  var args: list(string);
  args.append("foo");
  args.append("search");
  args.append("--no-update");
  for arg in pattern.split() {
    if arg != "" then args.append(arg);
  }


  masonSearch(args);
}
