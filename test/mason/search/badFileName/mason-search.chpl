use MasonSearch;

use FileSystem;

proc main() {
  var args: [0..2] string = ["foo", "search", "--no-update"];
  masonSearch(args);
}
