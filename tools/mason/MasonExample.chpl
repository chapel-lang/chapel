/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
use MasonEnv;
use MasonHelp;
use MasonUpdate;
use MasonUtils;
use Path;
use Subprocess;
use TOML;


/* Runs the .chpl files found within the /example directory */
proc masonExample(args: [] string, checkProj=true) throws {

  var parser = new argumentParser();

  var runFlag = parser.addFlag(name="run",
                               defaultValue=true,
                               flagInversion=true);
  var buildFlag = parser.addFlag(name="build",
                                 defaultValue=true,
                                 flagInversion=true);

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);
  var exampleOpts = parser.addOption(name="example", numArgs=0..);

  if checkProj {
    const projectType = getProjectType();
    if projectType == "light" then
      throw new owned MasonError("Mason light projects do not currently support 'mason example'");
  }

  try! {
    parser.parseArgs(args);
  }
  catch ex : ArgumentError {
    stderr.writeln(ex.message());
    exit(1);
  }
  var show = showFlag.valueAsBool();
  var run = runFlag.valueAsBool();
  var build = buildFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var force = forceFlag.valueAsBool();
  var skipUpdate = MASON_OFFLINE;
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }
  var examples = new list(exampleOpts.values());
  updateLock(skipUpdate);
  runExamples(show, run, build, release, skipUpdate, force, examples);
}


private proc getBuildInfo(projectHome: string, skipUpdate: bool) {

  // parse lock and toml(examples dont make it to lock file)
  const lock = open(projectHome + "/Mason.lock", ioMode.r);
  const toml = open(projectHome + "/Mason.toml", ioMode.r);
  const lockFile = parseToml(lock);
  const tomlFile = parseToml(toml);

  // Get project source code and dependencies
  const (sourceList, gitList) = genSourceList(lockFile);

  //
  // TODO: Temporarily use `toArray` here because `list` does not yet
  // support parallel iteration, which the `getSrcCode` method _must_
  // have for good performance.
  //
  getSrcCode(sourceList, skipUpdate, false);
  getGitCode(gitList, false);
  const project = lockFile["root"]!["name"]!.s;
  const projectPath = "".join(projectHome, "/src/", project, ".chpl");

  // get the example names from lockfile or from example directory
  const exampleNames = getExamples(tomlFile.borrow(), projectHome);

  var emptyCompopts = new list(string);
  emptyCompopts.pushBack("");

  // Get system, and external compopts
  const compopts = getTomlCompopts(lockFile.borrow(), emptyCompopts);
  var perExampleOptions = getExampleOptions(tomlFile.borrow(), exampleNames);

  // Close lock and toml
  lock.close();
  toml.close();


  return (sourceList, gitList, projectPath, compopts, exampleNames, perExampleOptions);
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
        examplesToRun.pushBack(example);
      }
    }
    return examplesToRun;
  }
  // user didnt list any examples, run all examples
  else return exampleNames;
}


private proc runExamples(show: bool, run: bool, build: bool, release: bool,
                         skipUpdate: bool, force: bool, examplesRequested: list(string)) throws {

  try! {

    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd);

    // Get buildInfo: dependencies, path to src code, compopts,
    // names of examples, example compopts
    var buildInfo = getBuildInfo(projectHome, skipUpdate);
    const sourceList = buildInfo[0];
    const gitList = buildInfo[1];
    const projectPath = buildInfo[2];
    const compopts = buildInfo[3];
    const exampleNames = buildInfo[4];
    const perExampleOptions = buildInfo[5];
    const projectName = basename(stripExt(projectPath, ".chpl"));
    //TODO: This build info is weird and only used here, we should
    //      move away from this

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
            const masonCompopts = getMasonDependencies(sourceList, gitList, exampleName);
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

  const exampleResult = runWithStatus(command);
  if exampleResult != 0 {
    throw new owned MasonError("Example has not been compiled: " + exampleName + ".chpl\n" +
    "Try running: mason build --example " + exampleName + ".chpl\n" +
    "         or: mason run --example " + exampleName + ".chpl --build");
  }
}

private proc getExamples(toml: Toml, projectHome: string) {
  var exampleNames: list(string);
  const examplePath = joinPath(projectHome, "example");

  if toml.pathExists("examples.examples") {

    var examples = toml["examples"]!["examples"]!.toString();
    var strippedExamples = examples.split(',').strip('[]');
    for example in strippedExamples {
      const t = example.strip().strip('"');
      exampleNames.pushBack(t);
    }
    return exampleNames;
  }
  else if isDir(examplePath) {
    var examples = findFiles(startdir=examplePath, recursive=true, hidden=false);
    for example in examples {
      if example.endsWith(".chpl") {
        exampleNames.pushBack(getExamplePath(example));
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
    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", ioMode.r);
    const toml = parseToml(toParse);
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
