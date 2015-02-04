/*
 * Copyright 2004-2015 Cray Inc.
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

proc LinearSearch(Data:[?Dom], val) {
  for i in Dom {
    if (Data(i) == val) {
      return (true, i);
    } else if (Data(i) > val) {
      return (false, i);
    }
  }
  return (false,Dom.high+1);
}


// would really like to drop the lo/hi arguments here, but right now
// that causes too big of a memory leak
proc BinarySearch(Data:[?Dom], val, in lo = Dom.low, in hi = Dom.high) {
  while (lo <= hi) {
    const mid = (hi - lo)/2 + lo;
    if (Data(mid) == val) {
      return (true, mid);
    } else if (val > Data(mid)) {
      lo = mid+1;
    } else {
      hi = mid-1;
    }
  }
  return (false, lo);
}
