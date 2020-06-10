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

This module contains features that support compile-time reasoning
about version numbers in general, and Chapel version numbers
specifically.  In more detail, it features:

* :var:`chplVersion`: the version number of the copy of ``chpl`` used
  to compile the program.

* two types that can be used to represent version numbers:

  * :type:`semanticVersion` which represents a semantic version value

  * :type:`gitVersion` which is a semantic version plus a SHA

* :proc:`version`: a utility function for creating new version values

Version types in this module are defined in terms of `param` values to
support compile-time reasoning about versions and code specialization.

The version types support:

* being printed out or cast to a ``param`` string

* compile-time comparisons between version values via ``==``, ``!=``,
  ``<``, ``<=``, ``>``, and ``>``, including mixed types.  Generally
  speaking, "less than" corresponds to "is an earlier version than."

*/

module ChapelVersion {
  // Query the compiler to get its version number and cache those values
  private param chplMajor = __primitive("version major");
  private param chplMinor = __primitive("version minor");
  private param chplUpdate = __primitive("version update");
  private param chplSHA = __primitive("version sha");


  /*
    The version of ``chpl`` used to compile this program.  For an
    official Chapel release, this will have type ``semanticVersion``
    while for a pre-release, it will have type ``gitVersion``.

    Note that Chapel ``1.x.y``/``2.x.y`` corresponds to version
    ``0.x.y``/``2.x.y`` in traditional semantic versioning.
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

    :arg sha: The optional SHA (defaults to "")
    :type sha: `string`

    :returns: A new version value.  If SHA == "", this routine returns
              a value of :type:`semanticVersion` otherwise it returns
              a value of :type:`gitVersion`.
  */
  proc version(param major: int, param minor: int,
               param update: int = 0, param sha: string = "") {
    if (sha == "") then
      return new semanticVersion(major, minor, update);
    else
      return new gitVersion(major, minor, update, sha);

  }


  /*
    This record represents a semantic version value.  It uses
    ``param`` values to represent its components in order to support
    compile-time comparison of version numbers which in turn permits
    code to specialize to specific versions of Chapel.  When printed
    or converted to a string, it is represented as ``version x.y.z``.
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


  /*
    This record represents a software version in a Git repository.  It
    uses ``param`` values to represent its components in order to
    support compile-time comparison of version numbers which in turn
    permits code to specialize to specific versions of Chapel.  When
    printed or converted to a string, it is represented as ``version
    x.y.z (SHA)``.

    Note that comparisons between two gitVersion values with identical
    semantic versions are not supported due to the challenges involved
    in ordering SHA values.  In addition, when a semanticVersion is
    compared with a gitVersion with the identical semantic version,
    the gitVersion value is considered "less than" the semanticVersion
    using the model that it represents a pre-release of the official
    release represented by the semantic version.
  */
  record gitVersion {
    forwarding version only major, minor, update;

    // TODO: Why does the following type declaration break compilation?
    //    const version /*: semanticVersion(?,?,?) */;

    /* The semantic version portion of the version number. */
    const version;

    /* The git SHA of the version */
    param sha: string;

    pragma "no doc"
    proc init(param major: int, param minor: int, param update: int,
              param sha: string) {
      this.version = new semanticVersion(major, minor, update);
      this.sha = sha;
      if sha == "" then
        compilerError("gitVersion values must have a SHA");
    }

    pragma "no doc"
    proc writeThis(s) throws {
      s.write(this:string);
    }
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

  proc !=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) != 0;
  }

  proc <(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) == -1;
  }

  proc <=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) != 1;
  }

  proc >(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) == 1;
  }

  proc >=(v1: semanticVersion(?), v2: semanticVersion(?)) param : bool {
    return spaceship(v1,v2) != -1;
  }


  // Comparisons between gitVersions

  proc ==(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    return (v1.version == v2.version &&
            v1.sha == v2.sha);
  }

  proc !=(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    return (v1.version != v2.version ||
            v1.sha != v2.sha);
  }

  proc <(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp == -1;
    } else if v1.sha == v2.sha {
      return false;
    } else {
      compilerError("can't compare gitVersions that only differ by SHA");
      return false;
    }
  }


  proc <=(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp == -1;
    } else if v1.sha == v2.sha {
      return true;
    } else {
      compilerError("can't compare gitVersions that only differ by SHA");
      return false;
    }
  }

  proc >(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp == 1;
    } else if v1.sha == v2.sha {
      return false;
    } else {
      compilerError("can't compare gitVersions that only differ by SHA");
      return false;
    }
  }

  proc >=(v1: gitVersion(?), v2: gitVersion(?)) param : bool {
    param versionComp = spaceship(v1.version, v2.version);
    if versionComp != 0 {
      return versionComp == 1;
    } else if v1.sha == v2.sha {
      return true;
    } else {
      compilerError("can't compare gitVersions that only differ by SHA");
      return false;
    }
  }

  // semanticVersion / gitVersion comparison ops

  proc ==(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return false;
  }

  proc !=(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return true;
  }

  proc <(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return v1 < v2.version;
  }

  proc <=(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return v1 < v2.version;
  }

  proc >(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return v1 >= v2.version;
  }

  proc >=(v1: semanticVersion(?), v2: gitVersion(?)) param : bool {
    return v1 >= v2.version;
  }


  // gitVersion / semanticVersion comparison ops

  proc ==(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 == v1;
  }

  proc !=(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 != v1;
  }

  proc <(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 >= v1;
  }

  proc <=(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 > v1;
  }

  proc >(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 <= v1;
  }

  proc >=(v1: gitVersion(?), v2: semanticVersion(?)) param : bool {
    return v2 < v1;
  }

  pragma "no doc"
  proc _cast(type t: string, x: semanticVersion(?)) param : string {
    return "version " + x.major:string + "." + x.minor:string + "." + x.update:string;
  }

  pragma "no doc"
  proc _cast(type t: string, x: gitVersion(?)) {
    return x.version:string + " (" + x.sha + ")";
  }
}
