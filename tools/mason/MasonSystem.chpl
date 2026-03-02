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
use List;
use Map;
use MasonHelp;
use MasonUtils;
use Path;
use Regex;

/* Entry point for mason system commands */
proc masonSystem(args: [] string): int throws {

  var parser = new argumentParser(helpHandler=new MasonSystemHelpHandler());

  var subCommands = new map(string, shared Argument);
  for c in ("pc", "search") {
    subCommands.add(c, parser.addSubCommand(c));
  }

  parser.parseArgs(args);

  var usedCmd: string;
  var cmdList: list(string);
  for cmd in subCommands.keys() {
    if subCommands[cmd].hasValue() {
      usedCmd = cmd;
      cmdList = new list(subCommands[cmd].values());
      break;
    }
  }
  select usedCmd {
    when "pc" {
      if !pkgConfigExists() then
        throw new MasonError("pkg-config is not installed");
      printPkgPc(cmdList.toArray());
    }
    when "search" {
      if !pkgConfigExists() then
        throw new MasonError("pkg-config is not installed");
      pkgSearch(cmdList.toArray());
    }
    otherwise {
      writeln("No subcommand provided");
      masonSystemHelp();
      return 1;
    }
  }
  return 0;
}


proc pkgConfigExists() {
  var status = runWithStatus("pkg-config --version", true);
  return status == 0;
}

/* Searches available system packages */
proc pkgSearch(args) throws {

  var parser =
    new argumentParser(helpHandler=new MasonSystemSearchHelpHandler());

  var nameOnlyFlag = parser.addFlag(name="no-show-desc", defaultValue=false);
  var searchDescFlag = parser.addFlag(name="desc", defaultValue=false);
  var pkgNameArg = parser.addArgument(name="package", numArgs=0..1);

  parser.parseArgs(args);

  var searchDesc = searchDescFlag.valueAsBool();
  var nameOnly = nameOnlyFlag.valueAsBool();
  var pkgName = if pkgNameArg.hasValue() then pkgNameArg.value() else "";

  const command = "pkg-config --list-all";
  var lines = runCommand(["pkg-config", "--list-all"],
                         quiet=true,
                         retType=list(string));

  if pkgName == "" {
    // just print the output. If nameOnly, print just the names
    for line in lines {
      if nameOnly {
        const parts = line.partition(" ");
        writeln(parts[0]);
      } else writeln(line);
    }
  } else {
    // search each line for the pattern
    // search the whole line if requested, otherwise just the package name
    // print just the name if requested, otherwise print the whole line
    const pattern = new regex(pkgName, ignoreCase=true);
    for line in lines {
      const parts = line.partition(" ");
      var searchStr: string;
      if searchDesc then
        searchStr = line;
      else
        searchStr = parts[0];
      if pattern.search(searchStr) {
        if nameOnly {
          writeln(parts[0]);
        } else writeln(line);
      }
    }
  }
}

/* Prints a pc for user debugging */
proc printPkgPc(args: [] string) throws {
  var parser = new argumentParser(helpHandler=new MasonSystemPcHelpHandler());

  var pkgNameArg = parser.addArgument(name="package");
  parser.parseArgs(args);

  const pkgName = pkgNameArg.value();
  if pkgExists(pkgName) {
    //
    // Add a these call, since `string.join` has an iterator overload but
    // not one for list.
    //
    var pcDir = getPkgVariable(pkgName, "--variable=pcfiledir");
    var pcFile = joinPath(pcDir, pkgName + ".pc");
    if !isFile(pcFile) {
      throw new MasonError("Package exists but no .pc file found at '" +
                           pcFile + "'");
    }
    var pc = openReader(pcFile, locking=false);
    writeln("\n------- " + pkgName + ".pc -------\n");
    for line in pc.lines() {
      write(line);
    }
    writeln("\n-------------------\n");
  } else {
    throw new MasonError("Mason could not find " +
                          pkgName + " on your system");
  }
}

/* Gets a single variable from pkg-config
   given package name and variable */
proc getPkgVariable(pkgName: string, option: string) {
  var lines = runCommand(["pkg-config", pkgName, option],
                          quiet=true,
                          retType=list(string));
  var ret: string;
  for l in lines do if l != "" then ret += l + "\n";
  return ret.strip();
 }

/* Queries pkg-config for package existence */
proc pkgExists(pkgName: string) : bool {
  var command = "pkg-config " + pkgName + " --exists";
  const status = runWithStatus(command);
  return status == 0;
}

/* Retrieves build information for MasonUpdate */
proc getPkgInfo(pkgName: string, version: string) throws {

  var pkgDom: domain(string, parSafe=false);
  var pkgToml: [pkgDom] shared Toml?;
  var pkgInfo = new shared Toml(pkgToml);

  if !pkgExists(pkgName) {
    throw new MasonError("No pkg-config package by the name of: " + pkgName);
  }
  const pcVersion = getPkgVariable(pkgName, "--modversion");
  const libs = getPkgVariable(pkgName, "--libs");
  const includePath = getPkgVariable(pkgName, "--variable=includedir");

  pkgInfo.set("name", pkgName);
  pkgInfo.set("version", pcVersion);
  pkgInfo.set("libs", libs);
  pkgInfo.set("include", includePath);

  if pcVersion != version && version != "*" {
    throw new MasonError("Unable to locate " + pkgName +
                          ": " +version + "\n Found " + pcVersion);
  }
  return pkgInfo;
}

/* Given a toml of external dependencies returns
   the dependencies in a toml */
proc getPCDeps(exDeps: Toml) throws {

  if !pkgConfigExists() {
    throw new MasonError("pkg-config is not installed");
  }

  var exDom: domain(string, parSafe=false);
  var exDepTree: [exDom] shared Toml?;

  for (name, vers) in zip(exDeps.A.keys(), exDeps.A.values()) {
    const pkgInfo = getPkgInfo(name, vers!.s);
    exDom += name;
    exDepTree[name] = pkgInfo;
  }
  return exDepTree;
}
