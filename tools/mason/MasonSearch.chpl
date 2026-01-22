/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
use Map;

//
// TODO:
// - order results by some kind of 'best match' metric
// - allow for exclusion of a pattern
//



proc masonSearch(args: [] string): int throws {

  var parser = new argumentParser(helpHandler=new MasonSearchHelpHandler());

  // If no query is provided, list all packages in registry
  var queryArg = parser.addArgument(name="query",
                                    numArgs=0..1,
                                    defaultValue=".*");

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  parser.parseArgs(args);

  const show = showFlag.valueAsBool();
  var skipUpdate = MASON_OFFLINE;
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }

  updateRegistry(skipUpdate);

  const query = queryArg.value();
  const pattern = new regex(query, ignoreCase=true);
  var pkgs = searchDependencies(pattern);

  for package in pkgs {
    writeln(package.name + "@" + package.version.str());
  }

  // Handle --show flag
  if show {
    if pkgs.size == 1 {
      const pkg = pkgs[0];
      writeln('Displaying the latest version: ' +
              pkg.name + '@' + pkg.version.str());
      const brickPath = joinPath(pkg.registry, 'Bricks',
                                 pkg.name, pkg.version.str() + '.toml');
      showToml(brickPath);
    } else if pkgs.size == 0 {
      throw new MasonError('"%s" returned no packages\n'.format(query) +
                           '--show requires the search to return one package');
    } else if pkgs.size > 1 {
      var msg = '';
      if query == '.*' {
        msg += 'You must specify a package to show the manifest\n';
      } else {
        msg += '"%s"  returned more than one package.\n'.format(query);
      }
      msg += "--show requires the search to return one package.";
      throw new MasonError(msg);
    }
  }

  return if pkgs.size == 0 then 1 else 0;
}

