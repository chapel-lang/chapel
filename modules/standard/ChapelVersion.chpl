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

This module contains features that support reasoning about Chapel
version numbers at compile-time.  Chapel version numbers have major,
minor, and update components as in semantic versioning, though for
Chapel, version 2.0.0 corresponds to version 1.0.0 in traditional
semantic versioning.  In addition, pre-release versions of Chapel
(those built from the GitHub source tree) have a SHA component
indicating a specific point in that version's development.

This module features:

* :var:`chplVersion`: the version number of the copy of ``chpl`` used
  to compile the program.

* :proc:`version`: a utility function for creating new version values

* :type:`versionValue`: the record type used to represent version
  values, supporting compile-time comparisons

*/

module ChapelVersion {
  // Query the compiler to get its version number and cache those values
  private param major = __primitive("version major");
  private param minor = __primitive("version minor");
  private param update = __primitive("version update");
  private param SHA = __primitive("version sha");


  /* The version of ``chpl`` used to compile this program. */
  const chplVersion: versionValue(?);
  chplVersion = new versionValue(major, minor, update, SHA);

  
  /*
    A helper function that creates a new version value from its
    arguments.

    :arg major: The major version number
    :type major: `int`

    :arg minor: The minor version number
    :type minor: `int`

    :arg update: The optional update version number (defaults to 0)
    :type major: `int`

    :arg sha: The optional SHA (defaults to "")
    :type major: `int`

    :returns: A new version value
  */
     
  proc version(param major: int, param minor: int,
               param update: int = 0, param sha: string=""): versionValue(?) {
    return new versionValue(major, minor, update, sha);
  }


  /* This record represents a Chapel version value.  It uses ``param``
     fields to represent its components in order to support
     compile-time comparison of version numbers which in turn permits
     code to specialize to specific versions of Chapel.  Values of
     :type:`versionValue` are printed out identically to `chpl
     --version`.
  */
  record versionValue {
    /* The major version number.  For version ``2.0.1``, this would be
       ``2``. */
    param major: int;

    /* The minor version number.  For version ``2.0.1``, this would be
       ``0``. */
    param minor: int;

    /* The update version number.  For version ``2.0.1``, this would
       be ``1``. */
    param update: int;

    /* For pre-releases of Chapel, the git SHA from which the compiler
       was built.  For official releases, this will be the empty
       string. */
    param sha: string;

    /* Indicates whether or not this is a pre-release of Chapel.  This
       is currently implemented as a method, but may become a field
       in future versions of this module. */
    proc preRelease param : bool {
      return sha != "";
    }

    /* Indicates whether or not this is an official release of Chapel.
       This is always the opposite of ``preRelease``. */
    proc isOfficialRelease() param : bool {
      return !preRelease;
    }

    pragma "no doc"
    proc writeThis(s) throws {
      s.write("chpl version ", major, ".", minor, ".", update);
      if preRelease then
        s.write(" pre-release (", sha, ")");
    }
  }

  proc ==(v1: versionValue(?), v2: versionValue(?)) param : bool {
    return (v1.major == v2.major &&
            v1.minor == v2.minor &&
            v1.update == v2.update &&
            v1.sha == v2.sha);
  }

  proc !=(v1: versionValue(?), v2: versionValue(?)) param : bool {
    return (v1.major != v2.major ||
            v1.minor != v2.minor ||
            v1.update != v2.update ||
            v1.sha != v2.sha);
  }

  proc <(v1: versionValue(?), v2: versionValue(?)) param : bool {
    if v1.major != v2.major {
      return v1.major < v2.major;
    } else if v1.minor != v2.minor {
      return v1.minor < v2.minor;
    } else if v1.update != v2.update {
      return v1.update < v2.update;
    } else if v1.sha == v2.sha {
      return false;
    } else if v1.sha == "" {
      return false;
    } else if v2.sha == "" {
      return true;
    } else {
      compilerError("can't compare distinct SHAs");
      return false;
    }
  }


  proc <=(v1: versionValue(?), v2: versionValue(?)) param : bool {
    if v1.major != v2.major {
      return v1.major <= v2.major;
    } else if v1.minor != v2.minor {
      return v1.minor <= v2.minor;
    } else if v1.update != v2.update {
      return v1.update <= v2.update;
    } else if v1.sha == v2.sha {
      return true;
    } else if v1.sha == "" {
      return false;
    } else if v2.sha == "" {
      return true;
    } else {
      compilerError("can't compare distinct SHAs");
      return false;
    }
  }

  proc >(v1: versionValue(?), v2: versionValue(?)) param : bool {
    if v1.major != v2.major {
      return v1.major > v2.major;
    } else if v1.minor != v2.minor {
      return v1.minor > v2.minor;
    } else if v1.update != v2.update {
      return v1.update > v2.update;
    } else if v1.sha == v2.sha {
      return false;
    } else if v1.sha == "" {
      return true;
    } else if v2.sha == "" {
      return false;
    } else {
      compilerError("can't compare distinct SHAs");
      return false;
    }
  }

  /* These comparison operators compare two version values where "less
     than"/"greater than" correspond to a version being older/newer,
     chronologically speaking.  Note that a version number containing
     a SHA:

     - is less than an identical ``major.minor.update`` version
       without a SHA since the presence of a SHA indicates a
       pre-release version

     - can only be compared to another version with a SHA using ``==``
       and ``!=``; we make no attempt to order SHAs.
  */

  proc >=(v1: versionValue(?), v2: versionValue(?)) param : bool {
    if v1.major != v2.major {
      return v1.major >= v2.major;
    } else if v1.minor != v2.minor {
      return v1.minor >= v2.minor;
    } else if v1.update != v2.update {
      return v1.update >= v2.update;
    } else if v1.sha == v2.sha {
      return true;
    } else if v1.sha == "" {
      return true;
    } else if v2.sha == "" {
      return false;
    } else {
      compilerError("can't compare distinct SHAs");
      return false;
    }
  }
}
