use List;
use MasonSearch;
import MasonLogger;

proc main() {
  MasonLogger.setUseColorOutput(false);
  masonSearch(["search", "--no-update"]);
}
