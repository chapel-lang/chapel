
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

  const printHelp = || reduce [a in args] (a == "-h" || a == "--help");
  if printHelp {
    masonSearchHelp();
    exit(0);
  }

  updateRegistry("", args);

  consumeArgs(args);

  // If no query is provided, list all packages in registry
  const query = if args.size > 0 then args.tail().toLower()
                else ".*";

  const searchDir = MasonUpdate.getRegistryDir() + "/Bricks/";
  const pattern   = compile(query, ignorecase=true);

  var numFound = 0;
  for dir in listdir(searchDir, files=false, dirs=true) {
    if pattern.search(dir) {
      numFound += 1;

      const pkg = dir.replace("/", "");
      const ver = findLatest(searchDir + dir);

      writeln(pkg, " (", ver.str(), ")");
    }
  }

  if numFound == 0 {
    exit(1);
  }
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

  const options = {"--no-update-registry"};

  while args.size > 0 && options.member(args.head()) {
    args.pop_front();
  }
}
