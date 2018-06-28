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
use FileSystem;

/* Runs the .chpl files found within the /tests directory */
proc masonTest(args) {

  var show = false;
  var run = true;
  var parallel = false;

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
      else if arg == '--parallel' {
        parallel = true;
      }
      else {
        masonTestHelp();
        exit();
      }
    }
  }
  const uargs = [""];
  UpdateLock(uargs);
  runTests(show, run, parallel);
}

private proc runTests(show: bool, run: bool, parallel: bool) {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", iomode.r);
    const lockFile = parseToml(toParse);

    // Get project source code and dependencies
    const sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);
    const project = lockFile["root"]["name"].s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");

    if isDir(joinPath(projectHome, "target/test/")) {
      rmTree(joinPath(projectHome, "target/test/"));
    }
    // Make target files if they dont exist from a build
    makeTargetFiles("debug", projectHome);

    // get the test names from lockfile or from test directory
    const testNames = getTests(lockFile, projectHome);
    var numTests = testNames.domain.size;

    // Check for tests to run
    if numTests > 0 {

      coforall test in testNames {

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
        }
      }
      if run {
        runTestBinaries(projectHome, testNames, numTests, show, parallel);
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

private proc runTestBinaries(projectHome: string, testNames: [?D] string, numTests: int,
                             show: bool, parallel: bool) {

  var resultDomain: domain(string);
  var testResults: [resultDomain] string;
  var numPassed: atomic int;

  if show then writeln("\n--- Test Output ---\n");

  if parallel {
    forall test in testNames {
      const testName = basename(test.strip(".chpl"));
      const command = "".join(projectHome,'/target/test/', testName);
      const result = runWithStatus(command, show);
      if result != 0 {
        testResults[basename(command)] = "Failed";
      }
      else {
        testResults[basename(command)] = "Passed";
        numPassed.add(1);
      }
    }
  }
  else { 
    for test in testNames {
      const testName = basename(test.strip(".chpl"));
      const command = "".join(projectHome,'/target/test/', testName);
      const result = runWithStatus(command, show);
      if result != 0 {
        testResults[basename(command)] = "Failed";
      }
      else {
        testResults[basename(command)] = "Passed";
        numPassed.add(1);
      }
    }
  }
  if show then writeln("\n-------------------\n");
  writeln("--- Results ---");
  for test in testResults.domain {
    writeln(" ".join("Test:",test, testResults[test]));
  }
  writeln("\n--- Summary:  ",numTests, " tests run ---");
  writeln("-----> ", numPassed, " Passed");
  writeln("-----> ", (numTests - numPassed.read()), " Failed");
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

private proc getTests(lock: Toml, projectHome: string) {
  var testNames: [1..0] string;
  const testPath = joinPath(projectHome, "test");

  if lock.pathExists("root.tests") {
    var tests = lock["root"]["tests"].toString();
    var strippedTests = tests.split(',').strip('[]');
    for test in strippedTests {
      const t = test.strip().strip('"');
      testNames.push_back(t);
    }
    return testNames;
  }
  else if isDir(testPath) {
    var tests = findfiles(startdir=testPath, recursive=true, hidden=false);
    for test in tests {
      if test.endsWith(".chpl") {
        testNames.push_back(getTestPath(test));
      }
    }
    return testNames;
  }
  return testNames;
}

/* Gets the path of the test following the test dir */
proc getTestPath(fullPath: string, testPath = "") : string {
  var split = splitPath(fullPath);
  if split[2] == "test" {
    return testPath;
  }
  else {
    if testPath == "" {
      return getTestPath(split[1], split[2]);
    }
    else {
      var appendedPath = joinPath(split[2], testPath);
      return getTestPath(split[1], appendedPath);
    }
  }
}




