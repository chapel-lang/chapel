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

/* Runs the .chpl files found within the /example directory */
proc masonExample(args) {

  var show = false;
  var run = true;
  var build = true;
  var release = false;
  var force = false;
  var examples: [1..0] string;

  for arg in args {
    if arg == '--show' {
      show = true;
    }
    else if arg == '--no-run' {
      run = false;
    }
    else if arg == '--no-build' {
      build = false;
    }
    else if arg == '--release' {
      release = true;
    }
    else if arg == '--force' {
      force = true;
    }
    else if arg == '--example' {
      continue;
    }
    else if arg == '--build' {
      continue;
    }
    else {
      examples.push_back(arg);
    }
  }
  var uargs: [1..0] string;
  if !build then uargs.push_back("--no-update");  
  UpdateLock(uargs);
  runExamples(show, run, build, release, force, examples);
}


private proc getBuildInfo(projectHome: string) {

  // parse lock and toml(examples dont make it to lock file)
  const lock = open(projectHome + "/Mason.lock", iomode.r);
  const toml = open(projectHome + "/Mason.toml", iomode.r);
  const lockFile = new owned(parseToml(lock));
  const tomlFile = new owned(parseToml(toml));
  
  // Get project source code and dependencies
  const sourceList = genSourceList(lockFile);
  getSrcCode(sourceList, false);
  const project = lockFile["root"]["name"].s;
  const projectPath = "".join(projectHome, "/src/", project, ".chpl");
  
  // get the example names from lockfile or from example directory
  const exampleNames = getExamples(tomlFile.borrow(), projectHome);

  // Get system, and external compopts
  const compopts = getTomlCompopts(lockFile.borrow(), [""]);
  const perExampleOptions = getExampleOptions(tomlFile.borrow(), exampleNames);

  // Close lock and toml
  lock.close();
  toml.close();


  return (sourceList, projectPath, compopts, exampleNames, perExampleOptions);
}

// retrieves compopts and execopts for each example.
// returns assoc array of <example_name> -> <(compopts, execopts)>
private proc getExampleOptions(toml: Toml, exampleNames: [?d] string) {

  var exampleDomain: domain(string);
  var exampleOptions: [exampleDomain] (string, string);
  for example in exampleNames {
    const exampleName = basename(stripExt(example, ".chpl"));
    exampleOptions[exampleName] = ("", "");
    if toml.pathExists("".join("examples.", exampleName, ".compopts")) {
      var compopts = toml["".join("examples.", exampleName)]["compopts"].s;
      exampleOptions[exampleName][1] = compopts;
    }
    if toml.pathExists("".join("examples.", exampleName, ".execopts")) {
      var execopts = toml["".join("examples.", exampleName)]["execopts"].s;
      exampleOptions[exampleName][2] = execopts;
    }
  }
  return exampleOptions;
}

// Cleans out example dir from a previous run
private proc setupExampleDir(projectHome: string) {

  const exampleDir = joinPath(projectHome, "target/example/");
  if !isDir(exampleDir) {
    makeTargetFiles("debug", projectHome);
  }
}

// prevent building one example from removing all.
private proc removeExampleBinary(projectHome: string, exampleName: string) {

  const exampleDir = joinPath(projectHome, "target/example/");
  if isDir(exampleDir) {
    const exampleBinPath = joinPath(exampleDir, exampleName);
    if isFile(exampleBinPath) {
      remove(exampleBinPath);
    }
  }
}


// Takes in examples found by mason and examples requested by user
// outputs examples that should be built/run
private proc determineExamples(exampleNames: [?d1] string,
                               examplesRequested: [?d2] string) throws {

  var examplesToRun: [1..0] string;

  // check if user listed examples actually exist
  if examplesRequested.domain.size > 0 {
    for example in examplesRequested {
      if exampleNames.count(example) == 0 {
        throw new MasonError("Mason could not find example: " + example);
      }
      else {
        examplesToRun.push_back(example);
      }
    }
    return examplesToRun;
  }
  // user didnt list any examples, run all examples
  else return exampleNames;
}


private proc runExamples(show: bool, run: bool, build: bool, release: bool,
                         force: bool, examplesRequested: [?d] string) throws {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // Get buildInfo: dependencies, path to src code, compopts,
    // names of examples, example compopts
    const buildInfo = getBuildInfo(projectHome);
    const sourceList = buildInfo[1];
    const projectPath = buildInfo[2];
    const compopts = buildInfo[3];
    const exampleNames = buildInfo[4];
    const perExampleOptions = buildInfo[5];
    const projectName = basename(stripExt(projectPath, ".chpl"));
    
    var numExamples = exampleNames.domain.size;
    var examplesToRun = determineExamples(exampleNames, examplesRequested);

    // Clean out example binaries from previous runs
    if build then setupExampleDir(projectHome);

    if numExamples > 0 {
      for example in examplesToRun {

        const examplePath = "".join(projectHome, '/example/', example);
        const exampleName = basename(stripExt(example, ".chpl"));

        // retrieves compopts and execopts found per example in the toml file      
        const optsFromToml = perExampleOptions[exampleName];
        var exampleCompopts = optsFromToml[1];
        var exampleExecopts = optsFromToml[2];

        if release then exampleCompopts += " --fast";

        if build {  
          if exampleModified(projectHome, projectName, example) || force { 

            // remove old binary
            removeExampleBinary(projectHome, exampleName);

            // get the string of dependencies for compilation
            // also names example as --main-module
            const masonCompopts = getMasonDependencies(sourceList, exampleName);
            var allCompOpts = " ".join(" ".join(compopts), masonCompopts,
                                       exampleCompopts);

            const moveTo = "-o " + projectHome + "/target/example/" + exampleName;
            const compCommand = " ".join("chpl",examplePath, projectPath,
                                         moveTo, allCompOpts);
            if show then writeln(compCommand);
            const compilation = runWithStatus(compCommand);

            if compilation != 0 {
              stderr.writeln("compilation failed for " + example);
            }
            else {
              if show || !run then writeln("compiled ", example, " successfully");
              if run {
                runExampleBinary(projectHome, exampleName, release, show, exampleExecopts);
              }
            }
          }
          // build is skipped but examples still need to be run
          else {
            writeln("Skipping "+ example + ": no changes made to project or example");
            if run {
              runExampleBinary(projectHome, exampleName, release, show, exampleExecopts);
            }
          }
        }
        // just running the example
        else {
          runExampleBinary(projectHome, exampleName, release, show, exampleExecopts);
        }
      }
    }
    else {
      throw new MasonError("No examples were found in /example");
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


private proc runExampleBinary(projectHome: string, exampleName: string,
                              release: bool, show: bool, execopts: string) throws {
  const command = "".join(projectHome,'/target/example/', exampleName, " ", execopts);
  if show {
    if release then writeln("Executing [release] target: " + command);
    else writeln("Executing [debug] target: " + command);
  }

  const exampleResult = runWithStatus(command, true);
  if exampleResult != 0 {
    throw new MasonError("Mason failed to find and run compiled example: " + exampleName + ".chpl");
  }
}  


private proc getMasonDependencies(sourceList: [?d] (string, string, string),
                                 exampleName: string) {

  // Declare example to run as the main module
  var masonCompopts = " ".join(" --main-module", exampleName, " ");

  if sourceList.numElements > 0 {
    const depPath = MASON_HOME + "/src/";

    // Add dependencies to project
    for (_, name, version) in sourceList {
      var depSrc = "".join(' ',depPath, name, "-", version, '/src/', name, ".chpl");
      masonCompopts += depSrc;
    }
  }
  return masonCompopts;
}

private proc getExamples(toml: Toml, projectHome: string) {
  var exampleNames: [1..0] string;
  const examplePath = joinPath(projectHome, "example");

  if toml.pathExists("examples.examples") {

    var examples = toml["examples"]["examples"].toString();
    var strippedExamples = examples.split(',').strip('[]');
    for example in strippedExamples {
      const t = example.strip().strip('"');
      exampleNames.push_back(t);
    }
    return exampleNames;
  }
  else if isDir(examplePath) {
    var examples = findfiles(startdir=examplePath, recursive=true, hidden=false);
    for example in examples {
      if example.endsWith(".chpl") {
        exampleNames.push_back(getExamplePath(example));
      }
    }
    return exampleNames;
  }
  return exampleNames;
}

/* Gets the path of the example by following the example dir */
proc getExamplePath(fullPath: string, examplePath = "") : string {
  var split = splitPath(fullPath);
  if split[2] == "example" {
    return examplePath;
  }
  else {
    if examplePath == "" {
      return getExamplePath(split[1], split[2]);
    }
    else {
      var appendedPath = joinPath(split[2], examplePath);
      return getExamplePath(split[1], appendedPath);
    }
  }
}

// used when user calls `mason run --example` without argument
proc printAvailableExamples() {
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    const toml = new owned(parseToml(toParse));
    const examples = getExamples(toml, projectHome);
    writeln("--- available examples ---");
    for example in examples {
      writeln(" --- " + example);
    }
    writeln("--------------------------");
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}

// Checks to see if an example, source code, or Mason.toml has been modified
proc exampleModified(projectHome: string, projectName: string,
                             exampleName: string) {
  const example = basename(stripExt(exampleName, ".chpl"));
  const exampleBinPath = joinPath(projectHome, "target/example", example);
  const examplePath = joinPath(projectHome, "example");

  // check for changes to Mason.toml and src code
  if projectModified(projectHome, example, "example") {
      return true;
  }
  else {
    // check for binary existance
     if isFile(exampleBinPath) {
      // check for changes to example
       const exModTime = getLastModified(joinPath(examplePath, exampleName));
       const exBinModTime = getLastModified(exampleBinPath);
      if exModTime > exBinModTime {
        return true;
      }
      else return false;
    }
    else return true;
  }
}
