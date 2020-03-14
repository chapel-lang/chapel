/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
private use Map;
use TOML;
use Time;
use Spawn;
use MasonUtils;
use MasonHelp;
use MasonUpdate;
use MasonBuild;
use Path;
use FileSystem;
use TestResult;
use Sys;

var subdir = false;
var keepExec = false;
var customTest = false;
var setComm: string;
var comm: string;
var dirs: list(string);
var files: list(string);

/* Runs the .chpl files found within the /tests directory of Mason packages
   or files which in the path provided.
*/
proc masonTest(args) throws {

  var show = false;
  var run = true;
  var parallel = false;
  var update = true;
  if MASON_OFFLINE then update = false;
  var compopts: list(string);
  var countArgs = 0;
  for arg in args {
    countArgs += 1;
    if countArgs > 2 {
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
      else if arg == '--keep-binary' {
        keepExec = true;
      }
      else if arg == '--recursive' {
        subdir = true;
      }
      else if arg == '--update' {
        update = true;
      }
      else if arg.startsWith('--setComm=') {
        setComm = arg['--setComm='.size+1..];
      }
      else {
        try! {
          if isFile(arg) && arg.endsWith(".chpl") {
            files.append(arg);
          }
          else if isDir(arg) {
            dirs.append(arg);
          }
          else {
            compopts.append(arg);
          }
        }
      }
    }
  }
  getRuntimeComm();
  var uargs: list(string);
  if !update then uargs.append('--no-update');
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    UpdateLock(uargs);
    compopts.append("".join("--comm=",comm));
    runTests(show, run, parallel, compopts);
  }
  catch e: MasonError {
    runUnitTest(compopts, show);
  }
}

private proc runTests(show: bool, run: bool, parallel: bool, ref cmdLineCompopts: list(string)) throws {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", iomode.r);
    const lockFile = owned.create(parseToml(toParse));

    // Get project source code and dependencies
    const sourceList = genSourceList(lockFile);

    getSrcCode(sourceList, show);
    const project = lockFile["root"]!["name"]!.s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");

    // Get system, and external compopts
    const compopts = getTomlCompopts(lockFile, cmdLineCompopts);

    if isDir(joinPath(projectHome, "target/test/")) {
      rmTree(joinPath(projectHome, "target/test/"));
    }
    // Make target files if they dont exist from a build
    makeTargetFiles("debug", projectHome);
    var numTests: int;
    var testNames: list(string);
    // get the test names from lockfile or from test directory
    if (files.size == 0 && dirs.size == 0) {
      testNames = getTests(lockFile.borrow(), projectHome);
      numTests = testNames.size;
    }
    else {
      try! {
        for dir in dirs {
          for file in findfiles(startdir = dir, recursive = subdir) {
            if file.endsWith(".chpl") {
              files.append(file);
            }
          }
        }
      }
      testNames = files;
      numTests = files.size;
      customTest = true;
    }
    // Check for tests to run
    if numTests > 0 {

      var result =  new TestResult();
      var timeElapsed = new Timer();
      timeElapsed.start();
      for test in testNames {
        var testPath: string;
        if customTest {
          testPath = "".join(cwd,"/",test);
        } 
        else {
          testPath = "".join(projectHome, '/test/', test);
        }
        const testName = basename(stripExt(test, ".chpl"));

        // get the string of dependencies for compilation
        // also names test as --main-module
        const masonCompopts = getMasonDependencies(sourceList, testName);
        const allCompOpts = "".join(" ".join(compopts.these()), masonCompopts);
        var testTemp: string = test;
        if cwd == projectHome && customTest {
          testTemp = relPath(testTemp,"test/");
        }
        const outputLoc = projectHome + "/target/test/" + stripExt(testTemp, ".chpl");
        const moveTo = "-o " + outputLoc;
        const compCommand = " ".join("chpl",testPath, projectPath, moveTo, allCompOpts);
        const compilation = runWithStatus(compCommand);
        
        if compilation != 0 {
          stderr.writeln("compilation failed for " + test);
        }
        else {
          if show || !run then writeln("Compiled '", test, "' successfully");
          if parallel {
            runTestBinary(projectHome, outputLoc, testName, result, show);
          }
        }
      }
      if run && !parallel {
        runTestBinaries(projectHome, testNames, numTests, result, show);
      }
      timeElapsed.stop();
      if run {
        printTestResults(result, timeElapsed);
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


private proc runTestBinary(projectHome: string, outputLoc: string, testName: string, 
                        ref result, show: bool) {
  const command = outputLoc;
  var testNames: list(string),
      failedTestNames: list(string),
      erroredTestNames: list(string),
      testsPassed: list(string),
      skippedTestNames: list(string);
  var localesCountMap: map(int, int, parSafe=true);
  const exitCode = runAndLog(command, testName+".chpl", result, numLocales, testsPassed,
            testNames, localesCountMap, failedTestNames, erroredTestNames, skippedTestNames, show);
  if exitCode != 0 {
    const newCommand = " ".join(command,"-nl","1");
    const testResult = runWithStatus(newCommand, show);
    if testResult != 0 {
      const errMsg = testName: string +" returned exitCode = "+testResult: string;
      result.addFailure(testName, testName+".chpl", errMsg);
    }
    else {
      result.addSuccess(testName, testName+".chpl");
    }
  }
}


private proc runTestBinaries(projectHome: string, testNames: list(string),
                             numTests: int, ref result, show: bool) {

  const cwd = getEnv("PWD");
  for test in testNames {
    var testTemp: string = test;
    if cwd == projectHome && customTest {
      testTemp = relPath(testTemp,"test/");
    }
    const outputLoc = projectHome + "/target/test/" + stripExt(testTemp, ".chpl");
    const testName = basename(stripExt(test, ".chpl"));
    runTestBinary(projectHome, outputLoc, testName, result, show);
  }
}


private proc printTestResults(ref result, timeElapsed) {

  result.printErrors();
  writeln(result.separator2);
  result.printResult(timeElapsed.elapsed());
  if (result.testsRun - result.testsPassed) == 0 {
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
    var tests = lock["root"]!["tests"]!.toString();
    var strippedTests = tests.split(',').strip('[]');
    for test in strippedTests {
      const t = test.strip().strip('"');
      testNames.append(t);
    }
  }
  else if isDir(testPath) {
    var tests = findfiles(startdir=testPath, recursive=true, hidden=false);
    for test in tests {
      if test.endsWith(".chpl") {
        testNames.append(getTestPath(test));
      }
    }
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

proc runUnitTest(ref cmdLineCompopts: list(string), show: bool) {
  var comm_c: c_string;
  try! {
    var checkChpl = spawn(["which","chpl"],stdout = PIPE);
    checkChpl.wait();
    var line: string;
    if checkChpl.stdout.readline(line) {

      if files.size == 0 && dirs.size == 0 {
        dirs.append(".");
      }
      
      var result =  new TestResult();
      var timeElapsed = new Timer();
      timeElapsed.start();
      for tests in files {
        try {
          testFile(tests, result, show);
        }
        catch e {
          writeln("Caught an Exception in Running Test File: ", tests);
          writeln(e);
        }
      }

      for dir in dirs {
        try {
          testDirectory(dir, result, show);
        }
        catch e {
          writeln("Caught an Exception in Running Test Directory: ", dir);
          writeln(e);
        }
      }
      timeElapsed.stop();
      printTestResults(result, timeElapsed);
    }
    else {
      writeln("chpl not found.");
      exit(2);
    } 
  }
  
}

pragma "no doc"
/*Docs: Todo*/
proc testFile(file, ref result, show: bool) throws {
  var fileName = basename(file);
  var line: string;
  var compErr = false;
  var executable = stripExt(fileName,".chpl");
  var executableReal = executable + "_real";
  // remove the binaries if they exist
  if isFile(executable) {
    FileSystem.remove(executable);
  }
  if isFile(executableReal) {
    FileSystem.remove(executableReal);
  }

  const moveTo = "-o " + executable;
  const allCompOpts = "--comm " + comm;
  const compCommand = " ".join("chpl",file, moveTo, allCompOpts);
  const compilation = runWithStatus(compCommand);

  if compilation != 0 {
    stderr.writeln("compilation failed for " + fileName);
  }
  else {
    if show then writeln("\nCompiled '", fileName, "' successfully");
    var testNames: list(string),
        failedTestNames: list(string),
        erroredTestNames: list(string),
        testsPassed: list(string),
        skippedTestNames: list(string);
    var localesCountMap: map(int, int, parSafe=true);
    const exitCode = runAndLog("./"+executable, fileName, result, numLocales, testsPassed,
              testNames, localesCountMap, failedTestNames, erroredTestNames, skippedTestNames, show);
    if exitCode != 0 {
      const command = " ".join("./"+executable,"-nl","1");
      const testResult = runWithStatus(command, show);
      if testResult != 0 {
        const errMsg = executable: string +" returned exitCode = "+testResult: string;
        result.addFailure(executable, fileName, errMsg);
      }
      else {
        result.addSuccess(executable, fileName);
      }
    }
    if !keepExec {
      FileSystem.remove(executable);
      if isFile(executableReal) {
        FileSystem.remove(executableReal);
      }
    }
  }
}

pragma "no doc"
/*Docs: Todo*/
proc testDirectory(dir, ref result, show: bool) throws {
  for file in findfiles(startdir = dir, recursive = subdir) {
    if file.endsWith(".chpl") {
      testFile(file, result, show);
    }
  }
}

pragma "no doc"
/*Docs: Todo*/
proc runAndLog(executable, fileName, ref result, reqNumLocales: int = numLocales,
              ref testsPassed, ref testNames, ref localesCountMap, 
              ref failedTestNames, ref erroredTestNames, ref skippedTestNames, show: bool): int throws 
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
  var exitCode: int;
  
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
  var exec = spawn([executable, "-nl", reqNumLocales: string, "--testNames", 
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
      addTestResult(result, localesCountMap, testNames, flavour, fileName, 
                testName, testExecMsg, failedTestNames, erroredTestNames, 
                skippedTestNames, testsPassed, show);
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
      if show then writeln("Ran ",testName," ERROR");
      result.addError(testName, fileName, testErrMsg);
      haltOccured =  true;
    }
  }
  exec.wait();//wait till the subprocess is complete
  exitCode = exec.exit_status;
  if haltOccured then
    exitCode = runAndLog(executable, fileName, result, reqNumLocales, testsPassed,
              testNames, localesCountMap, failedTestNames, erroredTestNames, skippedTestNames, show);
  if testNames.size != 0 {
    var maxCount = -1;
    for key in localesCountMap {
      if maxCount < localesCountMap[key] {
        reqLocales = key;
        maxCount = localesCountMap[key];
      }
    }
    localesCountMap.remove(reqLocales);
    exitCode = runAndLog(executable, fileName, result, reqLocales, testsPassed,
              testNames, localesCountMap, failedTestNames, erroredTestNames, skippedTestNames, show);
  }
  return exitCode;
}

pragma "no doc"
/*Docs: Todo*/
proc addTestResult(ref result, ref localesCountMap, ref testNames, 
                  flavour, fileName, testName, errMsg, ref failedTestNames, 
                  ref erroredTestNames, ref skippedTestNames, ref testsPassed,
                  show: bool) throws 
{
  select flavour {
    when "OK" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addSuccess(testName, fileName);
      testsPassed.append(testName);
    }
    when "ERROR" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addError(testName, fileName, errMsg);
      erroredTestNames.append(testName);
    }
    when "FAIL" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addFailure(testName, fileName, errMsg);
      failedTestNames.append(testName);
    }
    when "SKIPPED" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addSkip(testName, fileName, errMsg);
      skippedTestNames.append(testName);
    }
    when "IncorrectNumLocales" {
      if comm != "none" {
        var strSplit = errMsg.split("=");
        var reqLocalesStr = strSplit[2].strip().split(",");
        for a in reqLocalesStr do
          if localesCountMap.contains(a: int) then
            localesCountMap[a: int] += 1;
          else
            localesCountMap[a: int] = 1;
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
