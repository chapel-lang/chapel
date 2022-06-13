/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
use TOML;


proc masonRun(args: [] string) throws {

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

  var show = showFlag.valueAsBool();
  var release = releaseFlag.valueAsBool();
  var execopts = new list(passArgs.values());


  if exampleOpts._present && !exampleOpts.hasValue()
    && args.size == 2 {
    // when mason run --example called
    printAvailableExamples();
    exit(0);
  } else if exampleOpts._present || buildFlag.valueAsBool() {
    // --example with value or build flag
    masonBuildRun(args);
    exit(0);
  }
  runProjectBinary(show, release, execopts);
}

proc runProjectBinary(show: bool, release: bool, execopts: list(string)) throws {

  try! {

    const cwd = here.cwd();
    const projectHome = getProjectHome(cwd);
    const toParse = open(projectHome + "/Mason.toml", iomode.r);
    const tomlFile = owned.create(parseToml(toParse));
    const project = tomlFile["brick"]!["name"]!.s;

    // Find the Binary and execute
    if isDir(joinPath(projectHome, 'target')) {
      var execs = ' '.join(execopts.these());

      // decide which binary(release or debug) to run
      var command: string;
      if release {
        if isDir(joinPath(projectHome, 'target/release')) {
          command = joinPath(projectHome, "target/release", project);
        }
      }
      else {
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
      if isFile(joinPath(projectHome, "Mason.lock")) && built then {
        runCommand(command);
      }
      else if isFile(joinPath(projectHome, "Mason.toml")) {
        const msg = "Mason could not find your Mason.lock.\n";
        const help = "To build and run your project use: mason run --build";
        throw new owned MasonError(msg + help);
      }
      else {
        throw new owned MasonError("Mason could not find your Mason.toml file");
      }

      // Close memory
      toParse.close();
    }
    else {
      throw new owned MasonError("Mason could not find the compiled program");
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


/* Builds program before running. */
private proc masonBuildRun(args: [?d] string) {

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

  try! {
    var example = false;
    var show = showFlag.valueAsBool();
    var release = releaseFlag.valueAsBool();
    var force = forceFlag.valueAsBool();
    var exec = false;
    var buildExample = buildFlag.valueAsBool();
    var skipUpdate = MASON_OFFLINE;
    var execopts: list(string);
    var exampleProgram='';

    if exampleOpts._present then example = true;

    if passArgs.hasValue() && example {
      throw new owned MasonError("Examples do not support `--` syntax");
    }

    if updateFlag.hasValue() {
      if updateFlag.valueAsBool() then skipUpdate = false;
      else skipUpdate = true;
    }

    if example {
      // add expected arguments for masonExample
      execopts.insert(0,["example", "--example"]);
      for val in exampleOpts.values() do execopts.append(val);
      if !buildExample then execopts.append("--no-build");
      if release then execopts.append("--release");
      if force then execopts.append("--force");
      if show then execopts.append("--show");
      masonExample(execopts.toArray());
    }
    else {
      var buildArgs: list(string);
      buildArgs.append("build");
      if skipUpdate then buildArgs.append("--no-update");
                    else buildArgs.append("--update");
      if release then buildArgs.append("--release");
      if force then buildArgs.append("--force");
      if show then buildArgs.append("--show");
      masonBuild(buildArgs.toArray());
      for val in passArgs.values() do execopts.append(val);
      runProjectBinary(show, release, execopts);
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
  }
}


