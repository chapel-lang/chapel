/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

use TOML;
use Path;
use Spawn;
use MasonEnv;
use MasonNew;
use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use MasonModify;
use FileSystem;
use MasonTest;
private use List;
private use Map;

var files : list(string);
var dirs : list(string);
var customBenchTest: bool=false;

proc masonBench(args) throws {
  var show: bool = false;
  var skipUpdate = MASON_OFFLINE;
  var compopts: list(string);
  var countArgs = args.indices.low+2;

  for arg in args[args.indices.low+2..] {
    countArgs += 1;
    select(arg) {
      when '-h' {
        masonBenchHelp();
        exit(0);
      }
      when '--help' {
        masonBenchHelp();
        exit(0);
      }
      when '--show' {
        show = true;
      }
      when '--' {
        throw new owned MasonError("Benchmarking doesn't support -- syntax");
      }
      when '--update' {
        skipUpdate = false;
      }
      when '--no-update' {
        skipUpdate = true;
      }
      otherwise {
        try! {
          // bench filenames, directories, compopts passed as args
          if isFile(arg) && arg.endsWith(".chpl") then files.append(arg);
          else if isDir(arg) then dirs.append(arg);
          else {
            if arg.startsWith('-') then compopts.append(arg);
          } 
        }
      }
    }
  }
  try! {
    getRuntimeComm();
    updateLock(skipUpdate);
    compopts.append("".join('--comm=', comm));
    runBenchTests(show, compopts);
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* displays list of bench test files derived from bench/, Mason.toml file */
proc printMetadata(compopts: list(string), benchTestsFromDir: list(string)) {
  writeln("Benchmarking tests gathered from lockfile & benchmark directory...");
  for test in benchTestsFromDir do writeln(test);
  writeln("Compopts...");
  for c in compopts do writeln(c);
}

/* run the bench tests gathered */
proc runBenchTests(show: bool, ref cmdLineCompopts: list(string)) throws {
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.lock" , iomode.r);
    const lockFile = owned.create(parseToml(toParse));
    const sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);
    const project = lockFile["root"]!["name"]!.s;
    const projectPath = "".join(projectHome, "/src/", project, ".chpl");
    const compopts = getTomlCompopts(lockFile, cmdLineCompopts);
    if isDir(joinPath(projectHome, "target/benchmark/")) {
      rmTree(joinPath(projectHome, "target/benchmark/"));
    }
    makeTargetFiles("debug", projectHome);
    var numBenchTests: int;
    var benchTestNames: list(string);
    var benchTestsCompiled: list(string);
    if (files.size == 0 && dirs.size == 0) {
      benchTestNames = getTests(lockFile.borrow(), projectHome, true);
      numBenchTests = benchTestNames.size;
    } else {
      try! {
        for dir in dirs {
          for file in findfiles(startdir = dir, recursive=subdir) {
            if file.endsWith(".chpl") then files.append(file);
          }
        }
      }
      benchTestNames = files;
      numBenchTests = files.size;
      customBenchTest = true;
    }
    writeln("*********** Benchmark Results ****************");
    if numBenchTests > 0 {
      for benchTest in benchTestNames {
        var benchTestPath: string;
        if customBenchTest then benchTestPath = "".join(cwd, "/", benchTest);
        else benchTestPath = "".join(projectHome, "/benchmark/", benchTest);
        const benchTestName = basename(stripExt(benchTest, ".chpl"));
        const masonCompopts = getMasonDependencies(sourceList, benchTestName);
        const allCompopts = "".join(" ".join(compopts.these()), masonCompopts);
        var benchTestTemp: string = benchTest;
        if cwd == projectHome && customTest then
          benchTestTemp = relPath(benchTestTemp, "benchmark/");
        const outputLoc = projectHome + "/target/benchmark/" + stripExt(benchTestTemp, ".chpl");
        if !isDir(dirname(outputLoc)) then mkdir(dirname(outputLoc));
        const moveTo = "-o " + outputLoc;
        const compCommand = " ".join("chpl --fast", benchTestPath, projectPath, moveTo, allCompopts);
        const compilation = runWithStatus(compCommand, show);
        if compilation != 0 {
          stderr.writeln("Compilation failed for " + benchTest);
          const errMsg = benchTest + " failed to compile";
          // TODO:  add to results from unit test 
        } else {
          writeln("Compiled " + benchTest + " successfully");
          runAndLogBenchmarks(projectHome, outputLoc, benchTestName);
        }
        writeln("**********************************************");
      }  
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
  }
}

/* Execute benchmarks from target/ and log output */
proc runAndLogBenchmarks(projectHome: string, outputLoc: string, benchTestName: string) {
  var executable = outputLoc;
  var line: string;
  var exec = spawn([executable]);
  exec.wait();
}
