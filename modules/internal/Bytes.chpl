
/*
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
The following documentation shows functions and methods used to
manipulate and process Chapel bytes objects.

Casts from bytes to a Numeric Type
-----------------------------------

This module supports casts from :record:`bytes` to numeric types. Such casts
will convert the bytes to the numeric type and throw an error if the bytes
is invalid. For example:

.. code-block:: chapel

  var number = "a":int;

throws an error when it is executed, but

.. code-block:: chapel

  var number = "1":int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the
:ref:`Error Handling technical note <readme-errorHandling>`.
 */
module Bytes {
  use ChapelStandard;

  // to follow String.chpl, and maybe limit the index to smaller integers
  type byteIndex = int; 

  type byteType = uint(8);
  type bufferType = c_ptr(byteType);

  record bytes {  // _bytes when/if there is compiler support
    pragma "no doc"
    var len: int = 0;
    pragma "no doc"
    var buff: bufferType = nil;

    pragma "no doc"
    proc init() {

    }

    /*
      Initialize a new :record:`bytes` from ``b``. If ``isowned`` is set to
      ``true`` then ``b`` will be fully copied into the new instance. If it is
      ``false`` a shallow copy will be made such that any in-place modifications
      to the new bytes may appear in ``b``. It is the responsibility of the user
      to ensure that the underlying buffer is not freed while being used as part
      of a shallow copy.
     */
    proc init(b: bytes, isowned: bool = true) {

    }

    pragma "no doc"
    proc init=(b: bytes) {
      this.init(b);
    }

    /*
      :returns: The number of bytes in the object.
    */
    inline proc length return len;

    /*
      :returns: The number of bytes in the object.
      */
    inline proc size return len;

    /*
       Gets a version of the :record:`bytes` that is on the currently
       executing locale.

       :returns: A shallow copy if the :record:`bytes` is already on the
                 current locale, otherwise a deep copy is performed.
    */
    inline proc localize() : bytes {
      return this;
    }

    /*
      Get a `c_string` from a :record:`bytes`.
     */
    inline proc c_str(): c_string {
      return new c_string();
    }

    /*
      Iterates over the bytes

      :yields: 1-length :record:`bytes` objects
     */
    iter these(): bytes {
      yield 0;
    }

    /*
      Gets a byte from the object

      :arg i: The index

      :returns: 1-length :record:`bytes` object
     */
    proc this(i: int): bytes {

    }

    /*
      Slices the object. Halts if r is not completely inside the range
      ``1..bytes.length``.

      :arg r: range of the indices the new bytes should be made from

      :returns: a new bytes that is a slice within ``1..bytes.length``. If
                the length of `r` is zero, an empty bytes is returned.
     */
    proc this(r: range(?)) : bytes {

    }

    /*
      :returns: * `true`  -- when the object is empty
                * `false` -- otherwise
     */
    inline proc isEmpty() : bool {
      return this.len == 0;
    }

    /*
      :arg needles: A varargs list of :record:`bytes` objects to match against.

      :returns: * `true`  -- when the object begins with one or more of the `needles`
                * `false` -- otherwise
     */
    proc startsWith(needles: bytes ...) : bool {
      return false;
    }

    /*
      :arg needles: A varargs list of :record:`bytes` objects to match against.

      :returns: * `true`  -- when the object ends with one or more of the `needles`
                * `false` -- otherwise
     */
    proc endsWith(needles: bytes ...) : bool {

    }

    /*
      :arg needle: the :record:`bytes` object to search for
      :arg region: an optional range defining the indices to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the index of the first occurrence of `needle` within a
                the object, or 0 if the `needle` is not in the object.
     */
    proc find(needle: bytes, region: range(?) = 1:byteIndex..) : byteIndex {

    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg region: an optional range defining the indices to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the index of the first occurrence from the right of `needle`
                within the object, or 0 if the `needle` is not in the object.
     */
    proc rfind(needle: bytes, region: range(?) = 1:byteIndex..) : byteIndex {

    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg region: an optional range defining the substring to search within,
                   default is the whole object. Halts if the range is not
                   within ``1..bytes.length``

      :returns: the number of times `needle` occurs in the object
     */
    proc count(needle: bytes, region: range(?) = 1..) : int {

    }

    /*
      :arg needle: the :record:`bytes` to search for
      :arg replacement: the :record:`bytes` to replace `needle` with
      :arg count: an optional integer specifying the number of replacements to
                  make, values less than zero will replace all occurrences

      :returns: a copy of the object where `replacement` replaces `needle` up
                to `count` times
     */
    proc replace(needle: bytes, replacement: bytes, count: int = -1) : bytes {

    }

    /*
      Splits the :record:`bytes` on `sep` yielding the subbytes between each
      occurrence, up to `maxsplit` times.

      :arg sep: The delimiter used to break the object into chunks.
      :arg maxsplit: The number of times to split the object, negative values
                     indicate no limit.
      :arg ignoreEmpty: * When `true`  -- Empty :record:`bytes` will not be
                                          yielded, and will not count towards
                                          `maxsplit`
                        * When `false` -- Empty :record:`bytes` will be yielded
                                          when `sep` occurs multiple times in a
                                          row.
     */
    iter split(sep: bytes, maxsplit: int = -1,
               ignoreEmpty: bool = false): bytes {

    }

    /*
      Works as above, but uses runs of whitespace as the delimiter.

      :arg maxsplit: The number of times to split the bytes, negative values
                     indicate no limit.
     */
    iter split(maxsplit: int = -1) : bytes {

    }

    /*
      Returns a new :record:`bytes` object, which is the concatenation of all of
      the :record:`bytes` objects passed in with the receiving bytes inserted
      between them.
    */
    proc join(const ref S: bytes ...) : bytes {

    }

    /*
      Same as the varargs version, but with a homogeneous tuple of bytes.
    */
    proc join(const ref S) : bytes where isTuple(S) {

    }

    pragma "no doc"
    proc join(ir: _iteratorRecord) {

    }

    /*
      :arg chars: A :record:`bytes` containing each character to remove.
                  Defaults to `" \\t\\r\\n"`.
      :arg leading: Indicates if leading occurrences should be removed.
                    Defaults to `true`.
      :arg trailing: Indicates if trailing occurrences should be removed.
                     Defaults to `true`.

      :returns: A new :record:`bytes` with `leading` and/or `trailing`
                occurrences of characters in `chars` removed as appropriate.
    */
    proc strip(chars: bytes = " \t\r\n", leading=true, trailing=true) : bytes {

    }

    /*
      Splits the object on `sep` into a `3*bytes` consisting of the section
      before `sep`, `sep`, and the section after `sep`. If `sep` is not found,
      the tuple will contain the whole bytes, and then two empty bytes.
    */
    proc const partition(sep: bytes) : 3*bytes {

    }

    /*
      Returns a UTF-8 string from the given :record:`bytes` object. If the data
      is malformed for UTF-8, `errors` argument determines the action.
      
      :arg errors: `strict` raises an error, `replace` replaces the malformed
                   data, `ignore` drops the data silently.

      :returns: A UTF-8 string.
    */
    proc decode(errors: string): string {

    }

    /*
     Checks if all the characters in the object are either uppercase (A-Z) or
     uncased (not a letter).

      :returns: * `true`  -- if the object contains at least one uppercase
                             character and no lowercase characters, ignoring
                             uncased characters.
                * `false` -- otherwise
     */
    proc isUpper() : bool {

    }

    /*
     Checks if all the characters in the object are either lowercase (a-z) or
     uncased (not a letter).

      :returns: * `true`  -- when there are no uppercase characters in the
                             object
                * `false` -- otherwise
     */
    proc isLower() : bool {

    }

    /*
     Checks if all the characters in the object are whitespace (' ', '\t',
     '\n', '\v', '\f', '\r').

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc isSpace() : bool {

    }

    /*
     Checks if all the characters in the object are alphabetic (a-zA-Z).

      :returns: * `true`  -- when the characters are alphabetic.
                * `false` -- otherwise
     */
    proc isAlpha() : bool {

    }

    /*
     Checks if all the characters in the object are digits (0-9).

      :returns: * `true`  -- when the characters are digits.
                * `false` -- otherwise
     */
    proc isDigit() : bool {

    }

    /*
     Checks if all the characters in the object are alphanumeric (a-zA-Z0-9).

      :returns: * `true`  -- when the characters are alphanumeric.
                * `false` -- otherwise
     */
    proc isAlnum() : bool {

    }

    /*
     Checks if all the characters in the object are printable.

      :returns: * `true`  -- when the characters are printable.
                * `false` -- otherwise
     */
    proc isPrintable() : bool {

    }

    /*
      Checks if all uppercase characters are preceded by uncased characters,
      and if all lowercase characters are preceded by cased characters.

      :returns: * `true`  -- when the condition described above is met.
                * `false` -- otherwise
     */
    proc isTitle() : bool {

    }

    /*
      :returns: A new :record:`bytes` with all uppercase characters replaced
                with their lowercase counterpart.
    */
    proc toLower() : bytes {

    }

    /*
      :returns: A new bytes with all lowercase characters replaced with their
                uppercase counterpart.
    */
    proc toUpper() : bytes {

    }

    /*
      :returns: A new bytes with all cased characters following an uncased
                character converted to uppercase, and all cased characters
                following another cased character converted to lowercase.
     */
    proc toTitle() : bytes {

    }

  } // end of record bytes

  /*
     Copies the bytes `rhs` into the bytes `lhs`.
  */
  proc =(ref lhs: bytes, rhs: bytes) {

  }

  /*
     Copies the c_string `rhs_c` into the bytes `lhs`.

     Halts if `lhs` is a remote bytes.
  */
  proc =(ref lhs: bytes, rhs_c: c_string) {

  }

  //
  // Concatenation
  //
  /*
     :returns: A new :record:`bytes` which is the result of concatenating `s0`
               and `s1`
  */
  proc +(s0: bytes, s1: bytes) {
  
  }

  /*
     :returns: A new :record:`bytes` which is the result of repeating `s` `n`
               times.  If `n` is less than or equal to 0, an empty bytes is
               returned.
  */
  proc *(s: bytes, n: integral) {

  }

  /*
     The following concatenation functions return a new :record:`bytes` which is
     the result of casting the non-bytes argument to a bytes, and concatenating
     that result with `s`.
  */
  inline proc +(s: bytes, x: numeric) {}
  inline proc +(x: numeric, s: bytes) {}
  inline proc +(s: bytes, x: enumerated) {}
  inline proc +(x: enumerated, s: bytes) {}
  inline proc +(s: bytes, x: bool) {}
  inline proc +(x: bool, s: bytes) {}

} // end of module Bytes
