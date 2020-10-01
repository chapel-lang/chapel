/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

/*

.. highlight:: chapel

This module contains features that support compile-time reasoning
about version numbers in general, and Chapel version numbers
specifically.  In more detail, it features:

* :var:`chplVersion`: the version number of the copy of ``chpl`` used
  to compile the program.

* :type:`sourceVersion`: a type that can be used to represent a semantic
  version number plus an optional commit value.

* :proc:`createVersion`: a utility function for creating new version values

Version numbers in this module are represented using ``param`` values
to permit code specialization by being able to reason about versions
at compile-time.

The :type:`sourceVersion` type supports:

* being printed out or cast to a ``param`` string

* compile-time comparisons via ``==``, ``!=``, ``<``, ``<=``, ``>``,
  and ``>``.  Generally speaking, "less than" corresponds to "is an
  earlier version than."  For example::

    if chplVersion < createVersion(1,23) then
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
  chplVersion = createVersion(chplMajor, chplMinor, chplUpdate, chplSHA);


  /*
    A helper function that creates a new version value from its
    arguments.

    :arg major: The major version number
    :type major: `int`

    :arg minor: The minor version number
    :type minor: `int`

    :arg update: The optional update version number (defaults to 0)
    :type update: `int`

    :arg commit: The optional commit ID (defaults to "")
    :type commit: `string`

    :returns: A new version value of type :type:`sourceVersion`.
  */
  proc createVersion(param major: int,
                     param minor: int,
                     param update: int = 0,
                     param commit: string = ""): sourceVersion(?) {
    return new sourceVersion(major, minor, update, commit);

  }


  /*
    This record represents a software version in a Git repository.  It
    uses ``param`` values to represent its components in order to
    support compile-time comparison of version numbers which in turn
    permits code to specialize to specific versions of Chapel.  When
    printed or converted to a string, it is represented as ``version
    major.minor.update (commit)``.

    Note that ordered comparisons between two :type:`sourceVersion`
    values that only differ in their ``commit`` values are not
    supported due to the challenges involved in ordering commit
    values.  However, when a value with an empty ``update`` value is
    compared to one whose ``update`` is non-empty, the latter is
    considered to be earlier than (less than) the former, due to the
    interpretation that it represents a pre-release of the official
    release.  */

    record sourceVersion {
    /* The major version number.  For version ``2.0.1``, this would be
       ``2``. */
    param major: int;

    /* The minor version number.  For version ``2.0.1``, this would be
       ``0``. */
    param minor: int;

    /* The update version number.  For version ``2.0.1``, this would
       be ``1``. */
    param update: int;

    /* The commit ID of the version (e.g., a Git SHA) */
    param commit: string = "";

    pragma "no doc"
    proc writeThis(s) throws {
      s.write(this:string);
    }
  }

  // cast from sourceVersion to string
  pragma "no doc"
  proc _cast(type t: string, x: sourceVersion(?)) param {
    if (x.commit == "") then
      return ("version " + x.major:string + "." + x.minor:string + "." +
              x.update:string);
    else
      return ("version " + x.major:string + "." + x.minor:string + "." +
              x.update:string + " (" + x.commit + ")");
  }


  // helper functions for comparison operators

  private proc spaceship(param x: int, param y: int) param : int {
    if x < y then
      return -1;
    else if x == y then
      return 0;
    else
      return 1;
  }

  private proc spaceship(v1: sourceVersion(?),
                         v2: sourceVersion(?)) param : int {
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

  proc ==(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    return spaceship(v1, v2) == 0;
  }

  /*
    Equality/inequality operators between two values of type
    :type:`sourceVersion` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  proc !=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    return spaceship(v1, v2) != 0;
  }

  proc <(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval < 0;
  }

  proc <=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval <= 0;
  }

  proc >(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval > 0 && retval != 2;
  }

  /*
    Ordered comparisons between two values of type
    :type:`sourceVersion` are based on the ordering of the semantic
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
  proc >=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param retval = spaceship(v1, v2);
    if (retval == 2) then
      compilerError("can't compare versions that only differ by commit IDs");
    return retval >= 0 && retval != 2;
  }
}
