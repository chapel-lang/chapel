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

/* Support for reasoning about version numbers.

.. highlight:: chapel

This module contains features that support reasoning about version numbers.
The :type:`versionValue` type supports compile-time reasoning about version numbers
in general, and Chapel version numbers specifically, while the :type:`version`
type supports run-time reasoning about version numbers.

In more detail, the module features:

* :var:`chplVersion`: the version number of the copy of ``chpl`` used
  to compile the program.

* :type:`versionValue`: a type that can be used to represent a semantic
  version number plus an optional commit value. This type supports
  `compile-time` reasoning about version numbers.

* :type:`version`: a type that can be used to represent a semantic
  version number plus an optional commit value. This type supports
  `run-time` reasoning about version numbers.

Version numbers in :type:`versionValue` are represented using ``param`` values
to permit code specialization by being able to reason about versions
at `compile-time`.

The :type:`versionValue` type supports:

* being printed out or cast to a ``param`` string

* `compile-time` comparisons via ``==``, ``!=``, ``<``, ``<=``, ``>``,
  and ``>``.  Generally speaking, "less than" corresponds to "is an
  earlier version than."  For example::

    if chplVersion < new versionValue(1,23) then
      compilerWarning("This package doesn't support 'chpl' prior to 1.23.0");

Version numbers in :type:`version` are represented using ``var`` values
to permit constructing and assigning version number values at `run-time`.

The :type:`version` type supports:

* being printed out or cast to a string

* `run-time` comparisons via ``==``, ``!=``, ``<``, ``<=``, ``>``,
  and ``>``. Generally speaking, "less than" corresponds to "is an
  earlier version than."

The type :type:`versionValue` is useful for modifying compilation behavior,
such as requiring a minimum version of the Chapel compiler. The type
:type:`version` is useful for reasoning about version numbers that are not
known at compile-time, such as values read from a text file.

Comparisons between two :type:`versionValue`, two :type:`version`, or
a :type:`versionValue` and a :type:`version` are supported.
Equality/inequality operations check whether or not the two values
have identical major, minor, update, and commit values.

Ordered comparisons are based on the ordering of the semantic
versions of the two values, as defined by their ``major``,
``minor``, and ``update`` components.
Note that if the two values have identical semantic versions,
any cases that rely on an ordering of the commits will generate an error
if the values have differing, non-empty ``commit`` values due to the
challenge of ordering commits. An empty ``commit`` value is considered to
come after (be greater than) a non-empty value, as the former is considered
an official release and the latter a pre-release.

For example::

    // any of the following variable declarations can be replaced with
    // `const vN = new versionValue(...)` instead of using the version type
    // and the comparison results would remain the same

    var v1 = new version(1, 2, 3);
    var v2 = new version(1, 2, 4);
    var v3 = new version(1, 2, 4, "abc");
    var v4 = new version(1, 2, 4, "def");

    writeln(v1 < v2); // prints "true"
    writeln(v2 < v3); // prints "false" as v3 and v4 are considered pre-releases of v2
    writeln(v3 < v4); // error: cannot compare commits of different versions

    var v5 = new version(1, 2, 4, "abc");
    writeln(v3 == v5); // prints "true"
    writeln(v3 != v5); // prints "false"

*/

module Version {
  // Query the compiler to get its version number and cache those values
  private param chplMajor = __primitive("version major");
  private param chplMinor = __primitive("version minor");
  private param chplUpdate = __primitive("version update");
  private param chplSHA = __primitive("version sha");


  /*
    The version of ``chpl`` used to compile this program.  For an
    official Chapel release, this will have an empty ``commit`` value,
    while for a pre-release, it will indicate the Git SHA.

    Note that, for historical reasons, Chapel ``1.x.y``/``2.x.y``
    corresponds to version ``0.x.y``/``1.x.y`` in traditional semantic
    versioning.
  */
  const chplVersion;
  chplVersion = new versionValue(chplMajor, chplMinor, chplUpdate, chplSHA);

  /*
    This record represents a software version that is modeled after
    a semantic version, though not 100% true to the semver spec. The main
    deviation from the spec is that ``versionValue`` doesn't support pre-release
    identifiers and the version and optional build/commit value are separated
    with a space rather than with ``+``. See semver.org for details on the spec.

    The ``versionValue`` uses ``param`` values to represent its components in
    order to support `compile-time` comparison of version numbers which in turn
    permits code to specialize to specific versions of Chapel. When printed or
    converted to a string, it is represented as ``major.minor.update (commit)``.
  */

  record versionValue : writeSerializable {
    /*
      The major version number. For version ``2.0.1``, this would be ``2``.
    */
    param major: int;

    /*
      The minor version number. For version ``2.0.1``, this would be ``0``.
    */
    param minor: int;

    /*
      The update version number. For version ``2.0.1``, this would be ``1``.
    */
    param update: int;

    /* The commit ID of the version (e.g., a Git SHA) */
    param commit: string = "";

    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writer.write(this:string);
    }

    @chpldoc.nodoc
    proc init(param major: int,
              param minor: int,
              param update: int = 0,
              param commit: string = "") {
      this.major = major;
      this.minor = minor;
      this.update = update;
      this.commit = commit;
    }

  }

  @chpldoc.nodoc
  operator :(x: versionValue(?), type t: version) {
    return new version(x.major, x.minor, x.update, x.commit);
  }

  // cast from versionValue to string
  @chpldoc.nodoc
  operator :(x: versionValue(?), type t: string) param {
    if (x.commit == "") then
      return (x.major:string + "." + x.minor:string + "." +
              x.update:string);
    else
      return (x.major:string + "." + x.minor:string + "." +
              x.update:string + " (" + x.commit + ")");
  }


  // helper function for comparison operators
  private proc spaceship(v1: versionValue(?),
                         v2: versionValue(?)) param : int {
    param majComp = spaceship(v1.major, v2.major);
    if majComp != 0 {
      return majComp;
    } else {
      param minComp = spaceship(v1.minor, v2.minor);
      if minComp != 0 {
        return minComp;
      } else {
        param upComp = spaceship(v1.update, v2.update);
        if upComp != 0 {
          return upComp;
        } else if v1.commit == v2.commit {
          return 0;
        } else if v1.commit == "" {
          return 1;
        } else if v2.commit == "" then {
          return -1;
        } else {
          // sentinel for "not comparable"
          return 2;
        }
      }
    }
  }

  // Comparisons between sourceVersions
  @chpldoc.nodoc
  operator versionValue.==(v1: versionValue(?),
                           v2: versionValue(?)) param : bool {
    return spaceship(v1, v2) == 0;
  }

  /*
    Equality/inequality operators between two values of type
    :type:`versionValue` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  @chpldoc.nodoc
  operator versionValue.!=(v1: versionValue(?),
                           v2: versionValue(?)) param : bool {
    return spaceship(v1, v2) != 0;
  }

  @chpldoc.nodoc
  operator versionValue.<(v1: versionValue(?),
                          v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  @chpldoc.nodoc
  operator versionValue.<=(v1: versionValue(?),
                           v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

  @chpldoc.nodoc
  operator versionValue.>(v1: versionValue(?),
                          v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval > 0 && retval != 2;
  }

  /*
    Ordered comparisons between two values of type
    :type:`versionValue` are based on the ordering of the semantic
    versions of the two values, as defined by their ``major``,
    ``minor``, and ``update`` components.  If the two values have
    identical semantic versions, any cases that rely on an ordering of
    the commits will generate a compile-time error if the values have
    differing, non-empty ``commit`` values due to the challenge of
    ordering commits at compile-time.  An empty ``commit`` value is
    considered to come after (be greater than) a non-empty value, as
    the former is considered an official release and the latter a
    pre-release.
  */
  @chpldoc.nodoc
  operator versionValue.>=(v1: versionValue(?),
                           v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }


  /*
    This record represents a software version similar to the :type:`versionValue`.
    It uses ``var`` values to represent its components in order to support
    `run-time` building and comparison of version numbers. When printed or
    converted to a string, it is represented as ``major.minor.update (commit)``.
    Unlike :type:`versionValue`, a ``version`` can be created and modified at runtime.
  */
  record version : writeSerializable {
    /*
      The major version number. For version ``2.0.1``, this would be ``2``.
      Defaults to ``-1``
    */
    var major: int;

    /*
      The minor version number. For version ``2.0.1``, this would be ``0``.
      Defaults to ``-1``
    */
    var minor: int;

    /*
      The update version number. For version ``2.0.1``, this would be ``1``.
      Defaults to ``0``
    */
    var update: int;

    /*
      The commit ID of the version (e.g., a Git SHA)
      Defaults to ``""``
    */
    var commit: string = "";


    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writer.write(this:string);
    }

    @chpldoc.nodoc
    proc init(major: int = -1,
              minor: int = -1,
              update: int = 0,
              commit: string = "") {
      this.major = major;
      this.minor = minor;
      this.update = update;
      this.commit = commit;
    }

    @chpldoc.nodoc
    proc init(otherVersion: version) {
      this.major = otherVersion.major;
      this.minor = otherVersion.minor;
      this.update = otherVersion.update;
      this.commit = otherVersion.commit;
    }

    @chpldoc.nodoc
    proc init=(otherVersion: version) {
      major = otherVersion.major;
      minor = otherVersion.minor;
      update = otherVersion.update;
      commit = otherVersion.commit;
    }

    @chpldoc.nodoc
    proc init=(otherVersion: versionValue(?)) {
      major = otherVersion.major;
      minor = otherVersion.minor;
      update = otherVersion.update;
      commit = otherVersion.commit;
    }

  }

  @chpldoc.nodoc
    operator = (ref LHS:version, otherVersion: version) {
      LHS.major = otherVersion.major;
      LHS.minor = otherVersion.minor;
      LHS.update = otherVersion.update;
      LHS.commit = otherVersion.commit;
    }

    @chpldoc.nodoc
    operator = (ref LHS:version, otherVersion: versionValue(?)) {
      LHS.major = otherVersion.major;
      LHS.minor = otherVersion.minor;
      LHS.update = otherVersion.update;
      LHS.commit = otherVersion.commit;
    }


  // cast from version to string
  @chpldoc.nodoc
  operator :(x: version, type t: string) const {
    if (x.commit == "") then
      return (x.major:string + "." + x.minor:string + "." +
              x.update:string);
    else
      return (x.major:string + "." + x.minor:string + "." +
              x.update:string + " (" + x.commit + ")");
  }

  private proc spaceship(v1: version,
                         v2: version) : int {
    var majComp = spaceship(v1.major, v2.major);
    if majComp != 0 {
      return majComp;
    } else {
      var minComp = spaceship(v1.minor, v2.minor);
      if minComp != 0 {
        return minComp;
      } else {
        var upComp = spaceship(v1.update, v2.update);
        if upComp != 0 {
          return upComp;
        } else if v1.commit == v2.commit {
          return 0;
        } else if v1.commit == "" {
          return 1;
        } else if v2.commit == "" then {
          return -1;
        } else {
          // sentinel for "not comparable"
          return 2;
        }
      }
    }
  }

  // Comparisons between versions
  /*
    Equality/inequality operators between two objects of type
    :type:`version` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  @chpldoc.nodoc
  operator version.==(v1: version,
                      v2: version) : bool {
    return spaceship(v1, v2) == 0;
  }

  @chpldoc.nodoc
  operator version.!=(v1: version,
                             v2: version) : bool {
    return spaceship(v1, v2) != 0;
  }

  @chpldoc.nodoc
  operator version.<(v1: version,
                            v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  @chpldoc.nodoc
  operator version.<=(v1: version,
                             v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

  @chpldoc.nodoc
  operator version.>(v1: version,
                            v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval > 0 && retval != 2;
  }

  /*
    Ordered comparisons between two values of type
    :type:`version` are based on the ordering of the semantic
    versions of the two values, as defined by their ``major``,
    ``minor``, and ``update`` components.  If the two values have
    identical semantic versions, any cases that rely on an ordering of
    the commits will generate a run-time error if the values have
    differing, non-empty ``commit`` values due to the challenge of
    ordering commits.  An empty ``commit`` value is considered to come
    after (be greater than) a non-empty value, as the former is considered
    an official release and the latter a pre-release.
  */
  @chpldoc.nodoc
  operator version.>=(v1: version,
                      v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }

  // Comparisons between version and versionValue

  /*
    Equality/inequality operators between and object of type
    :type:`version` and a value of type :type:versionValue`
    check whether or not the two have identical major, minor, update,
    and commit values.
  */
  @chpldoc.nodoc
  operator ==(v1: version,
              v2: versionValue(?)) : bool {
    return spaceship(v1, v2) == 0;
  }

  @chpldoc.nodoc
  operator ==(v1: versionValue(?),
              v2: version) : bool {
    return v2 == v1;
  }

  @chpldoc.nodoc
  operator !=(v1: version,
              v2: versionValue(?)) : bool {
    return spaceship(v1, v2) != 0;
  }

  @chpldoc.nodoc
  operator !=(v1: versionValue(?),
              v2: version) : bool {
    return v2 != v1;
  }

  @chpldoc.nodoc
  operator <(v1: version,
             v2: versionValue(?)) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  @chpldoc.nodoc
  operator <(v1: versionValue(?),
             v2: version) : bool throws {
    return v2 > v1;
  }

  @chpldoc.nodoc
  operator <=(v1: version,
              v2: versionValue(?)) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

  @chpldoc.nodoc
  operator <=(v1: versionValue(?),
              v2: version) : bool throws {
    return v2 >= v1;
  }

  @chpldoc.nodoc
  operator >(v1: version,
             v2: versionValue(?)) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval > 0 && retval != 2;
  }

  @chpldoc.nodoc
  operator >(v1: versionValue(?),
             v2: version) : bool throws {
    return v2 < v1;
  }

  @chpldoc.nodoc
  operator >=(v1: version,
              v2: versionValue(?)) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }

  @chpldoc.nodoc
  operator >=(v1: versionValue(?),
              v2: version) : bool throws {
    return v2 <= v1;
  }

  private proc spaceship(v1: version,
                         v2: versionValue(?)) : int {
    const majComp = spaceship(v1.major, v2.major);
    if majComp != 0 {
      return majComp;
    } else {
      const minComp = spaceship(v1.minor, v2.minor);
      if minComp != 0 {
        return minComp;
      } else {
        const upComp = spaceship(v1.update, v2.update);
        if upComp != 0 {
          return upComp;
        } else if v1.commit == v2.commit {
          return 0;
        } else if v1.commit == "" {
          return 1;
        } else if v2.commit == "" then {
          return -1;
        } else {
          // sentinel for "not comparable"
          return 2;
        }
      }
    }
  }

  // helper function for comparison operators
  private proc spaceship(x: int, y: int) const : int {
    if x < y then
      return -1;
    else if x == y then
      return 0;
    else
      return 1;
  }

  // helper function for comparison operators
  private proc spaceship(param x: int, param y: int) param : int {
    if x < y then
      return -1;
    else if x == y then
      return 0;
    else
      return 1;
  }


  /*
    Error class thrown when two versions are compared that cannot be compared.

    For example, two versions differing only in commit IDs cannot be compared.
  */
  class VersionComparisonError : Error {
    @chpldoc.nodoc
    var msg:string;

    @chpldoc.nodoc
    proc init(msg:string) {
      this.msg = msg;
    }

    @chpldoc.nodoc
    override proc message() {
      return msg;
    }
  }

}
