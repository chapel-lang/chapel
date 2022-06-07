/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

Download data from a URL or upload data to a URL.

For example, the following program downloads a web-page from http://example.com and outputs each line to stdout:

.. code-block:: chapel

  use URL;
  var urlreader = openUrlReader("http://example.com");
  var str:bytes;
  // Output each line read from the URL to stdout
  while(urlreader.readLine(str)) {
    write(str);
  }

.. note::

  This module is currently implemented using :mod:`Curl`. As such, please see
  the :mod:`Curl` documentation to see which protocols are available.

 */

module URL {
  public use IO;

  deprecated "openUrlReader with a style argument is deprecated"
  proc openUrlReader(url:string,
                     param kind=iokind.dynamic, param locking=true,
                     start:int(64) = 0, end:int(64) = max(int(64)),
                     style:iostyle)
                    : channel(false, kind, locking) throws {
    return openUrlReaderHelper(url, kind, locking, start, end,
                               style: iostyleInternal);
  }
  /*

  Open a channel reading from a particular URL.

  :arg url: which url to open (for example, "http://example.com").
  :arg kind: :type:`~IO.iokind` compile-time argument to determine the
              corresponding parameter of the :record:`~IO.channel` type. Defaults
              to ``iokind.dynamic``, meaning that the associated
              :record:`~IO.iostyle` controls the formatting choices.
  :arg locking: compile-time argument to determine whether or not the
                channel should use locking; sets the
                corresponding parameter of the :record:`~IO.channel` type.
                Defaults to true, but when safe, setting it to false
                can improve performance.
  :arg start: zero-based byte offset indicating where in the file the
              channel should start reading. Defaults to 0.
  :arg end: zero-based byte offset indicating where in the file the
            channel should no longer be allowed to read. Defaults
            to a ``max(int)`` - meaning no end point.
  :returns: an open reading channel to the requested resource.

  :throws SystemError: Thrown if a reading channel could not be returned.
   */
  proc openUrlReader(url:string,
                     param kind=iokind.dynamic, param locking=true,
                     start:int(64) = 0, end:int(64) = max(int(64)))
                    : channel(false, kind, locking) throws {
    return openUrlReaderHelper(url, kind, locking, start, end);
  }

  private
  proc openUrlReaderHelper(url:string,
                           param kind=iokind.dynamic, param locking=true,
                           start:int(64) = 0, end:int(64) = max(int(64)),
                           style:iostyleInternal = defaultIOStyleInternal())
    : channel(false, kind, locking) throws {
    use Curl;
    use CurlQioIntegration;
    var f = openCurlFile(url, iomode.r, style);
    return f.reader(kind=kind, locking=locking,
                    start=start, end=end);
  }

  deprecated "openUrlWriter with a style argument is deprecated"
  proc openUrlWriter(url:string,
                 param kind=iokind.dynamic, param locking=true,
                 start:int(64) = 0, end:int(64) = max(int(64)),
                 style:iostyle)
                : channel(true, kind, locking) throws {
    return openUrlWriterHelper(url, kind, locking, start, end,
                               style: iostyleInternal);
  }
  /*

  Open a channel writing to a particular URL.

  :arg path: which file to open (for example, "ftp://127.0.0.1/upload/test.txt")
  :arg kind: :type:`~IO.iokind` compile-time argument to determine the
             corresponding parameter of the :record:`~IO.channel` type. Defaults
             to ``iokind.dynamic``, meaning that the associated
             :record:`~IO.iostyle` controls the formatting choices.
  :arg locking: compile-time argument to determine whether or not the
                channel should use locking; sets the
                corresponding parameter of the :record:`~IO.channel` type.
                Defaults to true, but when safe, setting it to false
                can improve performance.
  :arg start: zero-based byte offset indicating where in the file the
              channel should start writing. Defaults to 0.
  :arg end: zero-based byte offset indicating where in the file the
            channel should no longer be allowed to write. Defaults
            to a ``max(int)`` - meaning no end point.
  :returns: an open writing channel to the requested resource.

  :throws SystemError: Thrown if a writing channel could not be returned.
  */
  proc openUrlWriter(url:string,
                 param kind=iokind.dynamic, param locking=true,
                 start:int(64) = 0, end:int(64) = max(int(64)))
                : channel(true, kind, locking) throws {
    return openUrlWriterHelper(url, kind, locking, start, end);
  }

  private proc openUrlWriterHelper(url:string,
                                   param kind=iokind.dynamic,
                                   param locking=true,
                                   start:int(64) = 0,
                                   end:int(64) = max(int(64)),
                                   style:iostyleInternal = defaultIOStyleInternal())
    : channel(true, kind, locking) throws {
    use Curl;
    use CurlQioIntegration;
    var f = openCurlFile(url, iomode.cw, style);
    return f.writer(kind=kind, locking=locking,
                    start=start, end=end);
  }

}
