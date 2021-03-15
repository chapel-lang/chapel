/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use MasonArguments;

use FileSystem;
use List;
use TOML;

proc masonRun(args: list(string)) throws {

  var helpFlag = new HelpFlag();
  var buildFlag = new BooleanFlag('--build');
  var dashFlag = new OtherArgsFlag();
  var showFlag = new BooleanFlag('--show');
  var releaseFlag = new BooleanFlag('--release');
  var exampleFlag = new BooleanFlag('--example'); // TODO -- must be OptionalValueFlag
  var otherArgs: list(string);

  var ok = processArgs(args, otherArgs,
                       helpFlag, buildFlag, dashFlag, showFlag, exampleFlag);
  if !ok || helpFlag.present {
    masonRunHelp();
    exit();
  }

  if buildFlag.value {
    masonBuildRun(args);
    exit(0);
  }

  var show = false;
  var example = false;
  var release = false;
  var exec = false;
  var execopts: list(string);

  show = showFlag.value;
  release = releaseFlag.value;

  if exampleFlag.value {
    if otherArgs.isEmpty() {
      printAvailableExamples();
      exit(0);
    } else {
      masonBuildRun(args);
      exit(0);
    }
  } else {
    runProjectBinary(show, release, otherArgs);
  }
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
private proc masonBuildRun(args: list(string)) {

  try! {
    var example = false;
    var show = false;
    var release = false;
    var force = false;
    var exec = false;
    var buildExample = false;
    var skipUpdate = MASON_OFFLINE;
    var execopts: list(string);
    var exampleProgram='';

    var helpFlag = new HelpFlag();
    var dashFlag = new OtherArgsFlag();
    var exampleFlag = new ValueFlag('--example');
    var showFlag = new BooleanFlag('--show');
    var buildFlag = new BooleanFlag('--build');
    var forceFlag = new BooleanFlag('--force');
    var releaseFlag = new BooleanFlag('--release');
    var updateFlag = new BooleanFlag('--update', '--no-update', !MASON_OFFLINE);

    var otherArgs: list(string);

    var ok = processArgs(args, otherArgs,
                         helpFlag, buildFlag, dashFlag, showFlag, exampleFlag);
    if !ok || helpFlag.present {
      throw new MasonError("invalid masonBuildRun call");
    }

    if exampleFlag.present && dashFlag.present {
      throw new owned MasonError("Examples do not support `--` syntax");
    }
    show = showFlag.value;
    buildExample = buildFlag.value;
    force = forceFlag.value;
    release = releaseFlag.value;
    skipUpdate = !updateFlag.value;

    // could be examples or execopts
    execopts = otherArgs;

    if example {
      if !buildExample then execopts.append("--no-build");
      if release then execopts.append("--release");
      if force then execopts.append("--force");
      if show then execopts.append("--show");
      masonExample(execopts);
    }
    else {
      var buildArgs: list(string);
      buildArgs.append("mason");
      buildArgs.append("build");
      if skipUpdate then buildArgs.append("--no-update");
                    else buildArgs.append("--update");
      if release then buildArgs.append("--release");
      if force then buildArgs.append("--force");
      if show then buildArgs.append("--show");

      masonBuild(buildArgs);
      runProjectBinary(show, release, execopts);
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
  }
}


