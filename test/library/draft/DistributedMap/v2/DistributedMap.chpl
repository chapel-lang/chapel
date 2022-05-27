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

/* Attempt at starting a distributed map type */
module DistributedMap {
  private use Map;

  // TODO: document type, methods
  class distributedMap {
    /* Type of map keys. */
    type keyType;
    /* Type of map values. */
    type valType;

    pragma "no doc"
    const targetLocales = Locales;

    pragma "no doc"
    var tables: [0..targetLocales.size] chpl__hashtable(keyType, valType);

    // TODO: I seem to recall someone not liking the idea of a lock per locale,
    // remind myself of why and maybe do something different.
    pragma "no doc"
    var locks: [0..targetLocales.size] owned _LockWrapper;

    // TODO: how to hash across locales?

    proc init(type keyType, type valType) {
      this.keyType = keyType;
      this.valType = valType;
    }
    // TODO: initializer that takes way to control hash across locales?
    // TODO: initializer that uses only a subset of the locales?

    // TODO: impl
    proc clear() {
      compilerError("unimplemented");
    }

    // TODO: impl
    inline proc const size {
      compilerError("unimplemented");
    }

    // TODO: impl
    inline proc const isEmpty(): bool {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc const contains(const k: keyType): bool {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc extend(m: map(keyType, valType)) {
      compilerError("unimplmented");
    }

    // TODO: impl and maybe equivalent on map type
    proc extend(m: distributedMap(keyType, valType)) {
      compilerError("unimplemented");
    }

    // TODO: proc this overloads?
    // TODO: getBorrowed? getValue (throws Map.KeyNotFoundError)? getAndRemove?
    // TODO: these?

    // TODO: impl
    iter keys() const ref {
      compilerError("unimplemented");
    }

    // TODO: impl
    iter items() {
      compilerError("unimplemented");
    }

    // TODO: impl
    iter values() ref {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc readThis(ch: channel) throws {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc writeThis(ch: channel) throws {
      compilerError("unimplemented");
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    //
    // Could also take tactic of making it slow but safe - lock the appropriate
    // place and add it as is, providing the alternative handling for
    // performance.
    proc add(in k: keyType, in v: valType): bool lifetime this < v {
      compilerError("unimplemented");
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    proc set(k: keyType, in v: valType): bool {
      compilerError("unimplemented");
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    proc addOrSet(in k: keyType, in v: valType) {
      compilerError("unimplemented");
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    proc remove(k: keyType): bool {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc toArray(): [] (keyType, valType) {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc keysToArray(): [] keyType {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc valuesToArray(): [] valType {
      compilerError("unimplemented");
    }

    // TODO: impl
    // Return new aggregator for distributed map that stores an operation that
    // should be performed on the value stored by the key.  The aggregator
    // will handle putting its contents into the map using a lower-level
    // interface.
    proc updateAggregator(updater) {
      compilerError("unimplemented");
    }
  }
}
