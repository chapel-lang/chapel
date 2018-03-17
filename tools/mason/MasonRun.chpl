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
  var toParse = open("Mason.toml", iomode.r);
  var tomlFile = parseToml(toParse);
  var project = tomlFile["brick"]["name"].s;
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
  if isDir('target') {
    var execs = ' '.join(execopts);
    var command = "target/debug/" + project + ' ' + execs;
    if isDir('target/release') then
      command = "target/release/" + project + ' ' + execs;

    if show then
      writeln("Executing binary: " + command);

    if isFile("Mason.lock") then  // If built
      runCommand(command);
    else if isFile("Mason.toml") { // If not built
      masonBuild(args);
      runCommand(command);
    }
    else writeln("call mason run from the top level of your projects directory");

    // Close memory
    delete tomlFile;
    toParse.close();
  }
  else {
    writeln("Mason cannot find the compiled program");
    exit();
  }
}

