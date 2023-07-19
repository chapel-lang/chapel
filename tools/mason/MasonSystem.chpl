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
use List;
use Map;
use MasonHelp;
use MasonUtils;
use Path;
use Regex;

/* Entry point for mason system commands */
proc masonSystem(args: [] string) {

  var parser = new argumentParser(helpHandler=new MasonSystemHelpHandler());

  var pcCmd = parser.addSubCommand("pc");
  var searchCmd = parser.addSubCommand("search");

  parser.parseArgs(args);

  try! {
    if pcCmd.hasValue() {
      pkgConfigExists();
      var pcArgs = pcCmd.values();
      printPkgPc(pcArgs);
    }
    else if searchCmd.hasValue() {
      pkgConfigExists();
      var searchArgs = searchCmd.values();
      pkgSearch(searchArgs);
    }
    else { // no valid sub-command given
      masonSystemHelp();
      exit(0);
    }
  }
  catch e: MasonError { // likely pkg-config wasn't found on system
    stderr.writeln(e.message());
    exit(1);
  }
}


/* Checks to see that pkg-config is installed */
proc pkgConfigExists() throws {
  var status = runWithStatus("pkg-config --version", true);
  if status != 0 {
    throw new owned MasonError("pkg-config is not installed");
  }
  return true;
}

/* Searches available system packages */
proc pkgSearch(args) throws {

  var parser = new argumentParser(helpHandler=new MasonSystemSearchHelpHandler());

  var quietFlag = parser.addFlag(name="no-show-desc", defaultValue=false);
  var descFlag = parser.addFlag(name="desc", defaultValue=false);
  var pkgNameArg = parser.addArgument(name="package", numArgs=0..1);

  parser.parseArgs(args);

  var desc = descFlag.valueAsBool();
  var quiet = quietFlag.valueAsBool();
  var pkgName = "";

  if pkgNameArg.hasValue() {
    pkgName = pkgNameArg.value();
  }
  else {
    listAllPkgs();
    exit(0);
  }

  const pattern = new regex(pkgName, ignoreCase=true);
  const command = "pkg-config --list-all";
  const cmd = command.split();
  var sub = spawn(cmd, stdout=pipeStyle.pipe);
  sub.wait();

  // TODO: Test if this ever worked
  for line in sub.stdout.lines() {
    const toSearch = line.partition(" ");
    if desc {
      if pattern.search(line) {
        if quiet {
          writeln(toSearch[1]);
        }
        else write(line);
      }
    }
    else {
      if pattern.search(toSearch[1]) {
        if quiet {
          writeln(toSearch[1]);
        }
        else write(line);
      }
    }
  }
}


/* Lists all packages available on user system */
proc listAllPkgs() {
  const command = "pkg-config --list-all";
  const status = runCommand(command);
}


/* Prints a pc for user debugging */
proc printPkgPc(args) throws {
  var parser = new argumentParser(helpHandler=new MasonSystemPcHelpHandler());

  var pkgNameArg = parser.addArgument(name="package", numArgs=0..1);

  parser.parseArgs(args);

  if !pkgNameArg.hasValue() {
    masonSystemPcHelp();
    exit(1);
  }
  try! {
    const pkgName = pkgNameArg.value();
    if pkgExists(pkgName) {
      //
      // Add a these call, since `string.join` has an iterator overload but
      // not one for list.
      //
      var pcDir = "".join(getPkgVariable(pkgName, "--variable=pcfiledir").these()).strip();
      var pcFile = joinPath(pcDir, pkgName + ".pc");
      var pc = openReader(pcFile);
      writeln("\n------- " + pkgName + ".pc -------\n");
      for line in pc.lines() {
        write(line);
      }
      writeln("\n-------------------\n");
    }
    else {
      throw new owned MasonError("Mason could not find " + pkgName + " on your system");
    }
  }
  catch e: FileNotFoundError {
    stderr.writeln("Package exists but Mason could not find it's .pc file");
    exit(1);
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


/* Gets a single variable from pkg-config
   given package name and variable */
proc getPkgVariable(pkgName: string, option: string) {

  var command = " ".join("pkg-config", pkgName, option);

  var lines: list(string);
  var cmd = command.split();
  var sub = spawn(cmd, stdout=pipeStyle.pipe);
  sub.wait();

  var line:string;
  for line in sub.stdout.lines() {
    if line.size > 1 then
      lines.pushBack(line);
  }

  return lines;
 }

/* Queries pkg-config for package existence */
proc pkgExists(pkgName: string) : bool {

  const debug = " --print-errors";
  var command = "pkg-config " + pkgName + " --exists";
  const status = runWithStatus(command);
  if status != 0 {
    return false;
  }
  return true;
}

/* Retrieves build information for MasonUpdate */
proc getPkgInfo(pkgName: string, version: string) throws {

  var pkgDom: domain(string);
  var pkgToml: [pkgDom] shared Toml?;
  var pkgInfo = new shared Toml(pkgToml);

  if pkgExists(pkgName) {
    // Pass "these" to join instead of converting the list to an array.
    const pcVersion = "".join(getPkgVariable(pkgName, "--modversion").these()).strip();
    const libs = "".join(getPkgVariable(pkgName, "--libs").these()).strip();
    const includePath = "".join(getPkgVariable(pkgName, "--variable=includedir").these()).strip();

    pkgInfo.set("name", pkgName);
    pkgInfo.set("version", pcVersion);
    pkgInfo.set("libs", libs);
    pkgInfo.set("include", includePath);

    if pcVersion != version && version != "*" {
      throw new owned MasonError("Unable to locate " + pkgName + ": " +version + "\n Found " + pcVersion);
    }
  }
  else {
    throw new owned MasonError("No pkg-config package by the name of: " + pkgName);
  }
  return pkgInfo;
}

/* Given a toml of external dependencies returns
   the dependencies in a toml */
proc getPCDeps(exDeps: Toml) {

  var exDom: domain(string);
  var exDepTree: [exDom] shared Toml?;

  for (name, vers) in zip(exDeps.A.keys(), exDeps.A.values()) {
    try! {
      if pkgConfigExists() {
        const pkgInfo = getPkgInfo(name, vers!.s);
        exDom += name;
        exDepTree[name] = pkgInfo;
      }
    }
    catch e: MasonError {
      stderr.writeln(e.message());
      exit(1);
    }
  }
  return exDepTree;
}
