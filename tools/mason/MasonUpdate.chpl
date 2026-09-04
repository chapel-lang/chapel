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
module MasonUpdate {

use ArgumentParser;
use MasonHelp;
use MasonUtils;
use MasonEnv;
import MasonLogger;
import Package;
import Path;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;

/*
Update: Performs the upfront dependency resolution and generates the lock file.

Incompatible Version Resolution Strategy:

The current resolution strategy for Mason 0.1.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the project,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the project,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)
*/

private var log = MasonLogger.getLogger("mason update");

proc masonUpdate(args: [] string) throws {
  var parser = new argumentParser(helpHandler=new MasonUpdateHelpHandler());

  var updateFlag = parser.addFlag(name="update", flagInversion=true);
  var forceFlag = parser.addFlag(name="force", defaultValue=false);

  parser.parseArgs(args);

  var skipUpdate = MASON_OFFLINE;
  if updateFlag.hasValue() {
    skipUpdate = !updateFlag.valueAsBool();
  }
  Package.getMasonPackage(skipUpdate, show=true, force=forceFlag.valueAsBool());
}

}
