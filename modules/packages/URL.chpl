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
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

Create :rec:`channel`s for downloading or uploading data from a URL.

 */

module URL {

// aka openUrlReader
// openDownloader
// openUrlDownloader
proc openUrlReader(url:string,
                   param kind=iokind.dynamic, param locking=true,
                   start:int(64) = 0, end:int(64) = max(int(64)),
                   style:iostyle = defaultIOStyle())
                  : channel(false, kind, locking) throws {
  use Curl;
  use CurlQioIntegration;
  var f = openCurlFile(url, iomode.r, style);
  return f.reader(kind=kind, locking=locking,
                  start=start, end=end);
}

proc openUrlWriter(url:string,
               param kind=iokind.dynamic, param locking=true,
               start:int(64) = 0, end:int(64) = max(int(64)),
               style:iostyle = defaultIOStyle())
              : channel(true, kind, locking) throws {
  use Curl;
  use CurlQioIntegration;
  var f = openCurlFile(url, iomode.cw, style);
  return f.writer(kind=kind, locking=locking,
                  start=start, end=end);
}

}
