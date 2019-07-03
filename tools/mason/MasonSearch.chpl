/*
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  const show = hasOptions(args, "--show");
  const debug = hasOptions(args, "--debug");

  updateRegistry("", args);

  consumeArgs(args);

  // If no query is provided, list all packages in registry
  const query = if args.size > 0 then args.tail().toLower()
                else ".*";
  const pattern = compile(query, ignorecase=true);

  var results : [1..0] string;
  var showResults : [1..0] string;
  var versionToReturn : [1..0] string;

  for cached in MASON_CACHED_REGISTRY {
    const searchDir = cached + "/Bricks/";

    for dir in listdir(searchDir, files=false, dirs=true) {
      const name = dir.replace("/", "");

      if pattern.search(name) {
        if isHidden(name) {
          if debug {
            writeln("[DEBUG] found hidden package: ", name);
          }
        }  else {
          const ver = findLatest(searchDir + dir);
          const versionZero = new VersionInfo(0, 0, 0);
          if ver != versionZero then
            results.push_back(name + " (" + ver.str() + ")");
            showResults.push_back(name);
            versionToReturn.push_back(ver.str());
        }
      }
    }
  }

  for r in results.sorted() do writeln(r);

  if results.size == 1 && show {
    writeln('Displaying the latest version: ' + showResults + '@' + versionToReturn);
    showToml(versionToReturn + '.toml', showResults);
  }

  if results.size == 0 && show {
  writeln('"' + query + '" returned no packages');
  writeln('--show requires the search to return one package');
  }

  if show && results.size > 1 {
    if query == '.*' {
      writeln('You must specify a package to show the manifest');
    }
    else {
      writeln('"' + query + '"  returned more than one package.');
    }
  writeln("--show requires the search to return one package");
  }

  if results.size == 0 {
    exit(1);
  }
}

proc isHidden(name : string) : bool {
  return name.startsWith("_");
}

/* Search TOML files within a package directory to find the latest package
   version number that is supported with current Chapel version */
proc findLatest(packageDir: string): VersionInfo {
  var ret = new VersionInfo(0, 0, 0);
  const suffix = ".toml";
  const packageName = basename(packageDir);
  for manifest in listdir(packageDir, files=true, dirs=false) {
    // Check that it is a valid TOML file
    if !manifest.endsWith(suffix) {
      var warningStr = "File without '.toml' extension encountered - skipping ";
      warningStr += packageName + " " + manifest;
      stderr.writeln(warningStr);
      continue;
    }

    // Skip packages that are out of version bounds
    const chplVersion = getChapelVersionInfo();

    const manifestReader = openreader(packageDir + '/' + manifest);
    const manifestToml = new owned(parseToml(manifestReader));
    const brick = manifestToml['brick'];
    var (low, high) = parseChplVersion(brick);
    if chplVersion < low || chplVersion > high then continue;

    // Check that Chapel version is supported
    const end = manifest.length - suffix.length;
    const ver = new VersionInfo(manifest[1..end]);
    if ver > ret then ret = ver;
  }
  return ret;
}

proc consumeArgs(ref args : [] string) {
  args.pop_front(); // binary name
  const sub = args.head(); // 'search'
  assert(sub == "search");
  args.pop_front();

  const options = {"--no-update", "--debug", "--show"};

  while args.size > 0 && options.contains(args.head()) {
    args.pop_front();
  }
}

proc showToml(tomlFile : string, packageName : string) {
  const brickPath = MASON_HOME +'/mason-registry/Bricks/' +packageName + '/' + tomlFile;
  const openFile = openreader(brickPath);
  const toml = parseToml(openFile);
  writeln(toml);
  exit(0);
}
