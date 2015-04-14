/*
 * Copyright 2015 Cray Inc.
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

// VisualDebug.chpl
//
// pragma "no use ChapelStandard"

module VisualDebug
{

  use String;

  // get access to this quickly?
  extern proc chpl_now_time():real;

  //
  // Data Generation for the Visual Debug tool  (offline)
  //

  extern proc chpl_vdebug_start ( rootname: c_string, time:real);
  extern proc chpl_vdebug_stop ();
  extern proc chpl_vdebug_tag ( tagname: c_string );

  proc startVdebug ( rootname : string ) {
    var now = chpl_now_time();
    coforall l in Locales do
      on l do chpl_vdebug_start (rootname.c_str(), now);
  }

  proc tagVdebug ( tagname : string ) {
    coforall l in Locales do
      on l do chpl_vdebug_tag (tagname.c_str());
  }

  proc stopVdebug () {
    coforall l in Locales do
      on l do chpl_vdebug_stop();
  }

}
