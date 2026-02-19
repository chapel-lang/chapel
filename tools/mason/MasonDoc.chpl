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
use MasonHelp;
use MasonUtils;
import MasonLogger;
use List only list;

private var log = new MasonLogger.logger("mason doc");

proc masonDoc(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonDocHelpHandler());
  var passArgs = parser.addPassThrough();
  parser.parseArgs(args);

  const tomlName = 'Mason.toml';
  const cwd = here.cwd();

  const projectHome = getProjectHome(cwd, tomlName);
  const tomlPath = projectHome + "/" + tomlName;

  const toParse = open(projectHome + "/" + tomlName, ioMode.r);
  var tomlFile = parseToml(toParse);

  const projectName = tomlFile["brick.name"]!.s;
  const projectFile = projectName + '.chpl';

  const version = tomlFile["brick.version"]!.s;

  var authors: string;
  if const authorsToml = tomlFile.get["brick.authors"] {
    if !isStringOrStringArray(authorsToml) {
      throw new MasonError("unable to parse authors");
    }
    if authorsToml.tomlType == "string" {
      authors = authorsToml.s;
    } else if authorsToml.tomlType == "array" {
      authors = ", ".join(authorsToml.arr!.s);
    }
  }
  var copyrightYear: string;
  if const copyrightToml = tomlFile.get["brick.copyrightYear"] {
    copyrightYear = copyrightToml.s;
  }

  if isDir(projectHome + '/src/') &&
      isFile(projectHome + '/src/' + projectFile) {
    // Must use relative paths with chpldoc to prevent baking in abs paths
    here.chdir(projectHome);

    var command = new list([
      "chpldoc",
      "--project-name=" + projectName,
      "--project-version=" + version,
    ]);
    if authors != "" {
      command.pushBack("--author=" + authors);
    }
    if copyrightYear != "" {
      command.pushBack("--project-copyright-year=" + copyrightYear);
    }
    command.pushBack([
      joinPath("src", projectFile),
      "-o",
      "doc/",
      "--process-used-modules"
    ]);
    command.pushBack(getTomlDocopts(tomlFile));
    command.pushBack(passArgs.values());
    const commandArr = command.toArray();
    const commandStr = " ".join(commandArr);
    writeln(commandStr);
    runCommand(commandArr);
  } else {
    log.warnln('Mason could not find the project to document!');
    var command = new list([
      "chpldoc",
    ]);
    command.pushBack(passArgs.values());
    runCommand(command.toArray());
  }
}

proc getTomlDocopts(lock: borrowed Toml): list(string) throws {
  var docopts: list(string);

  // Checks for compilation options are present in Mason.toml
  if const docoptsToml = lock.get("brick.docopts") {
    try {
      docopts.pushBack(parseCompilerOptions(docoptsToml));
    } catch {
      throw new MasonError("unable to parse docopts");
    }
  }

  return docopts;
}
