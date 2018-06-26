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
use MasonBuild;
use Path;

/* Runs the .chpl files found within the /tests directory */
proc masonTest(args) {

  var show = false;
  var run = true;

  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonTestHelp();
        exit();
      }
      else if arg == '--show' {
        show = true;
      }
      else if arg == '--no-run' {
        run = false;
      }
      else {
        masonTestHelp();
        exit();
      }
    }
  }
  const uargs = [""];
  UpdateLock(uargs);
  runTests(show, run);
}

private proc runTests(show: bool, run: bool) {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", iomode.r);
    const lockFile = new Owned(parseToml(toParse));

    // Get project source code and dependencies
    const sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);
    const project = lockFile["root"]["name"].s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");

    // Make target files if they dont exist from a build
    makeTargetFiles("debug", projectHome);

    // Check for tests to run
    if lockFile.pathExists("root.tests") {
      var tests = lockFile["root"]["tests"].toString();
      const testNames = tests.split(',').strip("[]");

      for t in testNames {

        const test = t.strip().strip('"');
        const testPath = "".join(projectHome, '/test/', test);
        const testName = basename(test.strip(".chpl"));

        // get the string of dependencies for compilation
        const compopts = getDependencyString(sourceList, testName);

        const moveTo = "-o " + projectHome + "/target/test/" + testName;
        const compCommand = " ".join("chpl",testPath, projectPath, moveTo, compopts);
        const compilation = runWithStatus(compCommand);

        if show then writeln(compCommand);
        if compilation != 0 {
          writeln("compilation failed for " + test);
        }
        else {
          if show || !run then writeln("compiled ", test, " successfully");
          if run {
            const binCommand = "".join(projectHome,'/target/test/', testName);
            runCommand(binCommand);
          }
        }
      }
    }
    else {
      writeln("No tests were found in /test");
    }
    toParse.close();
  }
  catch e: MasonError {
    exit(1);
  }
}

private proc getDependencyString(sourceList: [?d] (string, string, string), testName) {

  // Declare test to run as the main module
  var compopts = " ".join(" --main-module", testName, " ");

  if sourceList.numElements > 0 {
    const depPath = MASON_HOME + "/src/";

    // Add dependencies to project
    for (_, name, version) in sourceList {
      var depSrc = "".join(' ',depPath, name, "-", version, '/src/', name, ".chpl");
      compopts += depSrc;
    }
  }
  return compopts;
}
