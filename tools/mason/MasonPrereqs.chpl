/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

import FileSystem as FS;
use FileSystem only cwd,chdir; // these are tertiary methods on locale
import Path;
import List.list;

import MasonLogger;
import MasonUtils;

private config param makeTargetChplFlags = "printchplflags";

private var log = new MasonLogger.logger("mason prereqs");

record pushdMgr: contextManager {
  var path: string;
  var initDir = here.cwd();

  proc ref enterContext() { here.chdir(path); }
  proc ref exitContext(in err: owned Error?) throws {
    if err then throw err;
    here.chdir(initDir);
  }
}

private inline proc pushd(path: string) {
  return new pushdMgr(path);
}

proc install(path: string) {
  log.infof("Installing prerequisite %s\n", path);
  manage pushd(path) {
    MasonUtils.runCommand("make"); // TODO check for errors
  }
}

proc install() {
  for prereq in prereqs() {
    install(prereq);
  }
  log.infoln("Prerequisites have been installed");
}

iter chplFlags() {
  var flags: list(string);

  for prereq in prereqs() {
    const cmd = "make --quiet -C %s %s".format(prereq, makeTargetChplFlags);
    const pFlags = MasonUtils.runCommand(cmd).strip();

    for pFlag in pFlags.split(" ") {
      yield pFlag;
    }
    // should prereqs expose these in their Makefiles instead?
    const incGlob = Path.joinPath(prereq, "include", "*.h");
    for path in FS.glob(incGlob) {
      flags.pushBack(path);
      yield path;
    }

    const objGlob = Path.joinPath(prereq, "obj", "*.o");
    for path in FS.glob(objGlob) {
      yield path;
    }
  }
}

iter prereqs(const baseDir = here.cwd()) {
  const prereqsDir = "prereqs";
  const prereqsPath = Path.joinPath(baseDir, prereqsDir);

  log.debugf("Looking for the prerequisites directory %s\n", prereqsPath);
  if FS.exists(prereqsPath) {
    if FS.isDir(prereqsPath) {
      log.infof("Prerequisites directory exists (%s)\n", prereqsPath);

      // You shouldn't need an array here, but there seems to be a compiler bug
      // See https://github.com/chapel-lang/chapel/issues/27855
      const dirs = FS.walkDirs(prereqsPath, depth=1);

      for dir in dirs[1..] do yield dir;
    }
    else {
      log.infof("%s is supposed to be directory with prerequisites " +
                "but it looks to be a file. It will be ignored.", prereqsDir);
    }
  }
  else {
    log.debugf("%s doesn't exist.\n", prereqsDir);
  }
}
