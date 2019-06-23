/*
  This launcher will be used to run the test files and catch the halts.
  You can specify a different directory name using the `--dir` 
  You can set whether or not to descend recursively into 
  subdirectories (defaults to true) using `--subdir`
*/
use FileSystem;
use Spawn;
use Path;
use TestResult;
config const subdir: string = "true";
config const keepExec: string = "false";

/*Docs: Todo*/
proc run_and_log(executable,fileName, ref result,skipId = 0) {
  var separator1 = result.separator1,
      separator2 = result.separator2;
  var testName: string,
      flavour: string,
      line: string,
      tempString: string;
  var curIndex = 0;
  var sep1Found = false,
      haltOccured = false;
  
  var exec = spawn(["./"+executable,"--skipId",skipId:string], stdout = PIPE, stderr = PIPE); //Executing the file
  //std output pipe
  while exec.stdout.readline(line) {
    if line.strip() == separator1 then sep1Found = true;
    else if line.strip() == separator2 && sep1Found {
      if flavour == "ERROR" then result.addError(testName, tempString);
      else if flavour == "FAIL" then result.addFailure(testName, tempString);
      else if flavour == "SKIPPED" then result.addSkip(testName,tempString);
      tempString = "";
      sep1Found = false;
    }
    else if sep1Found then tempString+=line;
    else {
      var temp = line.strip().split(":");
      if temp[1].strip().endsWith(")") {
        var strSplit = temp[1].strip().split(")");
        var testNameIndex = strSplit[1].split("(");
        testName = fileName+": "+testNameIndex[1];
        curIndex = testNameIndex[2]: int;
        result.startTest();
        if temp.size > 1 {
          flavour = temp[2].strip();
          if flavour == "OK" then result.addSuccess(testName);
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
      result.addError(testName,tempString);
      haltOccured =  true;
    }
  }
  exec.wait();//wait till the subprocess is complete
  if haltOccured then run_and_log(executable, fileName, result, curIndex);
}

/*Docs: Todo*/
proc test_file(file, ref result) {
  var fileName = basename(file);
  if fileName.endsWith(".chpl") && fileName.startsWith("test") {
    var line: string;
    var compErr = false;
    var tempName = fileName.split(".chpl");
    var executable = tempName[1];
    if isFile(executable) then
      var execRem = spawn(["rm",executable]);
    var sub = spawn(["chpl",file,"-o",executable,"-M."],stderr = PIPE); //Compiling the file
    if sub.stderr.readline(line) {
      writeln(line);
      compErr = true;
    }
    sub.wait();
    if !compErr {
      run_and_log(executable,fileName,result);
      if !keepExec:bool then 
        var execRem = spawn(["rm",executable]);
    }
    else {
      writeln("Compilation Error in ",fileName);
      writeln("Possible Reasons can be passing a non-test function to UnitTest.runTest()");
    }
  }
}

/*Docs: Todo*/
proc test_directory(dir, ref result) {
  for file in findfiles(startdir = dir, recursive = subdir:bool) {
    test_file(file, result);
  }
}

proc main(args: [] string) {
  var dirs: [1..0] string,
      files: [1..0] string;
  var had_invalid_file = false;

  for i in 1..(args.size-1) {
    var a = args[i];
    if isFile(a) {
      files.push_back(a);
    }
    else if isDir(a) {
      dirs.push_back(a);
    }
    else {
      writeln("[Error: ",a," is not a valid file or directory]");
      had_invalid_file = true;
    }
  }

  if had_invalid_file && files.size == 0 && dirs.size == 0 {
    exit(2);
  }

  if files.size == 0 && dirs.size == 0 {
    dirs.push_back(".");
  }
  
  var result =  new TestResult();

  for tests in files {
    test_file(tests, result);
  }

  for dir in dirs {
    test_directory(dir, result);
  }
    
  result.printErrors();
  writeln(result.separator2);
  result.PrintResult();
}