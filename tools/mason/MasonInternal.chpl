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

module MasonInternal {
  import IO;
  import this.SafeCalls as Safe;


  enum logLevel { no, error, warn, info, debug };

  config const logs = logLevel.info;

  private var logWriter = IO.stdout;

  private proc doDebug do return logs>=logLevel.debug;
  private proc doInfo do return logs>=logLevel.info;
  private proc doWarn do return logs>=logLevel.warn;
  private proc doError do return logs>=logLevel.error;

  record logger {
    var prefix: string;

    proc infoln(f: string) {
      if doInfo then Safe.writeln(logWriter, message(f));
    }

    proc debugln(f: string) {
      if doDebug then Safe.writeln(logWriter, message(f));
    }

    proc debugf(f: string) {
      if doDebug then Safe.writef(logWriter, message(f));
    }

    proc debugf(f: string, args...) {
      if doDebug then Safe.writef(logWriter, message(f), (...args));
    }

    proc message(f) {
      return Safe.format("%s: %s", this.prefix, f);
    }
  }

  module SafeCalls {
    proc format(fmt:string, args...) {
      try {
        return fmt.format((...args));
      }
      catch e {
        writeln("Error formatting string");
        return "Unable to format";
      }
    }

    proc writef(writer, fmt: string, args...) {
      try {
        writer.writef(fmt, (...args));
      }
      catch e {
        // just use the non-throwing console output
        writeln("Error writing formatted debug output");
      }
    }

    proc writef(writer, fmt: string) {
      try {
        writer.writef(fmt);
      }
      catch e {
        // just use the non-throwing console output
        writeln("Error writing formatted debug output");
      }
    }

    proc writeln(writer, s: string) {
      try {
        writer.writeln(s);
      }
      catch e {
        // just use the non-throwing console output
        writeln("Error writing formatted debug output");
      }
    }
  }
}

