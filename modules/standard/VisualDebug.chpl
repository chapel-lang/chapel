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

/*

    Support for the visualization tool "chplvis"

    This module provides access to and enables hooks to dump out
    task and communication information for post-run visualization
    of the tasks and communication.

 */
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
  extern proc chpl_vdebug_tag ( tagname: c_string, pause:int);
  extern proc chpl_vdebug_starttag();
  extern proc chpl_vdebug_resume ();

/*
  Open a new set of data files, one for each locale, for chplvis.
  :arg rootname:  Rootname for file. "-n" is added where n is locale number.
*/
  proc startVdebug ( rootname : string ) {
    var now = chpl_now_time();
    coforall l in Locales do
      on l do chpl_vdebug_start (rootname.c_str(), now);
  }

/*
  Add a tag to the data for chplvis to allow "view points" in the data.
  :arg tagname: name of the tag
*/
  proc tagVdebug ( tagname : string ) {
    chpl_vdebug_starttag();
    coforall l in Locales[1..] do
      on l do chpl_vdebug_tag (tagname.c_str(), 0);
    chpl_vdebug_tag (tagname.c_str(), 0);
  }

/*
  Stop collecting data of chplvis and close the data files.  
*/
  proc stopVdebug () {
    chpl_vdebug_stop();
    coforall l in Locales[1..] do
      on l do chpl_vdebug_stop();
  }

/*
  Stop collection of data for chplvis and tag the pause point.
  :arg tagname: name of the tag
*/
  proc pauseVdebug ( tagname : string ) {
    chpl_vdebug_starttag();
    coforall l in Locales[1..] do
      on l do chpl_vdebug_tag (tagname.c_str(), 1);
    chpl_vdebug_tag(tagname.c_str(), 1);
  }

/*
  Resume collection of data for chplvis after a pauseVdebug().
  This is a "no-op" if pauseVdebug was not called before.
*/
  proc resumeVdebug () {
    coforall l in Locales[1..] do
      on l do chpl_vdebug_resume ();
    chpl_vdebug_resume();
  }

}
