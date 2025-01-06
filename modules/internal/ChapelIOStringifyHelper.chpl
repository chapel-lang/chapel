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

/* Pull out the portions of stringify that don't rely on IO so that they can
   be called without IO and not cause circular dependencies.
*/
module ChapelIOStringifyHelper {
  private use ChapelStandard, CTypes;
  private use BytesStringCommon only decodePolicy;

  @chpldoc.nodoc
  proc _can_stringify_direct(t) param : bool {
    if (t.type == string ||
        t.type == bytes ||
        t.type == chpl_c_string ||
        t.type == c_ptrConst(c_char) ||
        isRangeType(t.type) ||
        isPrimitiveType(t.type)) {
      return true;
    } else if (isTupleType(t.type)) {
      for param i in 0..t.size-1 {
        if !_can_stringify_direct(t[i]) then
          return false;
      }
      return true;
    } else {
      return false;
    }
  }

  private proc _stringify_tuple(tup: _tuple) {
    var str = "(";

    for param i in 0..tup.size-1 {
      if (tup[i].type == chpl_c_string || tup[i].type == c_ptrConst(c_char)) {
        try! {
          str += string.createCopyingBuffer(tup[i]);
        }
      }
      else {
        str += tup[i]:string;
      }
      if tup.size == 1 then str += ",";
      else if i < tup.size-1 then str += ", ";
    }

    str += ")";

    return str;
  }

  @chpldoc.nodoc
    proc stringify_simple(const args ...?k): string {
    // As an optimization, use string concatenation for
    // all primitive type stringify...
    // This helps to work around some resolution errors
    // when internal modules use halt, which calls stringify.

    var str = "";

    for param i in 0..k-1 {
      if (args[i].type == string) {
        str += args[i];
      } else if (args[i].type == chpl_c_string || args[i].type == c_ptrConst(c_char)) {
        //decodePolicy.replace never throws
        try! {
          str += string.createCopyingBuffer(args[i],
                                           policy=decodePolicy.replace);
        }
      } else if (args[i].type == bytes) {
        //decodePolicy.replace never throws
        try! {
          str += args[i].decode(decodePolicy.replace);
        }
      } else if (isRangeType(args[i].type) ||
                 isPrimitiveType(args[i].type)) {
        str += args[i]:string;
      } else if (isTupleType(args[i].type)) {
        str += _stringify_tuple(args[i]);
      }
    }

    return str;
  }
}
