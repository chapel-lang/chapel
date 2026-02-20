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

  proc ref enterContext() do here.chdir(path);
  proc ref exitContext(in err: owned Error?) throws {
    if err then throw err;
    here.chdir(initDir);
  }
}

private inline proc pushd(path: string) {
  return new pushdMgr(path);
}

proc install(baseDir: string, path: string) {
  log.infof("Installing prerequisite %s\n", path);
  manage pushd(path) {
    // TODO check for errors
    MasonUtils.runCommand(["make", "MASON_PACKAGE_HOME=" + baseDir]);
  }
}

proc install() {
  const baseDir = here.cwd();
  for prereq in prereqs(baseDir=baseDir) {
    install(baseDir, prereq);
  }
  log.infoln("Prerequisites have been installed");
}

iter chplFlags(const baseDir = here.cwd()) {
  var flags: list(string);

  const cmd = ["make", "MASON_PACKAGE_HOME=" + baseDir,
                "--quiet", makeTargetChplFlags];
  for prereq in prereqs(baseDir) {
    var makeOutput: string;
    manage pushd(prereq) do makeOutput = MasonUtils.runCommand(cmd).strip();

    for pFlag in makeOutput.split(" ") {
      yield pFlag;
    }
  }
}

proc dirHasMakefile(dir: string) {
  const checkPath = Path.joinPath(dir, "Makefile");
  return FS.isFile(checkPath);
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

      for dir in dirs[1..] {
        if dirHasMakefile(dir) {
          yield dir;
        } else {
          log.warnf("%s is in prereqs directory. But doesn't have a Makefile." +
                    " Ignoring.", dir);
        }
      }
    } else {
      log.infof("%s is supposed to be directory with prerequisites " +
                "but it looks to be a file. It will be ignored.", prereqsDir);
    }
  } else {
    log.debugf("%s doesn't exist.\n", prereqsDir);
  }
}
