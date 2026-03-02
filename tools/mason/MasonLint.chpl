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

module MasonLint {
  use ArgumentParser;
  use FileSystem;
  use IO;
  import MasonHelp;
  import MasonUtils;
  import MasonUtils.MasonError;
  import MasonLogger;
  use List only list;

  private var log = new MasonLogger.logger("mason lint");

  proc masonLint(args: [] string) throws {

    var parser =
      new argumentParser(helpHandler=new MasonHelp.MasonLintHelpHandler());
    var passArgs = parser.addPassThrough();
    parser.parseArgs(args);

    const tomlName = 'Mason.toml';
    const cwd = here.cwd();

    const projectHome = MasonUtils.getProjectHome(cwd, tomlName);
    here.chdir(projectHome);

    if MasonUtils.runWithStatus("chplcheck --version", quiet=true) != 0 {
      throw new MasonError("chplcheck is not installed or not found in PATH");
    }

    const cmd = new list([
      "chplcheck",
      "-c",
      "Mason.toml",
      "src/**/*.chpl",
      "test/**/*.chpl",
      "example/**/*.chpl",
    ]);
    cmd.pushBack(passArgs.values());
    const cmdArr = cmd.toArray();
    const cmdStr = " ".join(cmdArr);
    log.infoln(cmdStr);
    const output =
      MasonUtils.runCommand(cmdStr, quiet=true, errorOk=true).strip();

    if output != "" {
      // change projectHome to "" to make output paths relative
      const filtered = output.replace(projectHome + "/", "");
      writeln(filtered);
      throw new MasonError("chplcheck found issues");
    }
  }
}
