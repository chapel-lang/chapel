/*
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


use Path;
use MasonUtils;
use MasonHelp;
use Regexp;

/* Entry point for mason system commands */
proc masonSystem(args) {
  try! {
    if args.size < 3 {
      masonSystemHelp();
      exit(0);
    }
    else if args[2] == "--help" || args[2] == "-h" {
      masonSystemHelp();
      exit(0);
    }
    else if pkgConfigExists() {
      if args[2] == "pc" {
        printPkgPc(args);
      }
      else if args[2] == "search" {
        pkgSearch(args);
      }
      else {
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
proc pkgSearch(args) throws {

  var desc = false;
  var quiet = false;
  var pkgName = "";
  if args.size < 4 {
    listAllPkgs();
    exit(0);
  }
  else {
    for arg in args[3..] {
      if arg == '-h' || arg == '--help' {
        masonSystemSearchHelp();
        exit(0);
      }
      else if arg == "--desc" {
        desc=true;
      }
      else if arg == "--no-show-desc" {
        quiet = true;
      }
      else {
        pkgName = arg;
      }
    }
  }
  if pkgName == "" {
    throw new owned MasonError("Must include a package name");
  }
  const pattern = compile(pkgName, ignorecase=true);
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
  if args.size < 4 {
    masonSystemPcHelp();
    exit(0);
  }
  else if args[3] == "-h" || args[3] == "--help" {
    masonSystemPcHelp();
    exit(0);
  }
  else {
    try! {
      const pkgName = args[3];
      if pkgExists(pkgName) {
        var pcDir = "".join(getPkgVariable(pkgName, "--variable=pcfiledir")).strip();
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
}


/* Gets a single variable from pkg-config
   given package name and variable */
proc getPkgVariable(pkgName: string, option: string) {

  var command = " ".join("pkg-config", pkgName, option);

  var lines: [1..0] string;
  var cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);
  sub.wait();

  var line:string;
  for line in sub.stdout.lines() {
    if line.length > 1 then
    lines.push_back(line);
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
  var pkgToml: [pkgDom] unmanaged Toml;
  var pkgInfo = new unmanaged Toml(pkgToml);

  if pkgExists(pkgName) {
    const pcVersion = "".join(getPkgVariable(pkgName, "--modversion")).strip();
    const libs = "".join(getPkgVariable(pkgName, "--libs")).strip();
    const include = "".join(getPkgVariable(pkgName, "--variable=includedir")).strip();

    pkgInfo.set("name", pkgName);
    pkgInfo.set("version", pcVersion);
    pkgInfo.set("libs", libs);
    pkgInfo.set("include", include);

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
  var exDepTree: [exDom] unmanaged Toml;

  for (name, vers) in zip(exDeps.D, exDeps.A) {
    try! {
      if pkgConfigExists() {
        const pkgInfo = getPkgInfo(name, vers.s);
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
