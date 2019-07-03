/*
  This launcher will be used to run the test files and catch the halts.
  You can set whether or not to descend recursively into 
  subdirectories (defaults to true) using `--subdir`
*/
module Launcher {
  use FileSystem;
  use Spawn;
  use Path;
  use TestResult;
  use Sys;
  use Help;
  config const subdir = true;
  config const keepExec = false;
  config const setComm: string = "";
  var comm: string;

  proc main(args: [] string) {
    var comm_c: c_string;
    sys_getenv("CHPL_COMM",comm_c);
    comm = comm_c: string;
    var dirs: [1..0] string,
        files: [1..0] string;
    var hadInvalidFile = false;
    var programName = args[0];
    for a in args[1..] {
      if a == "-h" || a == "--help" {
        writeln("Usage: ", programName, " <options> filename [filenames] directoryname [directorynames]");
        printUsage();
        exit(1); // returning 1 from main is also an option
      }
      else {
        try! {
          if isFile(a) {
            files.push_back(a);
          }
          else if isDir(a) {
            dirs.push_back(a);
          }
          else {
            writeln("[Error: ",a," is not a valid file or directory]");
            hadInvalidFile = true;
          }
        }
      }
    }

    if hadInvalidFile && files.size == 0 && dirs.size == 0 {
      exit(2);
    }

    if files.size == 0 && dirs.size == 0 {
      dirs.push_back(".");
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

  pragma "no doc"
  /*Docs: Todo*/
  proc runAndLog(executable,fileName, ref result,skipId = 0,
                  reqNumLocales:int = numLocales, ref testNames,
                  ref dictDomain,ref dict) throws {
    var separator1 = result.separator1,
        separator2 = result.separator2;
    var testName: string,
        flavour: string,
        line: string,
        tempString: string;
    var curIndex = 0,
        reqLocales = 0;
    var sep1Found = false,
        haltOccured = false;
    var testNamesStr = "None";
    if testNames.size!=0 then testNamesStr = testNames:string;
    var exec = spawn(["./"+executable,"--skipId",skipId: string,"-nl",
                      reqNumLocales: string,"--testNames",testNamesStr],
                      stdout = PIPE, stderr = PIPE); //Executing the file
    //std output pipe
    while exec.stdout.readline(line) {
      if line.strip() == separator1 then sep1Found = true;
      else if line.strip() == separator2 && sep1Found {
        var checkStatus = testNames.find(testName);
        if checkStatus[1] then
          testNames.remove(checkStatus[2]);
        select flavour {
          when "ERROR" do result.addError(fileName+": "+testName, tempString);
          when "FAIL" do result.addFailure(fileName+": "+testName, tempString);
          when "SKIPPED" do result.addSkip(fileName+": "+testName,tempString);
          when "IncorrectNumLocales" {
            if comm != "none" {
              result.testToBeReRan();
              var strSplit = tempString.split("=");
              var reqLocalesStr = strSplit[2].split(" ");
              for a in reqLocalesStr do
                if dictDomain.contains(a:int) then
                  dict[a:int] += 1;
                else
                  dict[a:int] = 1;
              testNames.push_back(testName);
            }
            else {
              var tempStr = "Not a MultiLocale Environment. $CHPL_COMM = "+comm+"\n";
              tempStr += tempString; 
              result.addFailure(fileName+": "+testName, tempStr);
            }
          }
        }
        tempString = "";
        sep1Found = false;
      }
      else if sep1Found then tempString+=line;
      else {
        var temp = line.strip().split(":");
        if temp[1].strip().endsWith("]") {
          var strSplit = temp[1].strip().split("]");
          var testNameIndex = strSplit[1].split("[");
          testName = testNameIndex[1];
          curIndex = testNameIndex[2]: int;
          result.startTest();
          if temp.size > 1 {
            flavour = temp[2].strip();
            if flavour == "OK" {
              result.addSuccess(fileName+": "+testName);
              var checkStatus = testNames.find(testName);
              if checkStatus[1] then
                testNames.remove(checkStatus[2]);
            }
          }
          tempString = "";
        }  
      }
    }
    //this is to check the error
    if exec.stderr.readline(line) { 
      tempString = line;
      while exec.stderr.readline(line) do tempString+=line;
      if testName!="" {
        result.addError(fileName+": "+testName, tempString);
        haltOccured =  true;
      }
    }
    exec.wait();//wait till the subprocess is complete
    if haltOccured then
      runAndLog(executable, fileName, result,curIndex,
                reqNumLocales, testNames,dictDomain, dict);
    if testNames.size != 0 {
      var maxCount = -1;
      for key in dictDomain.sorted() {
        if maxCount < dict[key] {
          reqLocales = key;
          maxCount = dict[key];
        }
      }
      dictDomain.remove(reqLocales);
      runAndLog(executable, fileName, result, 0, 
                reqLocales, testNames, dictDomain, dict);
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
      var executableReal = executable+"_real";
      if isFile(executable) {
        FileSystem.remove(executable);
      }
      if isFile(executableReal) {
        FileSystem.remove(executableReal);
      }
      if setComm!="" then comm = setComm;
      var sub = spawn(["chpl",file,"-o",executable,"-M.",
                    "--comm",comm],stderr = PIPE); //Compiling the file
      if sub.stderr.readline(line) {
        writeln(line);
        compErr = true;
      }
      sub.wait();
      if !compErr {
        var testNames: [1..0] string;
        var dictDomain: domain(int);
        var dict: [dictDomain] int;
        runAndLog(executable,fileName,result,0,numLocales,
                  testNames, dictDomain, dict);
        if !keepExec:bool {
          FileSystem.remove(executable);
          if isFile(executableReal) {
            FileSystem.remove(executableReal);
          }
        }
      }
      else {
        writeln("Compilation Error in ",fileName);
        writeln("Possible Reasons can be passing a non-test ",
                "function to UnitTest.runTest()");
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
}