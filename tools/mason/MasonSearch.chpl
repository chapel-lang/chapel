/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

use ArgumentParser;
use FileSystem;
use IO;
use List;
use MasonEnv;
use MasonHelp;
use MasonUpdate;
use MasonUtils;
use Regex;
use Sort;
use TOML;

//
// TODO:
// - order results by some kind of 'best match' metric
// - allow regex searches
// - allow for exclusion of a pattern
//

//
// Temporary passthrough transforming array to list to appease the compiler.
//
proc masonSearch(args: [?d] string) {
  var listArgs: list(string);
  for x in args do listArgs.pushBack(x);
  masonSearch(listArgs);
}

proc masonSearch(ref args: list(string)) {

  var parser = new argumentParser(helpHandler=new MasonSearchHelpHandler());

  // If no query is provided, list all packages in registry
  var queryArg = parser.addArgument(name="query",
                                    numArgs=0..1,
                                    defaultValue=".*");

  // TODO: Is the debug flag supposed to be documented in help or not?
  var debugFlag = parser.addFlag("debug", defaultValue=false);
  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  parser.parseArgs(args.toArray());

  const show = showFlag.valueAsBool();
  const debug = debugFlag.valueAsBool();
  var skipUpdate = MASON_OFFLINE;
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }

  updateRegistry(skipUpdate);

  const query = queryArg.value().toLower();
  const pattern = new regex(query, ignoreCase=true);
  var results: list(string);
  var packages: list(string);
  var versions: list(string);
  var registries: list(string);
  for registry in MASON_CACHED_REGISTRY {
    const searchDir = registry + "/Bricks/";

    for dir in listDir(searchDir, files=false, dirs=true) {
      const name = dir.replace("/", "");
      if pattern.search(name) {
        if isHidden(name) {
          if debug {
            writeln("[DEBUG] found hidden package: ", name);
          }
        } else {
          const ver = findLatest(searchDir + dir);
          const versionZero = new VersionInfo(0, 0, 0);
          if ver != versionZero {
            results.pushBack(name + " (" + ver.str() + ")");
            packages.pushBack(name);
            versions.pushBack(ver.str());
            registries.pushBack(registry);
          }
        }
      }
    }
  }

  var res = rankResults(results, query);
  for package in res {
    const pkgName = splitNameVersion(package, true);
    writeln(pkgName);
  }

  // Handle --show flag
  if show {
    if results.size == 1 {
      writeln('Displaying the latest version: ' + packages[0] + '@' + versions[0]);
      const brickPath = '/'.join(registries[0], 'Bricks', packages[0], versions[0]) + '.toml';
      showToml(brickPath);
      exit(0);
    } else if results.size == 0 {
      writeln('"' + query + '" returned no packages');
      writeln('--show requires the search to return one package');
      exit(1);
    } else if results.size > 1 {
      if query == '.*' {
        writeln('You must specify a package to show the manifest');
      } else {
        writeln('"' + query + '"  returned more than one package.');
      }
      writeln("--show requires the search to return one package.");
      exit(1);
    }
  }

  if results.size == 0 {
    exit(1);
  }
}

record RankResultsComparator {
  var query: string;
  var packageScores;
  proc compare(a, b) {
    // starting with the query puts it first, if query is not ""
    if query != "" {
      if a.toLower().startsWith(query) &&
         !b.toLower().startsWith(query) {
        return -1;
      } else if !a.toLower().startsWith(query) &&
                b.toLower().startsWith(query) {
        return 1;
      }
    }

    // then sort by score
    var scoreA = packageScores[a];
    var scoreB = packageScores[b];
    if scoreA > scoreB {
      return -1;
    } else if scoreA < scoreB {
      return 1;
    }

    // then sort by name
    if a < b {
      return -1;
    } else if a > b {
      return 1;
    }

    // consider them the same
    return 0;
  }
}

/* Sort the results in a order such that results that startWith needle
   are displayed first */
proc rankResults(results: list(string), query: string): [] string {
  use Sort;
  var r = results.toArray();
  var q = query;
  if query == ".*" then
    q = "";

  var cmp = new RankResultsComparator(q, getPackageScores(r));
  sort(r, comparator=cmp);
  return r;
}

/* Creates an empty cache file if its not found in registry */
proc touch(pathToReg: string) {
  const fileWriter = open(pathToReg, ioMode.cw).writer();
  fileWriter.write("");
  fileWriter.close();
}

/* Returns a map of packages found in cache along with their scores */
proc getPackageScores(res: [] string) {
  use Map;
  const pathToReg = MASON_HOME + "/mason-registry/cache.toml";
  var cacheExists = false;
  if isFile(pathToReg) then cacheExists = true;
  if !cacheExists then touch(pathToReg);
  const parse = open(pathToReg, ioMode.r);
  const cacheFile = parseToml(parse);
  var packageScores: map(string, int);
  var packageName: string;
  // defaultScore = (8/12 * 100) = 66
  const defaultScore = 66;
  var packageScore: int;
  for r in res {
    r = splitNameVersion(r, false);
    packageName = r;
    if cacheExists && cacheFile.pathExists(packageName) {
      packageScore = cacheFile[r]!['score']!.s : int;
      packageScores.add(packageName, packageScore);
    } else packageScores.add(packageName, defaultScore);
  }
  return packageScores;
}

proc isHidden(name : string) : bool {
  return name.startsWith("_");
}
