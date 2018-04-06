/*
 * Copyright 2004-2018 Cray Inc.
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

  const debug = hasOptions(args, "--debug");

  updateRegistry("", args);

  consumeArgs(args);

  // If no query is provided, list all packages in registry
  const query = if args.size > 0 then args.tail().toLower()
                else ".*";
  const pattern = compile(query, ignorecase=true);

  var results : [1..0] string;

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
          results.push_back(name + " (" + ver.str() + ")");
        }
      }
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
