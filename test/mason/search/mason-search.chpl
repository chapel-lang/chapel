
use MasonSearch;

use FileSystem;

config const pattern = "";

proc main() {
  var args = ["foo", "search", "--no-update-registry"];
  if pattern != "" then args.push_back(pattern);


  masonSearch(args);
}
