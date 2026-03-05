use List;
use MasonSearch;
use MasonUtils;
import MasonLogger;

config const pattern = "";
config const debug = false;

proc main() {
  var args: list(string);
  args.pushBack("search");
  args.pushBack("--no-update");
  for arg in pattern.split() {
    if arg != "" then args.pushBack(arg);
  }

  if debug {
    MasonLogger.logs = MasonLogger.logLevel.debug;
    MasonLogger.setUseColorOutput(false);
  }

  try! {
    masonSearch(args.toArray());
  } catch e: MasonError {
    writeln(e.message());
  }

}
