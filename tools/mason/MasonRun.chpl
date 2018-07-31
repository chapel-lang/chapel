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

proc masonRun(args) throws {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    const tomlFile = new Owned(parseToml(toParse));
    const project = tomlFile["brick"]["name"].s;
    var show = false;
    var example = false;
    var execopts: [1..0] string;

    if args.size > 2 {
      for arg in args[2..] {
        if arg == '-h' || arg == '--help' {
          masonRunHelp();
          exit(0);
        }
        else if arg == '--build' {
          masonBuild(['mason', 'build']);
        }
        else if arg == '--show' {
          show=true;
       }
       else if arg == 'run' {
          continue;
       }
       else if arg == '--example' {
         example=true;
         if args.size > 3 {
           var exampleArgs = args[3..];
           exampleArgs.push_back("--no-build");
           masonExample(exampleArgs);
         }         
         else printAvailableExamples(tomlFile, projectHome); 
       }
       else {
          execopts.push_back(arg);
       }
      }
    }
    if !example {
      // Find the Binary and execute
      if isDir(joinPath(projectHome, 'target')) {
        var execs = ' '.join(execopts);
        var command: string;
        if isDir(joinPath(projectHome, 'target/release')) then
          command = joinPath(projectHome, "target/release", project);
        else {
          command = joinPath(projectHome, "target/debug", project);
        }

        var built = false;
        if isFile(command) then built = true;

        // add execopts
        command += " " + execs;

        if show then writeln("Executing binary: " + command);

        if isFile(joinPath(projectHome, "Mason.lock")) && built then // If built
          runCommand(command);
        else if isFile(joinPath(projectHome, "Mason.toml")) { // If not built
          masonBuild(args);
          runCommand(command);
        }
        else {
          throw new MasonError("Mason could not find your Mason.toml file");
        }
        // Close memory
        toParse.close();
      }
      else {
        throw new MasonError("Mason could not find the compiled program");
      }
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}

