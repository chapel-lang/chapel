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


use TOML;
use Spawn;
use MasonUtils;
use MasonHelp;
use MasonUpdate;

/* Runs the .chpl files found within the /tests directory */
proc masonTest(args) {

  var show = false;

  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonTestHelp();
        exit();
      }
      else if arg == '--show' {
        show = true;
        runTests(show);
      }
      else {
        masonTestHelp();
        exit();
      }
    }
   }
   else {
     const args = [""];
     UpdateLock(args);
     runTests(show);
    }
}


private proc runTests(show: bool) {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", iomode.r);
    const lockFile = new Owned(parseToml(toParse));

    if lockFile.pathExists("root.tests") {
      var tests = lockFile["root"]["tests"].toString();
      const testNames = tests.split(',').strip("[]");

      for t in testNames {

        const test = t.strip().strip('"');
        if show then writeln("test: ", test);

        const pathToProject = projectHome + '/test/' + test;
        const binary = test.strip(".chpl");
        const moveTo = "-o " + projectHome + "/target/test/" + binary;
        const compCommand = "chpl " + pathToProject + " " + moveTo;
        const compilation = runWithStatus(compCommand);
        if compilation != 0 {
          writeln("compilation failed for " + test);
        }
        else {
          if show then writeln("compiled ", test, " successfully");
          const binCommand = projectHome + '/target/test/' + binary;
          runCommand(binCommand);
        }
      }
    }
    else {
      writeln("No tests were found your Mason.lock");
    }
    toParse.close();
  } 
  catch e: MasonError {
    writeln(e.message());
  }
}
