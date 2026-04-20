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

/**/
module MasonExample {

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
import MasonLogger;
import MasonPrereqs;
import ThirdParty.Pathlib.path;

private var log = MasonLogger.getLogger("mason example");

proc runExamples(show: bool, run: bool, build: bool, release: bool,
                 skipUpdate: bool, force: bool,
                 examplesRequested: list(string),
                 extraCompopts = new list(string),
                 extraExecopts = new list(string),
                 nLocales: int) throws {

  if build then
    updateLock(skipUpdate);

  const cwd = here.cwd();
  const projectHome = getProjectHome(cwd);

  // Get buildInfo: dependencies, path to src code, compopts,
  // names of examples, example compopts
  var buildInfo = getBuildInfo(projectHome, skipUpdate, build);
  const projectName = basename(stripExt(buildInfo.projectPath, ".chpl"));
  //TODO: This build info is weird and only used here, we should
  //      move away from this
  //      alternatively, should we use this more generally?

  var numExamples = buildInfo.exampleNames.size;
  var examplesToRun =
    determineExamples(buildInfo.exampleNames, examplesRequested);

  // Clean out example binaries from previous runs
  if build then setupExampleDir(projectHome);

  if numExamples > 0 {
    for example in examplesToRun {

      const examplePath = "".join(projectHome, '/example/', example);
      const exampleName = basename(stripExt(example, ".chpl"));

      // retrieves compopts and execopts found per example in the toml file
      const optsFromToml = buildInfo.perExampleOptions[exampleName];
      var exampleCompopts = optsFromToml.compopts;
      var exampleExecopts = optsFromToml.execopts;
      exampleExecopts.pushBack(extraExecopts);

      if release then exampleCompopts.pushBack("--fast");

      if build {
        if force || exampleModified(projectHome, projectName,
                                    example, extraCompopts) {

          // remove old binary
          removeExampleBinary(projectHome, exampleName);
          // get the string of dependencies for compilation
          // also names example as --main-module
          const masonCompopts =
            getMasonDependencies(buildInfo.sourceList,
                                  buildInfo.gitList,
                                  exampleName);
          const outputName =
            joinPath(projectHome, "target", "example", exampleName);
          var compCommand = new list(string);
          compCommand.pushBack(["chpl", examplePath, buildInfo.projectPath,
                                "-o", outputName]);
          compCommand.pushBack(buildInfo.compopts);
          compCommand.pushBack(masonCompopts);
          compCommand.pushBack(exampleCompopts);
          compCommand.pushBack(extraCompopts);
          if show then writeln(" ".join(compCommand.these()));
          const compilation = runWithStatus(compCommand.toArray());

          if compilation != 0 {
            const fingerprintDir =
              joinPath(projectHome, "target", "example", ".fingerprint");
            invalidateFingerprint(projectName, fingerprintDir);
            stderr.writeln("compilation failed for " + example);
          } else {
            if show || !run then
              writeln("compiled ", example, " successfully");
            if run then
              runExampleBinary(projectHome, exampleName,
                                release, show, exampleExecopts, nLocales);
          }
        } else {
          // build is skipped but examples still need to be run
          writeln("Skipping ", example,
                  ": no changes made to project or example");
          if run then
            runExampleBinary(projectHome, exampleName,
                              release, show, exampleExecopts, nLocales);
        }
      } else {
        // just running the example
        runExampleBinary(projectHome, exampleName,
                          release, show, exampleExecopts, nLocales);
      }
    }
  } else {
    throw new MasonError("No examples were found in /example");
  }
}

record examplesBuildInfo {
  var sourceList: list(srcSource);
  var gitList: list(gitSource);
  var projectPath: string;
  var compopts: list(string);
  var exampleNames: list(string);
  var perExampleOptions: map(string, chplOptions);
}

/*
  computes most of the information required to build and run examples

  if build is false, it only computes the example names and the path to the
  project source code. the rest of the information is left blank
*/
private proc getBuildInfo(projectHome: string,
                          skipUpdate: bool,
                          build: bool): examplesBuildInfo throws {

  const toml = open(Path.joinPath(projectHome, "Mason.toml"), ioMode.r);
  defer toml.close();
  const tomlFile = parseToml(toml);

  // get the example names from lockfile or from example directory
  const exampleNames = getExamples(tomlFile.borrow(), projectHome);

  var sourceList: list(srcSource);
  var gitList: list(gitSource);
  var compopts: list(string);
  var perExampleOptions = getExampleOptions(tomlFile.borrow(), exampleNames);

  const project = tomlFile["brick.name"]!.s;
  const projectPath = Path.joinPath(projectHome, "src", project + ".chpl");

  if build {
    const lock = open(Path.joinPath(projectHome, "Mason.lock"), ioMode.r);
    defer lock.close();
    const lockFile = parseToml(lock);
    // Get project source code and dependencies
    (sourceList, gitList) = genSourceList(lockFile);
    const depPath = Path.joinPath(MASON_HOME, 'src');
    const gitDepPath = Path.joinPath(MASON_HOME, 'git');


    getSrcCode(sourceList, skipUpdate, false);
    getGitCode(gitList, skipUpdate, false);


    compopts = getTomlCompopts(lockFile.borrow());
    log.debug("Adding prerequisite flags");
    // add prerequisite compopts
    for flag in MasonPrereqs.chplFlags() {
      log.debug("+compflag ", flag);
      compopts.pushBack(flag);
    }

    log.debug("Base compopts: ", compopts);

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

        log.debugf("Adding source dependency %s's flags", name);
        compopts.pushBack(depSrc);

        for flag in MasonPrereqs.chplFlags(depDir:path) {
          log.debug("+compflag ", flag);
          compopts.pushBack(flag);
        }
      }
    }

    // can't use _ since it will leak
    // see https://github.com/chapel-lang/chapel/issues/25926
    @chplcheck.ignore("UnusedLoopIndex")
    for (_x, name, branch, _y) in gitSource.iterList(gitList) {
      const depDir = Path.joinPath(gitDepPath, name + "-" + branch);
      const gitDepSrc = Path.joinPath(depDir, "src", name + ".chpl");
      compopts.pushBack(gitDepSrc);

      for flag in MasonPrereqs.chplFlags(depDir:path) {
        log.debug("+compflag ", flag);
        compopts.pushBack(flag);
      }
    }

    // get system deps
    if const pkgDeps = lockFile.get['system'] {
      for (_, depInfo) in zip(pkgDeps.A.keys(), pkgDeps.A.values()) {
        for (k,v) in allFields(depInfo!) {
          var val = v!;
          select k {
            when "libs" do compopts.pushBack(parseCompilerOptions(val));
            when "includes" do compopts.pushBack(parseCompilerOptions(val));
            otherwise continue;
          }
        }
      }
    }
  }

  return new examplesBuildInfo(sourceList, gitList, projectPath,
                               compopts, exampleNames, perExampleOptions);
}

// retrieves compopts and execopts for each example.
private proc getExampleOptions(
  toml: Toml,
  exampleNames: list(string)
): map(string, chplOptions) throws {
  var exampleOptions = new map(string, chplOptions);
  for example in exampleNames {
    const exampleName = basename(stripExt(example, ".chpl"));
    exampleOptions[exampleName] = new chplOptions();
    const examplePath = "examples." + exampleName;
    if const compopts = toml.get(examplePath + ".compopts") {
      try {
        exampleOptions[exampleName].compopts = parseCompilerOptions(compopts);
      } catch {
        throw new MasonError("unable to parse compopts");
      }
    }
    if const execopts = toml.get(examplePath + ".execopts") {
      try {
        exampleOptions[exampleName].execopts = parseCompilerOptions(execopts);
      } catch {
        throw new MasonError("unable to parse execopts");
      }
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
      } else {
        examplesToRun.pushBack(example);
      }
    }
    return examplesToRun;
  } else return exampleNames;
}

private proc runExampleBinary(projectHome: string, exampleName: string,
                              release: bool, show: bool,
                              execopts: list(string), nLocales: int) throws {
  const executable = joinPath(projectHome, "target", "example", exampleName);
  var command: list(string);
  command.pushBack(executable);
  command.pushBack("-nl" + nLocales:string);
  command.pushBack(execopts);
  if show then
    writef("Executing [%s] target: %s\n",
           if release then "release" else "debug",
           " ".join(command.these()));

  if !isFile(executable) {
    throw new MasonError(
      "Example has not been compiled: " + exampleName + ".chpl\n" +
      "Try running: mason build --example " + exampleName + ".chpl\n" +
      "         or: mason run --example " + exampleName + ".chpl --build");
  }

  // TODO: do we need to expose the error code in some way?
  const exampleResult = runWithStatus(command.toArray(), capture=false);
}

private proc getExamples(toml: Toml, projectHome: string) {
  var exampleNames: list(string);
  const examplePath = joinPath(projectHome, "example");

  if const examplesToml = toml.get("examples.examples") {
    var examples = examplesToml.toString();
    var strippedExamples = examples.split(',').strip('[]');
    for example in strippedExamples {
      const t = example.strip().strip('"');
      exampleNames.pushBack(t);
    }
    return exampleNames;
  } else if isDir(examplePath) {
    var examples = findFiles(startdir=examplePath,
                             recursive=true, hidden=false);
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
  } else {
    if examplePath == "" {
      return getExamplePath(split[0], split[1]);
    } else {
      var appendedPath = joinPath(split[1], examplePath);
      return getExamplePath(split[0], appendedPath);
    }
  }
}

// used when user calls `mason run --example` without argument
proc printAvailableExamples() throws {
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

// Checks to see if an example, source code, or Mason.toml has been modified
proc exampleModified(projectHome: string,
                     projectName: string,
                     exampleName: string,
                     commandLineCompopts: list(string)) {
  const example = basename(stripExt(exampleName, ".chpl"));
  const exampleBinPath = joinPath(projectHome, "target/example", example);
  const examplePath = joinPath(projectHome, "example");

  // check for changes to Mason.toml and src code
  const fingerprintDir =
    joinPath(projectHome, "target", "example", ".fingerprint");
  const fingerprintChanged =
    !checkFingerprint(projectName, fingerprintDir,
                      computeFingerprint(commandLineCompopts));
  if projectModified(projectHome, example, "example") || fingerprintChanged {
      return true;
  } else {
      // check for binary existence
      if isFile(exampleBinPath) {
        // check for changes to example
        const exModTime = getLastModified(joinPath(examplePath, exampleName));
        const exBinModTime = getLastModified(exampleBinPath);
        return exModTime > exBinModTime;
      } else
        return true;
  }
}

}
