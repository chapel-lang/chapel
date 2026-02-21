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
use MasonBuild;
use MasonExample;
use MasonHelp;
use MasonUtils;
import MasonLogger;
use TOML;

private var log = new MasonLogger.logger("mason run");

proc masonRun(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonRunHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var buildFlag = parser.addFlag(name="build", defaultValue=false);

  // not actually flags for Run, but rather for build
  // TODO: I'm failure certain _present being true when defaultValue is set
  // is a bug in ArgumentParser, but changing that may have wider impacts
  // for now, use not default so _present is meaningful
  // for the purposes of this function, the value of forceFlag isn't used
  // anyways
  var forceFlag = parser.addFlag(name="force" /*defaultValue=false*/);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var exampleOpts = parser.addOption(name="example",
                                     numArgs=0..);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);

  const projectType = getProjectType();
  if !exampleOpts._present && projectType != "application" then
    throw new MasonError(
      "Only mason applications can be run, but this is a Mason " + projectType);

  if exampleOpts._present && passArgs.hasValue() {
    throw new MasonError("Examples do not support `--` syntax");
  }

  // don't specify build flags unless we are actually building
  if !buildFlag.valueAsBool() {
    if forceFlag._present then
      throw new MasonError("The --force flag is only valid " +
                           "when used with --build");
    if updateFlag._present then
      throw new MasonError("The --[no]-update flag is only valid " +
                           "when used with --build");
  }

  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var execopts = new list(passArgs.values());


  if exampleOpts._present &&
    (!exampleOpts.hasValue() || exampleOpts.value().startsWith("-")) {
    // when mason run --example called
    printAvailableExamples();
  } else if exampleOpts._present || buildFlag.valueAsBool() {
    // --example with value or build flag
    masonBuildRun(args);
  } else {
    runProjectBinary(show, release, execopts);
  }
}

proc runProjectBinary(show: bool, release: bool,
                      execopts: list(string)) throws {

  const cwd = here.cwd();
  const projectHome = getProjectHome(cwd);
  const toParse = open(projectHome + "/Mason.toml", ioMode.r);
  const tomlFile = parseToml(toParse);
  const project = tomlFile["brick.name"]!.s;

  // Find the Binary and execute
  if isDir(joinPath(projectHome, 'target')) {
    var execs = ' '.join(execopts.these());

    // decide which binary(release or debug) to run
    var command: string;
    if release {
      if isDir(joinPath(projectHome, 'target/release')) {
        command = joinPath(projectHome, "target/release", project);
      }
    } else {
      command = joinPath(projectHome, "target/debug", project);
    }

    var built = false;
    if isFile(command) then built = true;

    // add execopts
    command += " " + execs;

    if show {
      if release then writeln("Executing [release] target: " + command);
      else writeln("Executing [debug] target: " + command);
    }

    // Build if not built, throwing error if Mason.toml doesnt exist
    if isFile(joinPath(projectHome, "Mason.lock")) && built {
      const output = runCommand(command, quiet=true);
      write(output);
    } else if isFile(joinPath(projectHome, "Mason.toml")) {
      const msg = "Mason could not find your Mason.lock.\n";
      const help = "To build and run your project use: mason run --build";
      throw new owned MasonError(msg + help);
    } else {
      throw new owned MasonError("Mason could not find your Mason.toml file");
    }

    // Close memory
    toParse.close();
  } else {
    throw new owned MasonError("Mason could not find the compiled program");
  }
}


// FIXME: this function reparses args to then pass calls to `masonBuild`. Why!?
// We should just restructure `masonBuild` so that it can be callable directly
// since we already parsed the args
/* Builds program before running. */
private proc masonBuildRun(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonRunHelpHandler());

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var releaseFlag = parser.addFlag(name="release", defaultValue=false);
  var buildFlag = parser.addFlag(name="build", defaultValue=false);

  // not actually flags for Run, but rather for build
  var forceFlag = parser.addFlag(name="force", defaultValue=false);
  var updateFlag = parser.addFlag(name="update", flagInversion=true);

  var exampleOpts = parser.addOption(name="example",
                                     numArgs=0..);

  var passArgs = parser.addPassThrough();

  parser.parseArgs(args);

  var example = false;
  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var force = forceFlag.valueAsBool();
  var exec = false;
  var buildExample = buildFlag.valueAsBool();
  var skipUpdate = MASON_OFFLINE;
  var execopts: list(string);

  if exampleOpts._present then example = true;

  if updateFlag.hasValue() {
    if updateFlag.valueAsBool() then skipUpdate = false;
    else skipUpdate = true;
  }

  if example {
    var examples = new list(exampleOpts.values());
    runExamples(show=show, run=true, build=buildExample, release=release,
                skipUpdate=skipUpdate, force=force,
                examplesRequested=examples);
  } else {
    var buildArgs: list(string);
    buildArgs.pushBack("build");
    if skipUpdate then buildArgs.pushBack("--no-update");
                  else buildArgs.pushBack("--update");
    if release then buildArgs.pushBack("--release");
    if force then buildArgs.pushBack("--force");
    if show then buildArgs.pushBack("--show");
    masonBuild(buildArgs.toArray());
    for val in passArgs.values() do execopts.pushBack(val);
    runProjectBinary(show, release, execopts);
  }
}
