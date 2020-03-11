/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use FileSystem;
use TOML;

proc masonRun(args) throws {

  var show = false;
  var example = false;
  var release = false;
  var exec = false;
  var execopts: list(string);

  if args.size > 2 {
    for arg in args[2..] {
      if exec == true {
        execopts.append(arg);
      }
      else if arg == '-h' || arg == '--help' {
        masonRunHelp();
        exit(0);
      }
      else if arg == '--build' {
        masonBuildRun(args);
        exit(1);
      }
      else if arg == '--' {        
        exec = true;
      }
      else if arg == '--show' {
        show=true;
      }
      else if arg == 'run' {
        continue;
      }
      else if arg == '--release' {
        release=true;
      }
      else if arg == '--example' {        
        if args.size > 3 {
          masonBuildRun(args);
        }
        // mason run --example
        else printAvailableExamples();
        exit(0);
      }
      else {
        execopts.append(arg);
      }
    }
  }
  runProjectBinary(show, release, execopts);
}

proc runProjectBinary(show: bool, release: bool, execopts: list(string)) throws {

  try! {

    const cwd = getEnv("PWD");
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

  try! {
    var example = false;
    var show = false;
    var release = false;
    var force = false;
    var exec = false;
    var buildExample = false;
    var updateRegistry = true;
    var execopts: list(string);
    for arg in args[2..] {
      if exec == true {
        execopts.append(arg);
      }
      else if arg == "--" {
        if example then
          throw new owned MasonError("Examples do not support `--` syntax");
        exec = true;
      }
      else if arg == "--example" {
        example = true;
      }
      else if arg == "--show" {
        show = true;
      }
      else if arg == "--build" {
        buildExample = true;
      }
      else if arg == "--force" {
        force = true;
      }
      else if arg == "--release" {
        release = true;
      }
      else if arg == '--no-update' {
        updateRegistry = false;
      }
      else {
        // could be examples or execopts
        execopts.append(arg);
      }
    }
    if example {
      if !buildExample then execopts.append("--no-build");
      if release then execopts.append("--release");
      if force then execopts.append("--force");
      if show then execopts.append("--show");
      masonExample(execopts);
    }
    else {
      var buildArgs: list(string);
      //var buildArgs: [0..1] string = ["mason", "build"];
      buildArgs.append("mason");
      buildArgs.append("build");
      if !updateRegistry then buildArgs.append("--no-update");
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


