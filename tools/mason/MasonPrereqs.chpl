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
use FileSystem only cwd,chdir;
import Path;
import List;

import MasonLogger;
import MasonUtils;


private var log = new MasonLogger.logger("prereqs");

proc install() {

  const baseDir = here.cwd();

  // dirs will include the path itself as the first element
  for prereq in prereqs() {
    here.chdir(prereq);
    log.infof("Installing prerequisite %s\n", prereq);
    here.chdir(prereq);

    MasonUtils.runCommand("make"); // TODO check for errors

  }
  log.infoln("Prerequisites have been installed");

  here.chdir(baseDir);
}

proc collectCompOpts() {
  var compopts = new List.list(string);
  for prereq in prereqs() {
    compopts.pushBack("-I%s".format(Path.joinPath(prereq, "include")));
    compopts.pushBack("-L%s".format(Path.joinPath(prereq, "obj")));
  }
}

iter prereqs() {
  const baseDir = here.cwd();
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
