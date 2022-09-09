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
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Attempt at Aggregator that will go with the distributed collections
   implementation */
module Aggregator {

  // TODO: document
  record Aggregator {
     // whatever's stored here must be a class to avoid copying, right?
    const client;
    const updater;

    var bufferSize: int;
    // TODO: make this use a type that's passed in instead of keyType so that
    // we can maybe use this aggregator with lists and sets as well
    var buffers: [client.locDom][0..<bufferSize] sync client.keyType;

    // How to avoid race in adding updates to buffer?

    // TODO: impl
    proc update(key: client.keyType) {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc flush() {
      compilerError("unimplemented");
    }

    proc deinit() {
      flush();
    }
  }
}
