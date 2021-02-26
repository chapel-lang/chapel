use MasonSearch;
use List;
use FileSystem;

proc main() {
  var args: list(string);
  args.append("mason");
  args.append("search");
  args.append("--no-update");
  masonSearch(args);
}

