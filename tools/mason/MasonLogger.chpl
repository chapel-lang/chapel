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
  use ThirdParty.Log;
  import ThirdParty.TerminalColors.{style, styledText, red, yellow, blue};

  import Time.dateTime;
  import IO.format;

  private var color = ColorMode.AUTO;
  private var logStream = new shared LogStream();
  private var logFormat = new shared MasonLogFormat("%NAME%: %m%");
  private var pad = 0;

  proc colorModeFromString(s: string): ColorMode throws {
    const lower = s.toLower();
    if lower == "true" || lower == "always" then
      return ColorMode.ALWAYS;
    else if lower == "false" || lower == "never" then
      return ColorMode.NEVER;
    else if lower == "auto" then
      return ColorMode.AUTO;
    else
      throw new Error("Invalid value: '"+ s +"'");
  }

  proc setColorMode(c: ColorMode) {
    color = c;
    logFormat.setUseColor(c, logStream);
  }

  class MasonLogFormat: LogFormat {
    proc init(args...) {
      super.init((...args));
    }
    override proc format(
      timestamp: dateTime, level: LogLevel,
      moduleName: string, routineName: string, lineNumber: int,
      loggerName: string, message: string): string {
      const paddedName = try! ("%<"+pad:string+"s").format(loggerName);
      return super.format(timestamp, level,
                          moduleName, routineName, lineNumber,
                          paddedName, message);
    }
    override proc styleForLogName(level: LogLevel): styledText {
      var s = style().bold();
      if level == LogLevel.ERROR then
        s = s.fg(red());
      else if level == LogLevel.WARNING then
        s = s.fg(yellow());
      else if level == LogLevel.DEBUG then
        s = s.fg(blue());
      return s;
    }
  }

  proc getLogger(name: string): logger {
    pad = max(pad, name.size);
    return new logger(name,
                      colorMode=color,
                      logLevelEnvVar="MASON_LOG_LEVEL",
                      stream=logStream,
                      format=logFormat);
  }
}

