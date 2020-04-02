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


private use List;
private use Map;
use TOML;
use Spawn;
use MasonUtils;
use MasonHelp;
use MasonUpdate;
use MasonBuild;
use Path;
use FileSystem;
use MasonEnv;

/* Runs the .chpl files found within the /example directory */
proc masonExample(args) {

  var show = false;
  var run = true;
  var build = true;
  var release = false;
  var force = false;
  var noUpdate = false;
  var update = false;
  var examples: list(string); 
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
    else if arg == '--no-update' {
      noUpdate = true;
    }
    else if arg == '--update' {
      update = true;
    }
    else if arg == '--example' {
      continue;
    }
    else if arg == '--build' {
      continue;
    }
    else {
      examples.append(arg);
    }
  }
  var uargs: list(string);
  if (!build || noUpdate) then uargs.append("--no-update");
  else {
    if MASON_OFFLINE && update {
      uargs.append('--update');
    }
  }
  UpdateLock(uargs);
  runExamples(show, run, build, release, force, examples);
}


private proc getBuildInfo(projectHome: string) {

  // parse lock and toml(examples dont make it to lock file)
  const lock = open(projectHome + "/Mason.lock", iomode.r);
  const toml = open(projectHome + "/Mason.toml", iomode.r);
  const lockFile = owned.create(parseToml(lock));
  const tomlFile = owned.create(parseToml(toml));
  
  // Get project source code and dependencies
  const sourceList = genSourceList(lockFile);

  //
  // TODO: Temporarily use `toArray` here because `list` does not yet
  // support parallel iteration, which the `getSrcCode` method _must_
  // have for good performance.
  //
  getSrcCode(sourceList, false);
  const project = lockFile["root"]!["name"]!.s;
  const projectPath = "".join(projectHome, "/src/", project, ".chpl");
  
  // get the example names from lockfile or from example directory
  const exampleNames = getExamples(tomlFile.borrow(), projectHome);

  var emptyCompopts = new list(string);
  emptyCompopts.append("");

  // Get system, and external compopts
  const compopts = getTomlCompopts(lockFile.borrow(), emptyCompopts);
  const perExampleOptions = getExampleOptions(tomlFile.borrow(), exampleNames);

  // Close lock and toml
  lock.close();
  toml.close();


  return (sourceList, projectPath, compopts, exampleNames, perExampleOptions);
}

// retrieves compopts and execopts for each example.
// returns assoc array of <example_name> -> <(compopts, execopts)>
private proc getExampleOptions(toml: Toml, exampleNames: list(string)) {

  var exampleOptions = new map(string, (string, string));
  for example in exampleNames {
    const exampleName = basename(stripExt(example, ".chpl"));
    exampleOptions[exampleName] = ("", "");
    if toml.pathExists("".join("examples.", exampleName, ".compopts")) {
      var compopts = toml["".join("examples.", exampleName)]!["compopts"]!.s;
      exampleOptions[exampleName][0] = compopts;
    }
    if toml.pathExists("".join("examples.", exampleName, ".execopts")) {
      var execopts = toml["".join("examples.", exampleName)]!["execopts"]!.s;
      exampleOptions[exampleName][1] = execopts;
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
private proc determineExamples(exampleNames: list(string),
                               examplesRequested: list(string)) throws {

  var examplesToRun: list(string);

  // check if user listed examples actually exist
  if examplesRequested.size > 0 {
    for example in examplesRequested {
      if exampleNames.count(example) == 0 {
        throw new owned MasonError("Mason could not find example: " + example);
      }
      else {
        examplesToRun.append(example);
      }
    }
    return examplesToRun;
  }
  // user didnt list any examples, run all examples
  else return exampleNames;
}


private proc runExamples(show: bool, run: bool, build: bool, release: bool,
                         force: bool, examplesRequested: list(string)) throws {

  try! {

    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);

    // Get buildInfo: dependencies, path to src code, compopts,
    // names of examples, example compopts
    var buildInfo = getBuildInfo(projectHome);
    const sourceList = buildInfo[0];
    const projectPath = buildInfo[1];
    const compopts = buildInfo[2];
    const exampleNames = buildInfo[3];
    const perExampleOptions = buildInfo[4];
    const projectName = basename(stripExt(projectPath, ".chpl"));
    
    var numExamples = exampleNames.size;
    var examplesToRun = determineExamples(exampleNames, examplesRequested);

    // Clean out example binaries from previous runs
    if build then setupExampleDir(projectHome);

    if numExamples > 0 {
      for example in examplesToRun {

        const examplePath = "".join(projectHome, '/example/', example);
        const exampleName = basename(stripExt(example, ".chpl"));

        // retrieves compopts and execopts found per example in the toml file      
        const optsFromToml = perExampleOptions[exampleName];
        var exampleCompopts = optsFromToml[0];
        var exampleExecopts = optsFromToml[1];

        if release then exampleCompopts += " --fast";

        if build {  
          if exampleModified(projectHome, projectName, example) || force { 

            // remove old binary
            removeExampleBinary(projectHome, exampleName);

            // get the string of dependencies for compilation
            // also names example as --main-module
            const masonCompopts = getMasonDependencies(sourceList, exampleName);
            var allCompOpts = " ".join(" ".join(compopts.these()), masonCompopts,
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
      throw new owned MasonError("No examples were found in /example");
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
    throw new owned MasonError("Example has not been compiled: " + exampleName + ".chpl\n" +
    "Try running: mason build --example " + exampleName + ".chpl\n" +
    "         or: mason run --example " + exampleName + ".chpl --build");
  }
}  


private proc getMasonDependencies(sourceList: list(3*string),
                                 exampleName: string) {

  // Declare example to run as the main module
  var masonCompopts = " ".join(" --main-module", exampleName, " ");

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

private proc getExamples(toml: Toml, projectHome: string) {
  var exampleNames: list(string);
  const examplePath = joinPath(projectHome, "example");

  if toml.pathExists("examples.examples") {

    var examples = toml["examples"]!["examples"]!.toString();
    var strippedExamples = examples.split(',').strip('[]');
    for example in strippedExamples {
      const t = example.strip().strip('"');
      exampleNames.append(t);
    }
    return exampleNames;
  }
  else if isDir(examplePath) {
    var examples = findfiles(startdir=examplePath, recursive=true, hidden=false);
    for example in examples {
      if example.endsWith(".chpl") {
        exampleNames.append(getExamplePath(example));
      }
    }
    return exampleNames;
  }
  return exampleNames;
}

/* Gets the path of the example by following the example dir */
proc getExamplePath(fullPath: string, examplePath = "") : string {
  var split = splitPath(fullPath);
  if split[1] == "example" {
    return examplePath;
  }
  else {
    if examplePath == "" {
      return getExamplePath(split[0], split[1]);
    }
    else {
      var appendedPath = joinPath(split[1], examplePath);
      return getExamplePath(split[0], appendedPath);
    }
  }
}

// used when user calls `mason run --example` without argument
proc printAvailableExamples() {
  try! {
    const cwd = getEnv("PWD");
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    const toml = owned.create(parseToml(toParse));
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
    // check for binary existence
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
