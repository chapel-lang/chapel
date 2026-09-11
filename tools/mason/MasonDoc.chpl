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

/**/
module MasonDoc {

use ArgumentParser;
use FileSystem;
use IO;
use MasonHelp;
use MasonUtils;
import MasonLogger;
use List only list;

import Package;

import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

private var log = MasonLogger.getLogger("mason doc");

proc masonDoc(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonDocHelpHandler());
  var passArgs = parser.addPassThrough();
  parser.parseArgs(args);

  const package = Package.getMasonPackage(skipUpdate=true, show=false);

  const projectName = package.name;
  const version = package.version;

  var authors: string = ", ".join(package.authors.toArray());
  var copyrightYear: string = package.copyrightYear;

  const srcDir = "src":path;
  const projectFile = package.projectHome / "src" / (projectName + ".chpl");
  if projectFile.isFile() {
    // Must use relative paths with chpldoc to prevent baking in abs paths
    package.projectHome.chdir();

    var command = new list([
      "chpldoc",
      "--project-name=" + projectName,
      "--project-version=" + version:string,
    ]);
    if authors != "" {
      command.pushBack("--author=" + authors);
    }
    if copyrightYear != "" {
      command.pushBack("--project-copyright-year=" + copyrightYear);
    }
    const srcFiles = [f in srcDir.findFiles(recursive=true)]
                        if f.suffix == ".chpl" then f:string;
    command.pushBack(srcFiles);
    command.pushBack([
      "-o",
      "doc/",
    ]);
    command.pushBack(package.docopts);
    command.pushBack(passArgs.values());
    const commandArr = command.toArray();
    const commandStr = " ".join(commandArr);
    writeln(commandStr);
    runCommand(commandArr);
  } else {
    log.warn("Mason could not find the project to document!");
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

}
