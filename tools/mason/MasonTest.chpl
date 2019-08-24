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
use TestResult;
use Sys;

config const subdir = true;
config const keepExec = false;
config const setComm: string = "";
var comm: string;

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
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    UpdateLock(uargs);
    runTests(show, run, parallel, compopts);
  }
  catch e: MasonError {
    runUnitTest(compopts);
  }
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

/* Gets the comm */
proc getRuntimeComm() throws {
  var line: string;
  var checkComm = spawn(["python",CHPL_HOME:string+"/util/chplenv/chpl_comm.py"],
                      stdout = PIPE);
  while checkComm.stdout.readline(line) {
    comm = line.strip();
  }
  // setting communication mechanism.
  if setComm != "" {
    if comm != "none" {
      comm = setComm;
    }
    else {
      if setComm == "none" then comm = setComm;
      else {
        writeln("Trying to execute in a multiLocale environment when ",
        "communication mechanism is `none`.");
        writeln("Try changing the communication mechanism");
        exit(2);
      }
    }
  }
}

proc runUnitTest(ref cmdLineCompopts: list(string)) {
  var comm_c: c_string;
  var dirs: list(string);
  var files: list(string);
  var hadInvalidFile = false;
  try! {
    var checkChpl = spawn(["which","chpl"],stdout = PIPE);
    checkChpl.wait();
    var line: string;
    if checkChpl.stdout.readline(line) {
      // get comm
      getRuntimeComm();

      for a in cmdLineCompopts {
        try! {
          if isFile(a) {
            files.append(a);
          }
          else if isDir(a) {
            dirs.append(a);
          }
          else {
            writeln("[Error: ", a, " is not a valid file or directory]");
            hadInvalidFile = true;
          }
        }
      }

      if hadInvalidFile && files.size == 0 && dirs.size == 0 {
        exit(2);
      }

      if files.size == 0 && dirs.size == 0 {
        dirs.append(".");
      }
      
      var result =  new TestResult();

      for tests in files {
        try {
          testFile(tests, result);
        }
        catch e {
          writeln("Caught an Exception in Running Test File: ", tests);
          writeln(e);
        }
      }

      for dir in dirs {
        try {
          testDirectory(dir, result);
        }
        catch e {
          writeln("Caught an Exception in Running Test Directory: ", dir);
          writeln(e);
        }
      }
        
      result.printErrors();
      writeln(result.separator2);
      result.printResult();
    }
    else {
      writeln("chpl not found.");
    } 
  }
  
}

pragma "no doc"
/*Docs: Todo*/
proc testFile(file, ref result) throws {
  var fileName = basename(file);
  if fileName.endsWith(".chpl") {
    var line: string;
    var compErr = false;
    var tempName = fileName.split(".chpl");
    var executable = tempName[1];
    var executableReal = executable + "_real";
    if isFile(executable) {
      FileSystem.remove(executable);
    }
    if isFile(executableReal) {
      FileSystem.remove(executableReal);
    }
    var sub = spawn(["chpl", file, "-o", executable, "-M.", 
                    "--comm", comm], stderr = PIPE); //Compiling the file
    var compError: string;
    if sub.stderr.readline(line) {
      compError = line;
      while sub.stderr.readline(line) do compError += line;
      compErr = true;
    }
    sub.wait();
    if !compErr {
      var testNames: list(string),
          failedTestNames: list(string),
          erroredTestNames: list(string),
          testsPassed: list(string),
          skippedTestNames: list(string);
      var dictDomain: domain(int);
      var dict: [dictDomain] int;
      runAndLog(executable, fileName, result, numLocales, testsPassed,
                testNames, dictDomain, dict, failedTestNames, erroredTestNames,
                skippedTestNames);
      if !keepExec {
        FileSystem.remove(executable);
        if isFile(executableReal) {
          FileSystem.remove(executableReal);
        }
      }
    }
    else {
      writeln("Compilation Error in ", fileName);
      writeln("Possible Reasons can be passing a non-test ",
              "function to UnitTest.runTest()");
      writeln(compError);
    }
  }
}

pragma "no doc"
/*Docs: Todo*/
proc testDirectory(dir, ref result) throws {
  for file in findfiles(startdir = dir, recursive = subdir) {
    testFile(file, result);
  }
}

pragma "no doc"
/*Docs: Todo*/
proc runAndLog(executable, fileName, ref result, reqNumLocales: int = numLocales,
              ref testsPassed, ref testNames, ref dictDomain, ref dict, 
              ref failedTestNames, ref erroredTestNames, ref skippedTestNames) throws 
{
  var separator1 = result.separator1,
      separator2 = result.separator2;
  var flavour: string,
      line: string,
      testExecMsg: string;
  var reqLocales = 0;
  var sep1Found = false,
      haltOccured = false;
  var testNamesStr,
      failedTestNamesStr,
      erroredTestNamesStr,
      passedTestStr,
      skippedTestNamesStr = "None";

  var currentRunningTests: list(string);
  
  //
  // List has a different `writeThis` format than arrays, since it encloses
  // the collection with brackets "[0, 1, 2, 3, ..., N]". This will cause
  // test failures since this code assumes array style output. The simplest
  // (albeit wasteful) thing we can do here is just cast the lists to
  // array here.
  //
  if testNames.size != 0 then testNamesStr = testNames.toArray(): string;
  if failedTestNames.size != 0 then failedTestNamesStr = failedTestNames.toArray(): string;
  if erroredTestNames.size != 0 then erroredTestNamesStr = erroredTestNames.toArray(): string;
  if testsPassed.size != 0 then passedTestStr = testsPassed.toArray(): string;
  if skippedTestNames.size != 0 then skippedTestNamesStr = skippedTestNames.toArray(): string;
  var exec = spawn(["./"+executable, "-nl", reqNumLocales: string, "--testNames", 
            testNamesStr,"--failedTestNames", failedTestNamesStr, "--errorTestNames", 
            erroredTestNamesStr, "--ranTests", passedTestStr, "--skippedTestNames", 
            skippedTestNamesStr], stdout = PIPE, 
            stderr = PIPE); //Executing the file
  //std output pipe
  while exec.stdout.readline(line) {
    if line.strip() == separator1 then sep1Found = true;
    else if line.strip() == separator2 && sep1Found {
      var testName = try! currentRunningTests.pop();
      if testNames.count(testName) != 0 then
        try! testNames.remove(testName);
      addTestResult(result, dictDomain, dict, testNames, flavour, fileName, 
                testName, testExecMsg, failedTestNames, erroredTestNames, 
                skippedTestNames, testsPassed);
      testExecMsg = "";
      sep1Found = false;
    }
    else if line.startsWith("Flavour") {
      var temp = line.strip().split(":");
      flavour = temp[2].strip();
      testExecMsg = "";
    }
    else if sep1Found then testExecMsg += line;
    else {
      if line.strip().endsWith(")") {
        var testName = line.strip();
        if currentRunningTests.count(testName) == 0 {
          currentRunningTests.append(testName);
          if testNames.count(testName) == 0 then
            testNames.append(testName);
        }
        testExecMsg = "";
      }  
    }
  }
  //this is to check the error
  if exec.stderr.readline(line) { 
    var testErrMsg = line;
    while exec.stderr.readline(line) do testErrMsg += line;
    if !currentRunningTests.isEmpty() {
      var testNameIndex = try! currentRunningTests.pop();
      var testName = testNameIndex;
      if testNames.count(testName) != 0 then
        try! testNames.remove(testName);
      erroredTestNames.append(testName);
      result.addError(testName, fileName, testErrMsg);
      haltOccured =  true;
    }
  }
  exec.wait();//wait till the subprocess is complete
  if haltOccured then
    runAndLog(executable, fileName, result, reqNumLocales, testsPassed,
              testNames, dictDomain, dict, failedTestNames, erroredTestNames,
              skippedTestNames);
  if testNames.size != 0 {
    var maxCount = -1;
    for key in dictDomain.sorted() {
      if maxCount < dict[key] {
        reqLocales = key;
        maxCount = dict[key];
      }
    }
    dictDomain.remove(reqLocales);
    runAndLog(executable, fileName, result, reqLocales, testsPassed,
              testNames, dictDomain, dict, failedTestNames, erroredTestNames, 
              skippedTestNames);
  }
}

pragma "no doc"
/*Docs: Todo*/
proc addTestResult(ref result, ref dictDomain, ref dict, ref testNames, 
                  flavour, fileName, testName, errMsg, ref failedTestNames, 
                  ref erroredTestNames, ref skippedTestNames, ref testsPassed) throws 
{
  select flavour {
    when "OK" {
      result.addSuccess(testName, fileName);
      testsPassed.append(testName);
    }
    when "ERROR" {
      result.addError(testName, fileName, errMsg);
      erroredTestNames.append(testName);
    }
    when "FAIL" {
      result.addFailure(testName, fileName, errMsg);
      failedTestNames.append(testName);
    }
    when "SKIPPED" {
      result.addSkip(testName, fileName, errMsg);
      skippedTestNames.append(testName);
    }
    when "IncorrectNumLocales" {
      if comm != "none" {
        var strSplit = errMsg.split("=");
        var reqLocalesStr = strSplit[2].strip().split(" ");
        for a in reqLocalesStr do
          if dictDomain.contains(a: int) then
            dict[a: int] += 1;
          else
            dict[a: int] = 1;
        testNames.append(testName);
      }
      else {
        var locErrMsg = "Not a MultiLocale Environment. $CHPL_COMM = " + comm + "\n";
        locErrMsg += errMsg; 
        result.addSkip(testName, fileName, locErrMsg);
        skippedTestNames.append(testName);
      }
    }
    when "Dependence" {
      testNames.append(testName);
    }
  }
}
