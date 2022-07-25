/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
use List;
use Map;
use MasonUtils;
use MasonHelp;
use MasonEnv;
use MasonBuild;
use Subprocess;
use TOML;

proc masonModules(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonLightHelpHandler());

  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);

  var skipUpdate = MASON_OFFLINE;

  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }

  if isDir(MASON_HOME) == false {
    mkdir(MASON_HOME, parents=true);
  }
  const configNames = updateLock(skipUpdate, show=false);
  const tomlName = configNames[0];
  const lockName = configNames[1];

  const cwd = here.cwd();
  const toParse = open(cwd + "/" + lockName, iomode.r);
  var lockFile = parseToml(toParse);

  // generate list of dependencies and get src code
  var (sourceList, gitList) = genSourceList(lockFile);
  getSrcCode(sourceList, false);
  getGitCode(gitList, false);

  const depPath = MASON_HOME + '/src/';
  const gitDepPath = MASON_HOME + '/git/';
  var modules: string;
  for (_, name, version) in sourceList {
    var depM = ' ' + depPath + name + "-" + version + '/src/';
    modules += depM;
  }

  for (_, name, branch) in gitList {
    var gitDepSrc = ' ' + gitDepPath + name + "-" + branch + '/src/' + name + ".chpl";
    modules += gitDepSrc;
  }
  
  writeln(modules);
}
