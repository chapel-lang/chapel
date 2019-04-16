
use MasonSearch;

use FileSystem;

config const pattern = "";

proc main() {
  var args: [0..2] string = ["foo", "search", "--no-update"];
  for arg in pattern.split() {
    if arg != "" then args.push_back(arg);
  }


  masonSearch(args);
}
