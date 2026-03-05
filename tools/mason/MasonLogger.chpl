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

module MasonLogger {
  import IO;
  import this.SafeCalls as Safe;


  // Note: we use >= to determine whether we should output something. So, if you
  // change enum values, make sure to put things in order, where the lower
  // values mean lower verbosity.
  enum logLevel { no, error, warn, info, debug }

  var logs = getDefaultLogs();

  private proc getDefaultLogs() {
    var ll = logLevel.info;

    import OS, OS.POSIX;
    // this is in lieu of proper mason log level control
    // https://github.com/chapel-lang/chapel/issues/28163
    const logLevelChar = OS.POSIX.getenv("MASON_LOG_LEVEL");
    if logLevelChar != nil {
      try {
        const s = string.createCopyingBuffer(logLevelChar).toLower();
        ll = if s == "none" then logLevel.no else s:logLevel;
      } catch {
        // do nothing
      }
    }
    return ll;
  }

  private proc doDebug do return logs>=logLevel.debug;
  private proc doInfo do return logs>=logLevel.info;
  private proc doWarn do return logs>=logLevel.warn;
  private proc doError do return logs>=logLevel.error;

  private var colorOutput = true;
  proc setUseColorOutput(flag: bool) {
    colorOutput = flag;
  }

  private var logWriter = IO.stdout;
  private var pad = 0;

  record logger {
    var prefix: string;

    proc init(prefix) {
      this.prefix = prefix;

      pad = max(pad, prefix.size);
    }

    // TODO make all variadic
    proc info(s: string) {
      if doInfo then Safe.writef(logWriter, addPrefix("%s"), s);
    }

    proc infoln(s: string) {
      if doInfo then Safe.writeln(logWriter, addPrefix(s));
    }

    proc infof(f: string, args...) {
      if doInfo then Safe.writef(logWriter, addPrefix(f), (...args));
    }

    proc warn(s: string) {
      if doWarn then Safe.writef(logWriter, addPrefix("%s"), s);
    }

    proc warnln(s: string) {
      if doWarn then Safe.writeln(logWriter, addPrefix(s));
    }

    proc warnf(f: string, args...) {
      if doWarn then Safe.writef(logWriter, addPrefix(f), (...args));
    }

    proc debug(s: string) {
      if doDebug then Safe.writef(logWriter, addPrefix("%s"), s);
    }

    proc debugln(s: string) {
      if doDebug then Safe.writeln(logWriter, addPrefix(s));
    }

    proc debugf(f: string, args...) {
      if doDebug then Safe.writef(logWriter, addPrefix(f), (...args));
    }

    proc error(s: string) {
      if doError then Safe.writef(logWriter, addPrefix("%s"), s);
    }

    proc errorln(s: string) {
      if doError then Safe.writeln(logWriter, addPrefix(s));
    }

    proc errorf(f: string, args...) {
      if doError then Safe.writef(logWriter, addPrefix(f), (...args));
    }

    proc addPrefix(f) {
      proc bold(s) {
        if !colorOutput then return s;

        param start = "\x1b[1m";
        param reset = "\x1b[0m";

        return start+s+reset;
      }

      const prefixFmt = bold(Safe.format("%%<%is:", pad));
      return Safe.format(prefixFmt+" %s", this.prefix, f);
    }
  }

  module SafeCalls {
    import IO.stderr;

    proc format(fmt:string, args...) {
      try {
        return fmt.format((...args));
      } catch e {
        try! stderr.writeln("Error formatting string\n", e);
        return "Unable to format";
      }
    }

    proc writef(writer, fmt: string, args...) {
      try {
        writer.writef(fmt, (...args));
      } catch e {
        try! stderr.writeln("Error writing formatted debug output\n", e);
      }
    }

    proc writeln(writer, s: string) {
      try {
        writer.writeln(s);
      } catch e {
        try! stderr.writeln("Error writing debug output\n", e);
      }
    }
  }
}

