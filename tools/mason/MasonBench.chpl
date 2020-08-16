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
    // get bench files from bench directory 
    const cwd = getEnv("PWD"); 
    const projectHome = getProjectHome(cwd);
    var benchTestsFromDir: list(string);
    var benchTestPath = joinPath(projectHome, "benchmark");
    var benchTests = findfiles(startdir=benchTestPath, recursive=true, hidden=false);
    for bench in benchTests {
      if bench.endsWith(".chpl") {
        benchTestsFromDir.append(bench);
      }
    }
    // get bench files from Mason.toml or any toml files
    var benchTestsFromToml: list(string);
    const tomlFilePath = joinPath(projectHome, 'Mason.toml');
    const toParse = open(tomlFilePath, iomode.r);
    const tomlFile = owned.create(parseToml(toParse));
    if isFile(tomlFilePath) {
      if tomlFile.pathExists('brick.benchmarks') {
        var benchTests = tomlFile['brick']!['benchmarks']!.toString(); 
        var stripBenchTests = benchTests.split(',').strip('[]');
        for test in stripBenchTests {
          const benchTest = test.strip().strip('"');
          benchTestsFromToml.append(benchTest);
        }
      }
    }
    if show {
      printMetadata(compopts, benchTestsFromDir, benchTestsFromToml); 
    }
    updateLock(skipUpdate);
    runBenchTests(show, compopts, benchTestsFromDir, benchTestsFromToml);
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/* displays list of bench test files derived from bench/, Mason.toml file */
proc printMetadata(compopts: list(string), benchTestsFromDir: list(string), benchTestsFromToml: list(string)) {
  writeln("Bench Test files from bench directory ... ");
  for benchTest in benchTestsFromDir do writeln(benchTest);
  writeln("Bench Test files from Mason.toml...");
  for benchTest in benchTestsFromToml do writeln(benchTest);
}

/* run the bench tests gathered */
proc runBenchTests(show:bool, ref cmdLineCompopts: list(string),
    benchTestsFromDir: list(string), benchTestsFromToml: list(string)) throws {
	try! {
		if show then writeln("Running benchmarks ...");
		const cwd = getEnv("PWD"); 
		const projectHome = getProjectHome(cwd);
		const toParse = open(projectHome + "/Mason.lock", iomode.r);
		const lockFile = owned.create(parseToml(toParse));
		const sourceList = genSourceList(lockFile);
		getSrcCode(sourceList, show);
		const project = lockFile["root"]!["name"]!.s;
		const projectPath = "".join(projectHome, "/src/", project, ".chpl");
		const compopts = getTomlCompopts(lockFile, cmdLineCompopts);
		if isDir(joinPath(projectHome, "target/benchmark/")) {
			rmTree(joinPath(projectHome, "target/benchmark/"));
		}
		// Make target files if they dont exist from a build
		makeTargetFiles("debug", projectHome);
		if show then writeln("Made target files...");
		if(benchTestsFromToml.size > 0) {
			// if benchTests found in Toml execute them only
			// check if file actually exists in bench/ else show error that a file doesnt exist  
			var invalidTests: list(string);
			for bT in benchTestsFromToml {
				var testPresentInDir: bool = false;
				for benchT in benchTestsFromDir {
					if(bT == basename(benchT)) then testPresentInDir = true;
				}
				if(testPresentInDir == false) then invalidTests.append(bT);
			}
			if invalidTests.size > 0 {
				for test in invalidTests {
					if invalidTests.size == 1 then write(test);
					else write(test, ', ');
				}
				throw new owned MasonError("from Mason.toml could not be found in your benchmarks/ directory.");
			} else {
				// run benchmarks
			}
		} else {
			// otherwise run other benchTests

		}
	}
	catch e: MasonError {
		writeln(e.message());
		exit(1);
	}
}

