use List;
use MasonSearch;
import MasonLogger;

proc main() {
  MasonLogger.setNoColor(true);
  masonSearch(["search", "--no-update"]);
}
