
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
The following document shows functions and methods used to
manipulate and process Chapel bytes objects. The :record:`bytes` object is
similar to astringbut allows arbitrary data to be stored in it. Methods onbytes`
that interpret the data as characters assume that the bytes are ASCII characters

Casts from bytes to a Numeric Type
-----------------------------------

This module supports casts from :record:`bytes` to numeric types. Such casts
will interpret the bytes as ASCII characters and convert it to the numeric type
and throw an error if the bytes is invalid. For example:

.. code-block:: chapel

  var b = bytes("a");
  var number = b:int;

throws an error when it is executed, but

.. code-block:: chapel

  var b = bytes("1");
  var number = b:int;

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

  /*
     ``DecodePolicy`` specifies what happens when there is malformed characters
     when decoding the :record:`bytes` object into a UTF-8 `string`.
       
       - **Strict**: default policy; raise error
       - **Replace**: replace with UTF-8 replacement character
       - **Ignore**: silently drop data
  */
  enum DecodePolicy { Strict, Replace, Ignore }
  use DecodePolicy;

  record bytes {  // _bytes when/if there is compiler support
    pragma "no doc"
    var len: int = 0;
    pragma "no doc"
    var buff: bufferType = nil;

    // We use chpl_nodeID as a shortcut to get at here.id without actually
    // constructing a locale object. Used when determining if we should make a
    // remote transfer.
    var locale_id = chpl_nodeID; // : chpl_nodeID_t

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
      Initialize a new :record:`bytes` from the `c_string` `cs`. If `isowned` is
      set to true, the backing buffer will be freed when the new record is
      destroyed.  If `needToCopy` is set to true, the `c_string` will be copied
      into the record, otherwise it will be used directly. It is the
      responsibility of the user to ensure that the underlying buffer is not
      freed if the `c_string` is not copied in.
     */

    proc init(cs: c_string, length: int = cs.length,
                isowned: bool = true, needToCopy:  bool = true) {

    }

    /*
      Initialize a new :record:`bytes` from `buff` ( `c_ptr` ). `size` indicates
      the total size of the buffer available, while `len` indicates the current
      length of the string in the buffer (the common case would be `size-1` for
      a C-style string). If `isowned` is set to true, the backing buffer will be
      freed when the new record is destroyed. If `needToCopy` is set to true,
      the `c_string` will be copied into the record, otherwise it will be used
      directly. It is the responsibility of the user to ensure that the
      underlying buffer is not freed if the `c_string` is not copied in.
     */
    // This initializer can cause a leak if isowned = false and needToCopy = true
    proc init(buff: c_ptr, length: int, size: int,
                isowned: bool = true, needToCopy: bool = true) {

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
      ``1..bytes.length`` when compiled with `--checks`. `--fast` disables this
      check.

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
      the :record:`bytes` objects passed in with the contents of the method
      receiver inserted between them.
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
      
      :arg errors: `DecodePolicy.Strict` raises an error, `DecodePolicy.Replace`
                   replaces the malformed character with UTF-8 replacement
                   character, `DecodePolicy.Ignore` drops the data silently.

      :returns: A UTF-8 string.
    */
    // NOTE: In the future this could support more encodings.
    proc decode(errors: DecodePolicy = Strict): string {

    }

    /*
     Checks if all the characters in the object are uppercase (A-Z) in ASCII.

      :returns: * `true`  -- if the object contains at least one uppercase
                             character and no lowercase characters, ignoring
                             uncased (not a letter) and extended ASCII
                             characters (decimal value is larger than 127)
                            
                * `false` -- otherwise
     */
    proc isUpper() : bool {

    }

    /*
     Checks if all the characters in the object are lowercase (a-z) in ASCII.

      :returns: * `true`  -- if the object contains at least one lowercase
                             character and no upper characters, ignoring
                             uncased (not a letter) and extended ASCII
                             characters (decimal value is larger than 127)
                            
                * `false` -- otherwise
     */
    proc isLower() : bool {

    }

    /*
     Checks if all the characters in the object are whitespace (' ', '\t',
     '\n', '\v', '\f', '\r') in ASCII.

      :returns: * `true`  -- when all the characters are whitespace.
                * `false` -- otherwise
     */
    proc isSpace() : bool {

    }

    /*
     Checks if all the characters in the object are alphabetic (a-zA-Z) in
     ASCII.

      :returns: * `true`  -- when the characters are alphabetic.
                * `false` -- otherwise
     */
    proc isAlpha() : bool {

    }

    /*
     Checks if all the characters in the object are digits (0-9) in ASCII.

      :returns: * `true`  -- when the characters are digits.
                * `false` -- otherwise
     */
    proc isDigit() : bool {

    }

    /*
     Checks if all the characters in the object are alphanumeric (a-zA-Z0-9) in
     ASCII.

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
      :returns: A new :record:`bytes` with all uppercase characters (A-Z)
                replaced with their lowercase counterpart in ASCII. Other
                characters remain untouched.
    */
    proc toLower() : bytes {

    }

    /*
      :returns: A new bytes with all lowercase characters (a-z) replaced with
                their uppercase counterpart in ASCII. Other characters remain
                untouched.
    */
    proc toUpper() : bytes {

    }

    /*
      :returns: A new bytes with all cased characters(a-zA-Z) following an
                uncased character converted to uppercase, and all cased
                characters following another cased character converted to
                lowercase.
     */
    proc toTitle() : bytes {

    }

  } // end of record bytes

  /*
     Appends the bytes `rhs` to the bytes `lhs`.
  */
  proc +=(ref lhs: bytes, const ref rhs: bytes) : void {

  }

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
