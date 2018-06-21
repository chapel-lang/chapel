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

use MasonBuild;
use MasonHelp;
use MasonUtils;
use FileSystem;
use TOML;

proc masonRun(args) {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    const tomlFile = new Owned(parseToml(toParse));
    const project = tomlFile["brick"]["name"].s;
    var show = false;
    var execopts: [1..0] string;

    if args.size > 2 {
      for arg in args[2..] {
        if arg == '-h' || arg == '--help' {
          masonRunHelp();
          exit();
        }
        else if arg == '--build' {
          masonBuild(['mason', 'build']);
        }
        else if arg == '--show' {
          show = true;
       }
       else if arg == 'run' {
          continue;
       }
       else {
          execopts.push_back(arg);
       }
      }
    }
    // Find the Binary and execute
    if isDir(projectHome + '/target') {
      var execs = ' '.join(execopts);
      var command = projectHome + "/target/debug/" + project + ' ' + execs;
      if isDir(projectHome + '/target/release') then
        command = projectHome + "/target/release/" + project + ' ' + execs;

      if show then
        writeln("Executing binary: " + command);

      if isFile(projectHome + "/Mason.lock") then  // If built
        runCommand(command);
      else if isFile(projectHome + "/Mason.toml") { // If not built
        masonBuild(args);
        runCommand(command);
      }
      else writeln("Mason could not find your Mason.lock file");

      // Close memory
      toParse.close();
    }
    else {
      writeln("Mason could not find the compiled program");
      exit();
    }
  }
  catch e: MasonError {
    writeln(e.message());
  }
}

