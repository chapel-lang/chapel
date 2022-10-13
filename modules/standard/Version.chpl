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

/* Support for reasoning about version numbers.

.. highlight:: chapel

This module contains features that support compile-time reasoning
about version numbers in general, and Chapel version numbers
specifically.  In more detail, it features:

* :var:`chplVersion`: the version number of the copy of ``chpl`` used
  to compile the program.

* :type:`versionValue`: a type that can be used to represent a semantic
  version number plus an optional commit value.

* :proc:`versionValue`: a utility function for creating new version values

Version numbers in this module are represented using ``param`` values
to permit code specialization by being able to reason about versions
at compile-time.

The :type:`versionValue` type supports:

* being printed out or cast to a ``param`` string

* compile-time comparisons via ``==``, ``!=``, ``<``, ``<=``, ``>``,
  and ``>``.  Generally speaking, "less than" corresponds to "is an
  earlier version than."  For example::

    if chplVersion < new versionValue(1,23) then
      compilerWarning("This package doesn't support 'chpl' prior to 1.23.0");

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
    This record represents a software version in a Git repository.  It
    uses ``param`` values to represent its components in order to
    support compile-time comparison of version numbers which in turn
    permits code to specialize to specific versions of Chapel.  When
    printed or converted to a string, it is represented as
    ``major.minor.update (commit)``.

    Note that ordered comparisons between two :type:`versionValue`
    values that only differ in their ``commit`` values are not
    supported due to the challenges involved in ordering commit
    values.  However, when a value with an empty ``update`` value is
    compared to one whose ``update`` is non-empty, the latter is
    considered to be earlier than (less than) the former, due to the
    interpretation that it represents a pre-release of the official
    release.
  */

  record versionValue {
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

    pragma "no doc"
    proc writeThis(s) throws {
      s._write(this:string);
    }

    pragma "no doc"
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

  pragma "no doc"
  operator :(x: versionValue(?), type t: version) {
    return new version(x.major, x.minor, x.update, x.commit);
  }

  // cast from versionValue to string
  pragma "no doc"
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

  operator versionValue.==(v1: versionValue(?),
                            v2: versionValue(?)) param : bool {
    return spaceship(v1, v2) == 0;
  }

  /*
    Equality/inequality operators between two values of type
    :type:`versionValue` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  operator versionValue.!=(v1: versionValue(?),
                            v2: versionValue(?)) param : bool {
    return spaceship(v1, v2) != 0;
  }

  operator versionValue.<(v1: versionValue(?),
                           v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  operator versionValue.<=(v1: versionValue(?),
                            v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

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
  operator versionValue.>=(v1: versionValue(?),
                            v2: versionValue(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }


  /*
    This record represents a software version in a Git repository.  It
    uses ``var`` values to represent its components in order to
    support run-time building and comparison of version numbers.
    When printed or converted to a string, it is represented as
    ``major.minor.update (commit)``.

    Note that ordered comparisons between two :type:`version`
    types that only differ in their ``commit`` values are not
    supported due to the challenges involved in ordering commit
    values.  However, when a value with an empty ``update`` value is
    compared to one whose ``update`` is non-empty, the latter is
    considered to be earlier than (less than) the former, due to the
    interpretation that it represents a pre-release of the official
    release.
  */

  record version {
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


    pragma "no doc"
    proc writeThis(s) throws {
      s.write(this:string);
    }

    pragma "no doc"
    proc init(major: int = -1,
              minor: int = -1,
              update: int = 0,
              commit: string = "") {
      this.major = major;
      this.minor = minor;
      this.update = update;
      this.commit = commit;
    }

    pragma "no doc"
    proc init(otherVersion: version) {
      this.major = otherVersion.major;
      this.minor = otherVersion.minor;
      this.update = otherVersion.update;
      this.commit = otherVersion.commit;
    }

    pragma "no doc"
    proc init=(otherVersion: version) {
      major = otherVersion.major;
      minor = otherVersion.minor;
      update = otherVersion.update;
      commit = otherVersion.commit;
    }

    pragma "no doc"
    proc init=(otherVersion: versionValue(?)) {
      major = otherVersion.major;
      minor = otherVersion.minor;
      update = otherVersion.update;
      commit = otherVersion.commit;
    }

  }

  pragma "no doc"
    operator = (ref LHS:version, otherVersion: version) {
      LHS.major = otherVersion.major;
      LHS.minor = otherVersion.minor;
      LHS.update = otherVersion.update;
      LHS.commit = otherVersion.commit;
    }

    pragma "no doc"
    operator = (ref LHS:version, otherVersion: versionValue) {
      LHS.major = otherVersion.major;
      LHS.minor = otherVersion.minor;
      LHS.update = otherVersion.update;
      LHS.commit = otherVersion.commit;
    }


  // cast from version to string
  pragma "no doc"
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

  // Comparisons between programVersions
  operator version.==(v1: version,
                             v2: version) : bool {
    return spaceship(v1, v2) == 0;
  }

  /*
    Equality/inequality operators between two values of type
    :type:`version` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  operator version.!=(v1: version,
                             v2: version) : bool {
    return spaceship(v1, v2) != 0;
  }

  operator version.<(v1: version,
                            v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  operator version.<=(v1: version,
                             v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

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
    the commits will generate a compile-time error if the values have
    differing, non-empty ``commit`` values due to the challenge of
    ordering commits at compile-time.  An empty ``commit`` value is
    considered to come after (be greater than) a non-empty value, as
    the former is considered an official release and the latter a
    pre-release.
  */
  operator version.>=(v1: version,
                             v2: version) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }

  // Comparisons between version and versionValue
  operator ==(v1: version,
              v2: versionValue) : bool {
    return spaceship(v1, v2) == 0;
  }

  operator ==(v1: versionValue,
              v2: version) : bool {
    return v2 == v1;
  }

  /*
    Equality/inequality operators between values of :type:`version` and
    :type:`versionValue` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  operator !=(v1: version,
              v2: versionValue) : bool {
    return spaceship(v1, v2) != 0;
  }

  operator !=(v1: versionValue,
              v2: version) : bool {
    return v2 != v1;
  }

  operator <(v1: version,
             v2: versionValue) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  operator <(v1: versionValue,
             v2: version) : bool throws {
    return v2 > v1;
  }

  operator <=(v1: version,
              v2: versionValue) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

  operator <=(v1: versionValue,
              v2: version) : bool throws {
    return v2 >= v1;
  }

  operator >(v1: version,
             v2: versionValue) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval > 0 && retval != 2;
  }

  operator >(v1: versionValue,
             v2: version) : bool throws {
    return v2 < v1;
  }

  operator >=(v1: version,
              v2: versionValue) : bool throws {
    const retval = spaceship(v1, v2);
    if (retval == 2) then
      throw new VersionComparisonError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }

  operator >=(v1: versionValue,
              v2: version) : bool throws {
    return v2 <= v1;
  }

  private proc spaceship(v1: version(?),
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



  class VersionComparisonError : Error {
    var msg:string;

    proc init(msg:string) {
      this.msg = msg;
    }

    override proc message() {
      return msg;
    }
  }

}
