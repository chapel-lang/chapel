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

/**/
module MasonPrereqs {

import FileSystem as FS;
import List.list;
import ThirdParty.Pathlib.path;

import MasonLogger;
import MasonUtils;

private config param makeTargetChplFlags = "printchplflags";

private var log = MasonLogger.getLogger("mason prereqs");


proc install(baseDir: path, p: path) throws {
  log.info("Installing prerequisite ", p:string);
  const oldDir = path.cwd();
  p.chdir();
  defer oldDir.chdir();
  // TODO: I would love to use p.pushChdir(), but I don't trust
  // error handling + context managers enough
  // TODO check for errors
  MasonUtils.runCommand(["make", "MASON_PACKAGE_HOME=" + baseDir:string]);
}

proc install() throws {
  const baseDir = path.cwd();
  for prereq in prereqs(baseDir=baseDir) {
    install(baseDir, prereq);
  }
  log.info("Prerequisites have been installed");
}

iter chplFlags(const baseDir = path.cwd()) throws {
  var flags: list(string);

  const cmd = ["make",
               "CHPL_HOME=" + MasonUtils.CHPL_HOME,
               "MASON_PACKAGE_HOME=" + baseDir:string,
               "--quiet", makeTargetChplFlags];
  for prereq in prereqs(baseDir) {
    var makeOutput: string;
    const oldDir = path.cwd();
    prereq.chdir();
    defer oldDir.chdir();
    // TODO: I would love to use prereq.pushChdir(), but I don't trust
    // error handling + context managers enough
    makeOutput = MasonUtils.runCommand(cmd).strip();

    for pFlag in makeOutput.split(" ") {
      if pFlag.strip() != "" then
        yield pFlag.strip();
    }
  }
}

proc dirHasMakefile(dir: path) {
  return (dir / "Makefile").isFile();
}

iter prereqs(const baseDir = path.cwd()): path {
  const prereqsDir = "prereqs";
  const prereqsPath = baseDir / prereqsDir;

  log.debug("Looking for the prerequisites directory ", prereqsPath);
  if prereqsPath.exists() {
    if prereqsPath.isDir() {
      log.infof("Prerequisites directory exists (%s)", prereqsPath:string);

      // You shouldn't need an array here, but there seems to be a compiler bug
      // See https://github.com/chapel-lang/chapel/issues/27855
      const dirs = FS.walkDirs(prereqsPath:string, depth=1);

      for dir in dirs[1..] {
        const dirP = dir:path;
        if dirHasMakefile(dirP) {
          yield dirP;
        } else {
          log.warnf("%s is in prereqs directory. But doesn't have a Makefile." +
                    " Ignoring.", dir:string);
        }
      }
    } else {
      log.infof("%s is supposed to be directory with prerequisites " +
                "but it looks to be a file. It will be ignored.",
                prereqsDir:string);
    }
  } else {
    log.debugf("%s don't exist.", prereqsDir:string);
  }
}

}
