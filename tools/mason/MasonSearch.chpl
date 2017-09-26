
use MasonHelp;
use MasonEnv;
use MasonUpdate;

use FileSystem;
use Regexp;

//
// TODO:
// - order results by some kind of 'best match' metric
// - allow regex searches
// - allow for exclusion of a pattern
//

proc masonSearch(origArgs : [] string) {
  var args : [1..origArgs.size] string = origArgs;

  if hasOptions(args, "-h", "--help") {
    masonSearchHelp();
    exit(0);
  }

  const debug = hasOptions(args, "--debug");

  updateRegistry("", args);

  consumeArgs(args);

  // If no query is provided, list all packages in registry
  const query = if args.size > 0 then args.tail().toLower()
                else ".*";

  const searchDir = MASON_CACHED_REGISTRY + "/Bricks/";
  const pattern   = compile(query, ignorecase=true);

  var results : [1..0] string;
  for dir in listdir(searchDir, files=false, dirs=true) {
    const name = dir.replace("/", "");

    if isHidden(name) {
      if debug {
        writeln("[DEBUG] found hidden package: ", name);
      }
    }
    else if pattern.search(name) {
      const ver = findLatest(searchDir + dir);
      results.push_back(name + " (" + ver.str() + ")");
    }
  }
  for r in results.sorted() do writeln(r);

  if results.size == 0 {
    exit(1);
  }
}

proc isHidden(name : string) : bool {
  return name.startsWith("_");
}

proc findLatest(packageDir) {
  var ret = new VersionInfo(0, 0, 0);
  const suffix = ".toml";

  for fi in listdir(packageDir, files=true, dirs=false) {
    assert(fi.endsWith(suffix));
    const end = fi.length - suffix.length;
    const ver = new VersionInfo(fi[1..end]);

    if ver > ret then ret = ver;
  }

  return ret;
}

proc consumeArgs(ref args : [] string) {
  args.pop_front(); // binary name
  const sub = args.head(); // 'search'
  assert(sub == "search");
  args.pop_front();

  const options = {"--no-update-registry", "--debug"};

  while args.size > 0 && options.member(args.head()) {
    args.pop_front();
  }
}
