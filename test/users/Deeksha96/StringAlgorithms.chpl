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

module StringAlgorithms{
proc minLexographicRotation(s: string): string{
  var len = s.size;
  var minstr = s;
  var S = s + s;
  for i in 1..len
  {
    var str: string;
    str = S.this(i..i+len-1);
    if(str < minstr)
    {
      minstr = str;
    }
  }
  return minstr;
}
proc maxLexographicRotation(s: string): string{
  var len = s.size;
  var maxstr = s;
  var S = s + s;
  for i in 1..len
  {
    var str:string;
    str = S.this(i..i+len-1);
    if(str > maxstr)
    {
      maxstr = str;
    }
  }
  return maxstr;
}
}
