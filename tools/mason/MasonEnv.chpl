/*
 * Copyright 2004-2018 Cray Inc.
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

use MasonUtils;
use MasonHelp;

proc MASON_HOME : string {
  const envHome = getEnv("MASON_HOME");
  const default = getEnv('HOME') + "/.mason";

  const masonHome = if envHome != "" then envHome else default;

  return masonHome;
}

proc MASON_CACHED_REGISTRY : string {
  const env = getEnv("MASON_CACHED_REGISTRY");
  if env == "" {
    return MASON_HOME + '/registry';
  } else {
    return env;
  }
}

proc MASON_REGISTRY : string {
  const env = getEnv("MASON_REGISTRY");
  const default = "https://github.com/chapel-lang/mason-registry";

  if env == "" {
    return default;
  } else {
    return env;
  }
}

proc masonEnv(args) {
  if hasOptions(args, "-h", "--help") {
    masonEnvHelp();
    exit(0);
  }

  const debug = hasOptions(args, "--debug");

  proc printVar(name : string, in val : string) {
    if getEnv(name) != "" then
      val += " *";

    writeln(name, ": ", val);
  }

  printVar("MASON_HOME", MASON_HOME);
  printVar("MASON_REGISTRY", MASON_REGISTRY);

  if debug {
    printVar("MASON_CACHED_REGISTRY", MASON_CACHED_REGISTRY);
  }
}
