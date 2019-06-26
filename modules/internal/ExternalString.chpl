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

module ExternalString {
  
  proc chpl__exportConvertToConstChar(in s: string): c_string {
    return convertToConstCharNoCopy(s);
  }

  proc chpl__exportConvertToConstCharNoCopy(const ref s: string): c_string {
    var result = s.c_str();
    s.isowned = false;
    return result;
  }

  proc chpl__exportConvertToString(c: c_string): string {
    return new string(cs=c, isowned=true, needToCopy=true);
  }

  //
  // Strings allocated in this fashion assume that the memory for the passed
  // buffer was allocated on the Chapel heap.
  //
  proc chpl__exportConvertToStringNoCopy(c: c_string): string {
    return new string(c, isowned=true, needToCopy=false);
  }

} // End module "ExternalString".

