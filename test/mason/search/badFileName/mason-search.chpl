
use MasonSearch;

use FileSystem;

config const pattern = "";

proc main() {
  var args: [0..2] string = ["foo", "search", "--no-update"];
  if pattern != "" then args.push_back(pattern);


  masonSearch(args);
}
