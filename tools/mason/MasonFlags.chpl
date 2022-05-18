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
use MasonEnv;
use MasonBuild;
use Subprocess;
use MasonUpdate;
use TOML;

proc masonFlags(args: [] string) throws {
  var listArgs: list(string);
  for x in args do listArgs.append(x);
  showMasonFlags();
}

proc masonPrep(args: [] string) throws {
  var listArgs: list(string);
  for x in args do listArgs.append(x);
  prepMasonLightweight();
}

proc prepMasonLightweight(tomlName="Mason.toml", lockName="Mason.lock") {
  updateLock(false, tomlName, lockName);
  const cwd = here.cwd();
  const toParse = open(cwd + "/" + lockName, iomode.r);
  var lockFile = owned.create(parseToml(toParse));
  const sourceList = genSourceList(lockFile);
  getSrcCode(sourceList, true);
}

proc showMasonFlags(tomlName="Mason.toml", lockName="Mason.lock") throws {
  const cwd = here.cwd();
  const toParse = open(cwd + "/" + lockName, iomode.r);
  var lockFile = owned.create(parseToml(toParse));
  const sourceList = genSourceList(lockFile);
  const depPath = MASON_HOME + '/src/';

  var flags: string;

  for (_, name, version) in sourceList {
    var depSrc = ' ' + depPath + name + "-" + version + '/src/' + name + ".chpl";
    flags += depSrc;
  }

  writeln(flags);
}
