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

* two types that can be used to represent version numbers:

  * :type:`semanticVersion` which represents a semantic version value

  * :type:`sourceVersion` which is a semantic version plus a commit value

* :proc:`version`: a utility function for creating new version values

Version types in this module are defined in terms of ``param`` values to
support compile-time reasoning about versions and code specialization.

The version types support:

* being printed out or cast to a ``param`` string

* compile-time comparisons between version values via ``==``, ``!=``,
  ``<``, ``<=``, ``>``, and ``>``, including mixed version types.
  Generally speaking, "less than" corresponds to "is an earlier
  version than."  For example::

    if chplVersion < version(1,23) then
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
    official Chapel release, this will have type
    :type:`semanticVersion` while for a pre-release, it will have
    type :type:`sourceVersion`.

    Note that Chapel ``1.x.y``/``2.x.y`` corresponds to version
    ``0.x.y``/``1.x.y`` in traditional semantic versioning.
  */

  const chplVersion;
  chplVersion = version(chplMajor, chplMinor, chplUpdate, chplSHA);


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

    :returns: A new version value.  If ``commit == ""``, this routine
              returns a value of type :type:`semanticVersion`
              otherwise it returns a value of type
              :type:`sourceVersion`.
  */
  proc version(param major: int, param minor: int,
               param update: int = 0, param commit: string = "") {
    if commit == "" then
      return new semanticVersion(major, minor, update);
    else
      return new sourceVersion(major, minor, update, commit);

  }


  /*
    This record represents a semantic version value.  It uses
    ``param`` values to represent its components in order to support
    compile-time comparison of version numbers which in turn permits
    code to specialize to specific versions of Chapel.  When printed
    or converted to a string, it is represented as ``version
    major.minor.update``.
  */
  record semanticVersion {
    /* The major version number.  For version ``2.0.1``, this would be
       ``2``. */
    param major: int;

    /* The minor version number.  For version ``2.0.1``, this would be
       ``0``. */
    param minor: int;

    /* The update version number.  For version ``2.0.1``, this would
       be ``1``. */
    param update: int;

    pragma "no doc"
    proc writeThis(s) throws {
      s.write(this:string);
    }
  }

  // cast from semanticVersion to string
  pragma "no doc"
  proc _cast(type t: string, x: semanticVersion(?)) param : string {
    return "version " + x.major:string + "." + x.minor:string + "." +
           x.update:string;
  }


  /*
    This record represents a software version in a Git repository.  It
    uses ``param`` values to represent its components in order to
    support compile-time comparison of version numbers which in turn
    permits code to specialize to specific versions of Chapel.  When
    printed or converted to a string, it is represented as ``version
    major.minor.update (commit)``.

    Note that ordered comparisons between two :type:`sourceVersion`
    values with identical semantic versions are not supported due to
    the challenges involved in ordering commit values.  Moreover, when
    a :type:`semanticVersion` is compared with a :type:`sourceVersion`
    that has the identical semantic version, the :type:`sourceVersion`
    value is considered to be less than the :type:`semanticVersion`
    using the model that it represents a pre-release of the official
    release represented by the semantic version.
  */
  record sourceVersion {
    forwarding version only major, minor, update;

    /* The semantic version portion of the version number. */
    const version: semanticVersion(?);

    /* The commit ID of the version (e.g., a git SHA) */
    param commit: string;

    pragma "no doc"
    proc init(param major: int, param minor: int, param update: int,
              param commit: string) {
      this.version = new semanticVersion(major, minor, update);
      this.commit = commit;
      if commit == "" then
        compilerError("sourceVersion values must have a commit value");
    }

    pragma "no doc"
    proc writeThis(s) throws {
      s.write(this:string);
    }
  }

  // cast from sourceVersion to string
  pragma "no doc"
  proc _cast(type t: string, x: sourceVersion(?)) {
    return x.version:string + " (" + x.commit + ")";
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

  private proc spaceship(v1: semanticVersion(?),
                         v2: semanticVersion(?)) param : int {
    param majComp = spaceship(v1.major, v2.major);
    if majComp != 0 then
      return majComp;
    param minComp = spaceship(v1.minor, v2.minor);
    if minComp != 0 then
      return minComp;
    return spaceship(v1.update, v2.update);
  }


  // Comparisons between semanticVersions

  proc ==(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) == 0;
  }

  /*
    Equality/inequality operators between two values of type
    :type:`semanticVersion` check whether or not the two values
    have identical major, minor, and update values.
  */
  proc !=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) != 0;
  }

  proc <(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) < 0;
  }

  proc <=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) <= 0;
  }

  proc >(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) > 0;
  }

  /*
    Ordered comparisons between two values of type
    :type:`semanticVersion` are based on the lexical ordering of each
    value's ``major.minor.update`` value, reflecting an "was released
    before/after" relationship.
  */
  proc >=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) >= 0;
  }


  // Comparisons between sourceVersions

  proc ==(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    return (v1.version == v2.version &&
            v1.commit == v2.commit);
  }

  /*
    Equality/inequality operators between two values of type
    :type:`sourceVersion` check whether or not the two values
    have identical major, minor, update, and commit values.
  */
  proc !=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    return (v1.version != v2.version ||
            v1.commit != v2.commit);
  }

  proc <(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp < 0;
    } else if v1.commit == v2.commit {
      return false;
    } else {
      compilerError("can't compare sourceVersions that only differ by commit IDs");
      return false;
    }
  }

  proc <=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp < 0;
    } else if v1.commit == v2.commit {
      return true;
    } else {
      compilerError("can't compare sourceVersions that only differ by commit IDs");
      return false;
    }
  }

  proc >(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp > 0;
    } else if v1.commit == v2.commit {
      return false;
    } else {
      compilerError("can't compare sourceVersions that only differ by commit IDs");
      return false;
    }
  }

  /*
    Ordered comparisons between two values of type
    :type:`sourceVersion` are based on the ordering of the semantic
    versions of the two values.  If the two values have identical
    semantic versions, any cases that rely on an ordering of the
    commits will generate a compile-time error due to the challenge of
    ordering commits at compile-time.
  */
  proc >=(v1: sourceVersion(?), v2: sourceVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp > 0;
    } else if v1.commit == v2.commit {
      return true;
    } else {
      compilerError("can't compare sourceVersions that only differ by commit IDs");
      return false;
    }
  }

  // semanticVersion / sourceVersion == and != ops

  proc ==(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return false;
  }

  proc !=(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return true;
  }

  proc ==(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return false;
  }

  /*
    Equality/inequality comparisons between mixed values of type
    :type:`semanticVersion` and :type:`sourceVersion` always return
    ``false`` for ``==`` and ``true`` for ``!=``.
  */
  proc !=(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return true;
  }


  // ordered comparison operators for semanticVersion and sourceVersion

  proc <(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return v1 < v2.version;
  }

  proc <=(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return v1 < v2.version;
  }

  proc >(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return v1 >= v2.version;
  }

  proc >=(v1: semanticVersion(?), v2: sourceVersion(?)) param : bool {
    return v1 >= v2.version;
  }

  proc <(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 >= v1;
  }

  proc <=(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 > v1;
  }

  proc >(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 <= v1;
  }

  /*
    Ordered comparisons between mixed values of type
    :type:`semanticVersion` and :type:`sourceVersion` are based on the
    ordering of the semantic versions of the two values.  If the
    two values have identical semantic versions, the
    :type:`sourceVersion` value is considered less than the
    :type:`semanticVersion` value.
  */
  proc >=(v1: sourceVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 < v1;
  }
}
