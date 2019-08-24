/*
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


private use List;
use TOML;
use Spawn;
use MasonUtils;
use MasonHelp;
use MasonUpdate;
use MasonBuild;
use Path;
use FileSystem;

/* Runs the .chpl files found within the /tests directory */
proc masonTest(args) throws {

  var show = false;
  var run = true;
  var parallel = false;
  var update = true;
  var compopts: list(string);

  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonTestHelp();
        exit(0);
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
      else if arg == '--' {
        throw new owned MasonError("Testing does not support -- syntax");
      }
      else if arg == '--no-update' {
        update = false;
      }
      else {
        compopts.append(arg);
      }
    }
  }
  var uargs: list(string);
  if !update then uargs.append('--no-update');
  UpdateLock(uargs);
  runTests(show, run, parallel, compopts);
}

private proc runTests(show: bool, run: bool, parallel: bool, ref cmdLineCompopts: list(string)) throws {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", iomode.r);
    const lockFile = new owned(parseToml(toParse));

    // Get project source code and dependencies
    const sourceList = genSourceList(lockFile);
    //
    // TODO: Temporarily use `toArray` here because `list` does not yet
    // support parallel iteration, which the `getSrcCode` method _must_
    // have for good performance.
    //
    getSrcCode(sourceList, show);
    const project = lockFile["root"]["name"].s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");

    // Get system, and external compopts
    const compopts = getTomlCompopts(lockFile, cmdLineCompopts);

    if isDir(joinPath(projectHome, "target/test/")) {
      rmTree(joinPath(projectHome, "target/test/"));
    }
    // Make target files if they dont exist from a build
    makeTargetFiles("debug", projectHome);

    // get the test names from lockfile or from test directory
    const testNames = getTests(lockFile.borrow(), projectHome);
    var numTests = testNames.size;

    // Check for tests to run
    if numTests > 0 {

      var resultDomain: domain(string);
      var testResults: [resultDomain] string;

      for test in testNames {

        const testPath = "".join(projectHome, '/test/', test);
        const testName = basename(stripExt(test, ".chpl"));

        // get the string of dependencies for compilation
        // also names test as --main-module
        const masonCompopts = getMasonDependencies(sourceList, testName);
        const allCompOpts = "".join(" ".join(compopts.these()), masonCompopts);

        const moveTo = "-o " + projectHome + "/target/test/" + testName;
        const compCommand = " ".join("chpl",testPath, projectPath, moveTo, allCompOpts);
        const compilation = runWithStatus(compCommand);

        if compilation != 0 {
          stderr.writeln("compilation failed for " + test);
        }
        else {
          if show || !run then writeln("compiled ", test, " successfully");
          if parallel {
            var result = runTestBinary(projectHome, testName, show);
            if result != 0 {
              testResults[testName] = "Failed";
            }
            else {
              testResults[testName] = "Passed";
            }
          }
        }
      }
      if run && !parallel {
        var testBinResults = runTestBinaries(projectHome, testNames, numTests, show);
        resultDomain = testBinResults.domain;
        testResults = testBinResults;
      }
      if run {
        const numPassed = testResults.count("Passed");
        printTestResults(testResults, numTests, numPassed, show);
      }
    }
    else {
      throw new owned MasonError("No tests were found in /test");
    }
    toParse.close();
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


private proc runTestBinary(projectHome: string, testName: string, show: bool) {
  const command = "".join(projectHome,'/target/test/', testName);
  const testResult = runWithStatus(command, show);
  return testResult;
}


private proc runTestBinaries(projectHome: string, testNames: list(string),
                             numTests: int, show: bool) {

  var resultDomain: domain(string);
  var testResults: [resultDomain] string;

  for test in testNames {
    const testName = basename(stripExt(test, ".chpl"));
    const result = runTestBinary(projectHome, testName, show);
    if result != 0 {
      testResults[testName] = "Failed";
    }
    else {
      testResults[testName] = "Passed";
    }
  }
  return testResults;
}


private proc printTestResults(testResults: [?d] string, numTests: int,
                              numPassed: int, show: bool) {

  if show then writeln("\n--------------------\n");
  writeln("--- Results ---");
  for test in testResults.domain {
    writeln(" ".join("Test:",test, testResults[test]));
  }
  writeln("\n--- Summary:  ",numTests, " tests run ---");
  writeln("-----> ", numPassed, " Passed");
  writeln("-----> ", (numTests - numPassed), " Failed");
  if (numTests - numPassed) == 0 {
    exit(0);
  }
  else {
    exit(1);
  }
}


private proc getMasonDependencies(sourceList: list(3*string),
                                 testName: string) {

  // Declare test to run as the main module
  var masonCompopts = " ".join(" --main-module", testName, " ");

  if sourceList.size > 0 {
    const depPath = MASON_HOME + "/src/";

    // Add dependencies to project
    for (_, name, version) in sourceList {
      var depSrc = "".join(' ',depPath, name, "-", version, '/src/', name, ".chpl");
      masonCompopts += depSrc;
    }
  }
  return masonCompopts;
}

private proc getTests(lock: borrowed Toml, projectHome: string) {
  var testNames: list(string);
  const testPath = joinPath(projectHome, "test");

  if lock.pathExists("root.tests") {
    var tests = lock["root"]["tests"].toString();
    var strippedTests = tests.split(',').strip('[]');
    for test in strippedTests {
      const t = test.strip().strip('"');
      testNames.append(t);
    }
    return testNames;
  }
  else if isDir(testPath) {
    var tests = findfiles(startdir=testPath, recursive=true, hidden=false);
    for test in tests {
      if test.endsWith(".chpl") {
        testNames.append(getTestPath(test));
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
