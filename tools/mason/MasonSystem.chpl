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


use MasonArguments;
use MasonHelp;
use MasonUtils;

use List;
use Map;
use Path;
use Regexp;

/* Entry point for mason system commands */
proc masonSystem(args: list(string)) {
  try! {
    var helpFlag = new HelpFlag();
    var pcFlag = new SubcommandFlag('pc');
    var searchFlag = new SubcommandFlag('search');
    var otherArgs: list(string);

    var ok = processArgs(args, otherArgs,
                         helpFlag, pcFlag, searchFlag);
    if !ok || helpFlag.present {
      masonSystemHelp();
      exit(1);
    }

    if pkgConfigExists() {
      if pcFlag.present {
        printPkgPc(pcFlag.args);
      }
      if searchFlag.present {
        pkgSearch(searchFlag.args);
      } else {
        masonSystemHelp();
        exit(0);
      }
    }
    else {
      masonSystemHelp();
      exit(0);
    }
  }
  catch e: MasonError {
    stderr.writeln(e.message());
    exit(1);
  }
}


/* Checks to see that pkg-config is installed */
proc pkgConfigExists() throws {
  var status = runWithStatus("pkg-config --version", false);
  if status != 0 {
    throw new owned MasonError("pkg-config is not installed");
  }
  return true;
}

/* Searches available system packages */
proc pkgSearch(args: list(string)) throws {
  var helpFlag = new HelpFlag();
  var descFlag = new BooleanFlag('--desc');
  var quietFlag = new BooleanFlag('--no-show-desc');
  var otherArgs: list(string);
  var ok = processArgs(args, otherArgs,
                       helpFlag, descFlag, quietFlag);
  if !ok || helpFlag.present {
    masonSystemSearchHelp();
    exit(1);
  }

  var desc = descFlag.value;
  var quiet = quietFlag.value;
  var pkgName = "";

  if otherArgs.size == 0 {
    listAllPkgs();
    exit(0);
  } else if otherArgs.size > 1 {
    throw new MasonError("Must include one package name");
  } else {
    pkgName = otherArgs.first();
  }
  if pkgName == "" {
    throw new MasonError("Must include a package name");
  }
  const pattern = compile(pkgName, ignoreCase=true);
  const command = "pkg-config --list-all";
  const cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);
  sub.wait();

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
  var helpFlag = new HelpFlag();
  var otherArgs: list(string);
  var ok = processArgs(args, otherArgs, helpFlag);
  if !ok || helpFlag.present {
    masonSystemPcHelp();
    exit(1);
  }
  if otherArgs.size != 1 {
    throw new MasonError("expected one package name");
  }
  const pkgName = otherArgs.first();
  try! {
    if pkgExists(pkgName) {
      //
      // Add a these call, since `string.join` has an iterator overload but
      // not one for list.
      //
      var pcDir = "".join(getPkgVariable(pkgName, "--variable=pcfiledir").these()).strip();
      var pcFile = joinPath(pcDir, pkgName + ".pc");
      var pc = open(pcFile, iomode.r);
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
  var sub = spawn(cmd, stdout=PIPE);
  sub.wait();

  var line:string;
  for line in sub.stdout.lines() {
    if line.size > 1 then
      lines.append(line);
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
  var pkgToml: [pkgDom] unmanaged Toml?;
  var pkgInfo = new unmanaged Toml(pkgToml);

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
proc getPCDeps(exDeps: unmanaged Toml) {

  var exDom: domain(string);
  var exDepTree: [exDom] unmanaged Toml?;

  for (name, vers) in exDeps.A.items() {
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
