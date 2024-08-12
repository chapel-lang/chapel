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
  record aggregator {
     // whatever's stored here must be a class to avoid copying, right?
    const client;
    const updater;

    var bufferSize: int;
    // TODO: make this use a type that's passed in instead of keyType so that
    // we can maybe use this aggregator with lists and sets as well
    var buffers: [client.locDom][0..<bufferSize] sync client.keyType;

    proc init(clientInst, updaterFcf) {
      client = clientInst;
      updater = updaterFcf;
      bufferSize = 20;
    }

    proc init(clientInst, updaterFcf, buffSize) {
      client = clientInst;
      updater = updaterFcf;
      bufferSize = buffSize;
    }

    proc init=(other: aggregator(?)) {
      this.client = other.client;
      this.updater = other.updater;
      this.bufferSize = other.bufferSize;
      this.buffers = other.buffers; // should this be allowed?
    }

    // TODO: document
    proc ref update(key: client.keyType) {
      var loc: int = client.getLocaleForKey(key);

      var idx = 0;
      while (idx < bufferSize) {
        if buffers[loc][idx].isFull {
          idx += 1;
        } else {
          buffers[loc][idx].writeEF(key);
          return;
        }
      }

      if (idx == bufferSize) {
        // TODO:
        // Should we flush the individual locale buffer?  Maybe have flush call
        // out to a flush that takes in the locale to update?  That'd avoid
        // locking everything when one buffer gets full and be more optimal for
        // remote work
        flush();
        buffers[loc][0].writeEF(key);
      }
    }

    // TODO: document
    proc flush() {
      for loc in client.locDom {
        on loc {
          client.locks[loc].lock();
          var max = 0;
          var arrCopy: [0..<bufferSize] client.keyType;
          for i in arrCopy.domain {
            if buffers[loc][i].isFull {
              arrCopy[i]  = buffers[loc][i].readFE();
              max = i;
            }
          }

          for i in arrCopy.domain {
            if (i <= max) {
              if (client.containsUnlocked(arrCopy[i])) {
                var curVal = try! client.getValueUnlocked(arrCopy[i]);
                this.updater(curVal);
                client.setUnlocked(arrCopy[i], curVal);
              } else {
                var newVal: client.valType; // use default value for val type
                this.updater(newVal);
                client.addUnlocked(arrCopy[i], newVal);
              }
            }
          }

          client.locks[loc].unlock();
        }
      }
    }

    proc deinit() {
      flush();
    }
  }
}
