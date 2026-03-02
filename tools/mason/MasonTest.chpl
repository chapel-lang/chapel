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


use ArgumentParser;
use FileSystem;
use List;
use Map;
use MasonBuild;
use MasonHelp;
use MasonUpdate;
use MasonUtils;
use Path;
use Subprocess;
use OS.POSIX;
use TestResult;
use Time;
use TOML;

import MasonLogger;
import MasonPrereqs;

var subdir = false;
var keepExec = false;
var customTest = false;
var setComm: string;
var comm: string;
var dirs: list(string);
var files: list(string);

private var log = new MasonLogger.logger("mason test");

/* Runs the .chpl files found within the /tests directory of Mason packages
   or files which in the path provided.
*/
proc masonTest(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonTestHelpHandler());

  var runFlag = parser.addFlag(name="run",
                               opts=["--no-run"],
                               defaultValue=false);

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var keepFlag = parser.addFlag(name="keep-binary", defaultValue=false);
  var recursFlag = parser.addFlag(name="recursive", defaultValue=false);
  var parFlag = parser.addFlag(name="parallel", defaultValue=false);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);
  var setCommOpt = parser.addOption(name="setComm");
  var filterFlag = parser.addOption(name="filter", defaultValue="");

  // TODO: Why doesn't masonTest support a passthrough for values that should
  // go to the runtime?
  var otherArgs = parser.addArgument(name="others", numArgs=0..);

  parser.parseArgs(args);

  var skipUpdate = MASON_OFFLINE;
  var show = showFlag.valueAsBool();
  var run = !runFlag.valueAsBool();
  var parallel = parFlag.valueAsBool();
  var filter = filterFlag.value();
  keepExec = keepFlag.valueAsBool();
  subdir = recursFlag.valueAsBool();
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }
  if skipUpdate then log.debugln("Will skip updates");

  if setCommOpt.hasValue() then setComm = setCommOpt.value();

  var isMasonProject = true;
  try {
    getProjectHome(here.cwd());
  } catch e: MasonError {
    isMasonProject = false;
  }

  if isMasonProject {
    const projectType = getProjectType();
    if projectType == "light" then
      throw new MasonError("Mason light projects do not " +
                           "currently support 'mason test'");
  }


  var compopts: list(string);
  var searchSubStrings: list(string);

  if otherArgs.hasValue() {
    var flagInArgs = false;
    for arg in otherArgs.values() {
      // try to get option values meant for compilation
      if flagInArgs && !arg.startsWith('-') {
        compopts.pushBack(arg);
        flagInArgs=false;
      } else if isFile(arg) && arg.endsWith(".chpl") {
        // assume this is an individual test file

        files.pushBack(arg);
      } else if isDir(arg) {
        // assume this is a test directory
        dirs.pushBack(arg);
      } else if arg.startsWith('-') {
        // assume a flag for compiler
        compopts.pushBack(arg);
        flagInArgs=true;
      } else {
        searchSubStrings.pushBack(arg);
      }
    }
  }

  getRuntimeComm();
  try! {
    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd);

    if !searchSubStrings.isEmpty() {
      var testNames: list(string);
      const testPath = joinPath(projectHome, "test");
      var subTestPath = testPath: string;

      var inProjectDir = cwd==projectHome;
      if !inProjectDir {
        subTestPath = cwd;
      }

      var tests = findFiles(startdir=subTestPath, recursive=true, hidden=false);
      for test in tests {
        if test.endsWith(".chpl") {
          if inProjectDir {
            testNames.pushBack(getTestPath(test));
          } else {
            var testLoc = "";
            while test!=subTestPath {
              var split = splitPath(test);
              testLoc = if !testLoc.isEmpty() then joinPath(split[1], testLoc)
                                              else split[1];
              test = split[0];
            }
            testNames.pushBack(testLoc);
          }
        }
      }

      var isSubString: bool;

      for subString in searchSubStrings {
        isSubString = false;
        for testName in testNames {
          if testName.find(subString) != -1 {
            isSubString = true;
            if inProjectDir {
              files.pushBack("".join('test/', testName));
            } else {
              files.pushBack(testName);
            }
          }
        }

        if !isSubString {
          compopts.pushBack(subString);
        }
      }
    }

    updateLock(skipUpdate);
    compopts.pushBack("".join("--comm=",comm));
    runTests(show, run, parallel, filter, skipUpdate, compopts);
  } catch e: MasonError {
    try! {
      if !searchSubStrings.isEmpty() {
        var testNames: list(string);

        if isDir('.') {
          var tests = findFiles(startdir='.', recursive=subdir);
          for test in tests {
            if test.endsWith(".chpl") {
              testNames.pushBack(test);
            }
          }
        }

        for subString in searchSubStrings {
          for testName in testNames {
            if testName.find(subString) != -1 {
              files.pushBack(testName);
            }
          }
        }
      }
    }
    runUnitTest(compopts, filter, show);
  }
}

private proc runTests(show: bool, run: bool, parallel: bool, filter: string,
                      skipUpdate: bool, cmdLineCompopts: list(string)) throws {

  try! {

    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd);

    // parse lockfile
    const toParse = open(projectHome + "/Mason.lock", ioMode.r);
    const lockFile = parseToml(toParse);

    // Get project source code and dependencies
    const (sourceList, gitList) = genSourceList(lockFile);
    const depPath = Path.joinPath(MASON_HOME, 'src');
    const gitDepPath = Path.joinPath(MASON_HOME, 'git');

    getSrcCode(sourceList, skipUpdate, show);
    getGitCode(gitList, show);

    const project = lockFile["root.name"]!.s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");

    // Get system, and external compopts
    var compopts = cmdLineCompopts;
    compopts.pushBack(getTomlCompopts(lockFile));
    log.debugf("compopts from Mason.toml: %?\n", compopts);

    log.debugln("Adding prerequisite flags");

    // add prerequisite compopts
    for flag in MasonPrereqs.chplFlags() {
      log.debugf("+compflag %s\n", flag);
      compopts.pushBack(flag);
    }

    log.debugf("Base compopts: %?\n", compopts);

    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, version) in srcSource.iterList(sourceList) {
      const nameVer = "%s-%s".format(name, version);
      // version of -1 specifies a git dep
      if version != "-1" {
        const depDir = Path.joinPath(depPath, nameVer);
        const depSrc = Path.replaceExt(Path.joinPath(depDir, "src", name),
                                       "chpl");

        log.debugf("Adding source dependency %s's flags\n", name);
        compopts.pushBack(depSrc);

        for flag in MasonPrereqs.chplFlags(depDir) {
          log.debugf("+compflag %s\n", flag);
          compopts.pushBack(flag);
        }
      }
    }

    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, branch, _y) in gitSource.iterList(gitList) {
      const gitDepSrc = Path.joinPath(gitDepPath, name + "-" + branch,
                                      'src', name + ".chpl");
      compopts.pushBack(gitDepSrc);
    }

    // get system deps
    if const pkgDeps = lockFile.get['system'] {
      for (_, depInfo) in zip(pkgDeps.A.keys(), pkgDeps.A.values()) {
        for (k,v) in allFields(depInfo!) {
          var val = v!;
          select k {
            when "libs" do compopts.pushBack(parseCompilerOptions(val));
            when "include" do
              if val.s != "" then compopts.pushBack("-I" + val.s);
            otherwise continue;
          }
        }
      }
    }

    if isDir(joinPath(projectHome, "target/test/")) {
      rmTree(joinPath(projectHome, "target/test/"));
    }
    // Make target files if they dont exist from a build
    makeTargetFiles("debug", projectHome);
    var numTests: int;
    var testNames: list(string);
    // names of tests that compiled
    var testsCompiled: list(string, parSafe=true);
    // get the test names from lockfile or from test directory
    if files.size == 0 && dirs.size == 0 {
      testNames = getTests(lockFile.borrow(), projectHome);
      numTests = testNames.size;
    } else {
      try! {
        for dir in dirs {
          for file in findFiles(startdir = dir, recursive = subdir) {
            if file.endsWith(".chpl") {
              files.pushBack(file);
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

      proc compile(test: string,
                   ref result: TestResult,
                   ref testsCompiled: list(?)): (string, bool) {
        var testPath: string;
        if isAbsPath(test) {
          testPath = test;
        } else {
          if customTest then
            testPath = "".join(cwd,"/",test);
          else
            testPath = "".join('test/', test);
        }
        log.infof("Testing %s\n", testPath);
        const testName = basename(stripExt(test, ".chpl"));

        // get the string of dependencies for compilation
        // also names test as --main-module
        const masonCompopts =
          getMasonDependencies(sourceList, gitList, testName);
        var testTemp: string = test;
        if cwd == projectHome && customTest {
          testTemp = relPath(testTemp, "test/");
        }
        const outputLoc =
          joinPath(projectHome, "target", "test", stripExt(testTemp, ".chpl"));
        var compCommand = new list(string);
        compCommand.pushBack(["chpl", testPath, projectPath, "-o", outputLoc]);
        compCommand.pushBack(compopts);
        compCommand.pushBack(masonCompopts);
        log.debugf("\t%?\n", compCommand);
        const compilation = runWithStatus(compCommand.toArray(), !show);
        const success = compilation == 0;

        if !success {
          stderr.writeln("compilation failed for " + test);
          var errMsg = test + " failed to compile";
          if !show then
            errMsg += "\nTry running 'mason test --show' for more details";
          result.addError(testName, test,  errMsg);
        } else {
          testsCompiled.pushBack(test);
          if show || !run then writeln("Compiled '", test, "' successfully");
        }
        return (outputLoc, success);
      }


      var timeElapsed = new stopwatch();
      timeElapsed.start();
      if parallel {
        // list.these does throttling for lists < 64 elements, so we won't get
        // parallelism for "small" test suites.
        // Use range as a fragile workaround instead.
        // we could also get fancy and use some kind of work queue, because
        // some test files could be huge and others could be tiny
        forall testIdx in 0..#testNames.size with (ref result,
                                                  ref testsCompiled) {
          const testName = testNames[testIdx];
          const (outputLoc, success) = compile(testName, result, testsCompiled);
          if success && run {
            runTestBinary(outputLoc, testName, filter, result, show);
          }
        }
      } else {
        for testName in testNames {
         const (outputLoc, success) = compile(testName, result, testsCompiled);
        }
        if run {
          runTestBinaries(projectHome, testsCompiled, filter, result, show);
        }
      }
      timeElapsed.stop();
      if run {
        printTestResults(result, timeElapsed);
      }
    } else {
      throw new owned MasonError("No tests were found in /test");
    }
    toParse.close();
  } catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


private proc runTestBinary(outputLoc: string, testName: string, filter: string,
                           ref result, show: bool) {
  const command = outputLoc;
  var testNames: list(string),
      failedTestNames: list(string),
      erroredTestNames: list(string),
      testsPassed: list(string),
      skippedTestNames: list(string);
  var localesCountMap: map(int, int, parSafe=false);
  const exitCode =
    runAndLog(command, testName + ".chpl", filter, result, numLocales,
              testsPassed, testNames, localesCountMap,
              failedTestNames, erroredTestNames, skippedTestNames, show);
  if exitCode != 0 {
    var newCommand = " ".join(command,"-nl","1");
    if filter != "" then newCommand += " --filter=" + filter;
    const testResult = runWithStatus(newCommand, !show);
    if testResult != 0 {
      const errMsg = testName: string +
                     " returned exitCode = " + testResult: string;
      result.addFailure(testName, testName+".chpl", errMsg);
    } else {
      result.addSuccess();
    }
  }
}


private proc runTestBinaries(projectHome: string, testNames: list(string),
                             filter: string, ref result, show: bool) {

  const cwd = here.cwd();
  for test in testNames {
    var testTemp: string = test;
    if cwd == projectHome && customTest {
      testTemp = relPath(testTemp,"test/");
    }
    const outputLoc = projectHome +
                      "/target/test/" + stripExt(testTemp, ".chpl");
    const testName = basename(stripExt(test, ".chpl"));
    runTestBinary(outputLoc, testName, filter, result, show);
  }
}


private proc printTestResults(ref result, timeElapsed) {

  result.printErrors();
  writeln(result.separator2);
  result.printResult(timeElapsed.elapsed());
  exit(!result.wasSuccessful():int);
}


private proc getTests(lock: borrowed Toml, projectHome: string) {
  var testNames: list(string);
  const testPath = joinPath(projectHome, "test");

  if const testsToml = lock.get("root.tests") {
    var tests = testsToml.toString();
    var strippedTests = tests.split(',').strip('[]');
    for test in strippedTests {
      const t = test.strip().strip('"');
      testNames.pushBack(t);
    }
  } else if isDir(testPath) {
    var tests = findFiles(startdir=testPath, recursive=true, hidden=false);
    for test in tests {
      if test.endsWith(".chpl") {
        testNames.pushBack(getTestPath(test));
      }
    }
  }
  return testNames;
}

/* Gets the path of the test following the test dir */
proc getTestPath(fullPath: string, testPath = "") : string {
  var split = splitPath(fullPath);
  if split[1] == "test" {
    return testPath;
  } else {
    if testPath == "" {
      return getTestPath(split[0], split[1]);
    } else {
      var appendedPath = joinPath(split[1], testPath);
      return getTestPath(split[0], appendedPath);
    }
  }
}

/* Gets the comm */
proc getRuntimeComm() throws {
  var line: string;
  var python: string;
  var findPython = spawn(
    [MasonUtils.CHPL_HOME:string+"/util/config/find-python.sh"],
    stdout = pipeStyle.pipe);
  while findPython.stdout.readLine(line) {
    python = line.strip();
  }

  var checkComm = spawn(
    [python, MasonUtils.CHPL_HOME:string+"/util/chplenv/chpl_comm.py"],
    stdout = pipeStyle.pipe);
  while checkComm.stdout.readLine(line) {
    comm = line.strip();
  }
  // setting communication mechanism.
  if setComm != "" {
    if comm != "none" {
      comm = setComm;
    } else {
      if setComm == "none" then
        comm = setComm;
      else {
        throw new MasonError(
          "Trying to execute in a multiLocale environment when " +
          "communication mechanism is `none`.\n"+
          "Try changing the communication mechanism"
        );
      }
    }
  }
}

proc runUnitTest(cmdLineCompopts: list(string), filter: string, show: bool) {
  var comm_c: c_ptrConst(c_char);
  try! {
    var checkChpl = spawn(["which", "chpl"], stdout = pipeStyle.pipe);
    checkChpl.wait();
    var line: string;
    if checkChpl.stdout.readLine(line) {

      if files.size == 0 && dirs.size == 0 {
        dirs.pushBack(".");
      }

      var result =  new TestResult();
      var timeElapsed = new stopwatch();
      timeElapsed.start();
      for tests in files {
        try {
          testFile(tests, cmdLineCompopts, filter, result, show);
        } catch e {
          writeln("Caught an Exception in Running Test File: ", tests);
          writeln(e);
        }
      }

      for dir in dirs {
        try {
          testDirectory(dir, cmdLineCompopts, filter, result, show);
        } catch e {
          writeln("Caught an Exception in Running Test Directory: ", dir);
          writeln(e);
        }
      }
      timeElapsed.stop();
      printTestResults(result, timeElapsed);
    } else {
      writeln("chpl not found.");
      exit(2);
    }
  }

}

@chpldoc.nodoc
/*Docs: Todo*/
proc testFile(file, const ref compopts: list(string),
              filter:string, ref result, show: bool) throws {
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

  var compCommand = new list(string);
  compCommand.pushBack(["chpl", file, "-o", executable, "--comm", comm]);
  compCommand.pushBack(compopts);
  const compilation = runWithStatus(compCommand.toArray(), !show);

  if compilation != 0 {
    stderr.writeln("compilation failed for " + fileName);
    const errMsg = fileName +" failed to compile";
    result.addError(executable, fileName,  errMsg);
  } else {
    if show then writeln("\nCompiled '", fileName, "' successfully");
    var testNames: list(string),
        failedTestNames: list(string),
        erroredTestNames: list(string),
        testsPassed: list(string),
        skippedTestNames: list(string);
    var localesCountMap: map(int, int, parSafe=false);
    const exitCode =
      runAndLog("./" + executable, fileName, filter, result,
                numLocales, testsPassed, testNames, localesCountMap,
                failedTestNames, erroredTestNames, skippedTestNames, show);
    if exitCode != 0 {
      var command = " ".join("./" + executable,"-nl","1");
      if filter != "" then command += " --filter=" + filter;
      const testResult = runWithStatus(command, !show);
      if testResult != 0 {
        const errMsg =
          executable:string + " returned exitCode = " + testResult:string;
        result.addFailure(executable, fileName, errMsg);
      } else {
        result.addSuccess();
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

@chpldoc.nodoc
/*Docs: Todo*/
proc testDirectory(dir, const ref compopts: list(string),
                   filter: string, ref result, show: bool) throws {
  for file in findFiles(startdir = dir, recursive = subdir) {
    if file.endsWith(".chpl") {
      testFile(file, compopts, filter, result, show);
    }
  }
}

@chpldoc.nodoc
/*Docs: Todo*/
proc runAndLog(executable, fileName, filter: string, ref result,
               reqNumLocales: int = numLocales,
               ref testsPassed, ref testNames, ref localesCountMap,
               ref failedTestNames, ref erroredTestNames,
               ref skippedTestNames, show: bool): int throws {
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
  if testNames.size != 0 then
    testNamesStr =        try! "%?".format(testNames.toArray());
  if failedTestNames.size != 0 then
    failedTestNamesStr =  try! "%?".format(failedTestNames.toArray());
  if erroredTestNames.size != 0 then
    erroredTestNamesStr = try! "%?".format(erroredTestNames.toArray());
  if testsPassed.size != 0 then
    passedTestStr =       try! "%?".format(testsPassed.toArray());
  if skippedTestNames.size != 0 then
    skippedTestNamesStr = try! "%?".format(skippedTestNames.toArray());
  var lst = new list([executable, "-nl", reqNumLocales: string]);
  if filter != "" then
    lst.pushBack("--filter="+filter);
  lst.pushBack(
    ["--testNames",testNamesStr,"--failedTestNames", failedTestNamesStr,
     "--errorTestNames", erroredTestNamesStr, "--ranTests", passedTestStr,
     "--skippedTestNames", skippedTestNamesStr]
  );
  var exec =
    spawn(lst.toArray(),
          stdout = pipeStyle.pipe,
          stderr = pipeStyle.pipe); //Executing the file
  //std output pipe
  while exec.stdout.readLine(line) {
    if line.strip() == separator1 then sep1Found = true;
    else if line.strip() == separator2 && sep1Found {
      var testName = try! currentRunningTests.popBack();
      if testNames.count(testName) != 0 then
        try! testNames.remove(testName);
      addTestResult(result, localesCountMap, testNames, flavour, fileName,
                testName, testExecMsg, failedTestNames, erroredTestNames,
                skippedTestNames, testsPassed, show);
      testExecMsg = "";
      sep1Found = false;
    } else if line.startsWith("Flavour") {
      var temp = line.strip().split(":");
      flavour = temp[1].strip();
      testExecMsg = "";
    } else if sep1Found then testExecMsg += line;
    else {
      if line.strip().endsWith("()") {
        var testName = line.strip();
        if currentRunningTests.count(testName) == 0 {
          currentRunningTests.pushBack(testName);
          if testNames.count(testName) == 0 then
            testNames.pushBack(testName);
        }
        testExecMsg = "";
      }
    }
  }
  //this is to check the error
  if exec.stderr.readLine(line) {
    var testErrMsg = line;
    while exec.stderr.readLine(line) do testErrMsg += line;
    if !currentRunningTests.isEmpty() {
      var testNameIndex = try! currentRunningTests.popBack();
      var testName = testNameIndex;
      if testNames.count(testName) != 0 then
        try! testNames.remove(testName);
      erroredTestNames.pushBack(testName);
      if show then writeln("Ran ",testName," ERROR");
      result.addError(testName, fileName, testErrMsg);
      haltOccured =  true;
    }
  }
  exec.wait();//wait till the subprocess is complete
  exitCode = exec.exitCode;
  if haltOccured {
    exitCode =
      runAndLog(executable, fileName, filter, result, reqNumLocales,
                testsPassed, testNames, localesCountMap,
                failedTestNames, erroredTestNames, skippedTestNames, show);
  }
  if testNames.size != 0 {
    var maxCount = -1;
    for key in localesCountMap.keys() {
      if maxCount < localesCountMap[key] {
        reqLocales = key;
        maxCount = localesCountMap[key];
      }
    }
    localesCountMap.remove(reqLocales);
    exitCode =
      runAndLog(executable, fileName, filter, result, reqLocales,
                testsPassed, testNames, localesCountMap, failedTestNames,
                erroredTestNames, skippedTestNames, show);
  }
  return exitCode;
}

@chpldoc.nodoc
/*Docs: Todo*/
proc addTestResult(ref result, ref localesCountMap, ref testNames,
                  flavour, fileName, testName, errMsg, ref failedTestNames,
                  ref erroredTestNames, ref skippedTestNames, ref testsPassed,
                  show: bool) throws {
  select flavour {
    when "OK" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addSuccess();
      testsPassed.pushBack(testName);
    }
    when "ERROR" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addError(testName, fileName, errMsg);
      erroredTestNames.pushBack(testName);
    }
    when "FAIL" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addFailure(testName, fileName, errMsg);
      failedTestNames.pushBack(testName);
    }
    when "SKIPPED" {
      if show then writeln("Ran ",testName," ",flavour);
      result.addSkip(testName, fileName, errMsg);
      skippedTestNames.pushBack(testName);
    }
    when "IncorrectNumLocales" {
      if comm != "none" {
        var strSplit = errMsg.split("=");
        var reqLocalesStr = strSplit[1].strip().split(",");
        for a in reqLocalesStr do
          if localesCountMap.contains(a: int) then
            localesCountMap[a: int] += 1;
          else
            localesCountMap[a: int] = 1;
        testNames.pushBack(testName);
      } else {
        const locErrMsg =
          "Not a MultiLocale Environment. $CHPL_COMM = " + comm + "\n" + errMsg;
        result.addSkip(testName, fileName, locErrMsg);
        skippedTestNames.pushBack(testName);
      }
    }
    when "Dependence" {
      testNames.pushBack(testName);
    }
  }
}
