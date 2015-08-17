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

  private extern proc chpl_now_time():real;

  //
  // Data Generation for the Visual Debug tool  (offline)
  //

  private extern proc chpl_vdebug_start ( rootname: c_string, time:real);

  private extern proc chpl_vdebug_stop ();

  private extern proc chpl_vdebug_tag ( tagname: c_string);

  private extern proc chpl_vdebug_pause ();

  private extern proc chpl_vdebug_nolog ();


/* Instead of using a "coforall l in Locales" which is an O(n) operation
   at the present time, vis_op, hc_id2com, and VDebugTree implement
   a hyper-cube broadcast tree to execute on all locales.  The vis_op
   is used to select which of the above routine to run at each locale.
   This code is O(log n), n the number of Locales.
*/

pragma "no doc"
  enum vis_op {v_start, v_stop, v_tag, v_pause};

private iter hc_id2com ( id: int, off: int ) {
   var offset = off;
   var ix = 1;
   while (offset > 0) {
      yield (id+offset, ix);
      offset = offset >> 1;
      ix = ix + 1;
   }
}
  
private proc VDebugTree (what: vis_op, name: string, time: real, id: int = 0,
                   n: int = numLocales, off: int = -1) {
      var offset = 1;
      if off < 0 then
         while offset >> 1 + id < n do offset = offset << 1;
      else
         offset = off;

      chpl_vdebug_nolog();
      coforall (rid, shift) in hc_id2com(id, offset) do
         if rid < n then
             on Locales[rid] do VDebugTree (what, name, time, rid, n, offset >> shift);

     /* Do the op at the root  */
     select what {
         when vis_op.v_start    do chpl_vdebug_start (name.c_str(), time);
         when vis_op.v_stop     do chpl_vdebug_stop ();
         when vis_op.v_tag      do chpl_vdebug_tag (name.c_str());
         when vis_op.v_pause    do chpl_vdebug_pause ();
     }
}


/* 
  Start logging events for VisualDebug.  Open a new set of data
  files, one for each locale, for chplvis.  This routine should be
  called only once for each program.  It creates a directory with the
  rootname and creates the files in that directory.  The files are
  named with the rootname and "-n" is added where n is the locale
  number.

  :arg rootname:  Directory name and rootname for files. 
*/
  proc startVdebug ( rootname : string ) {
    var now = chpl_now_time();
    //coforall l in Locales do
    //  on l do chpl_vdebug_start (rootname.c_str(), now);
    VDebugTree (vis_op.v_start, rootname, now);
  }

/*
  Add a tag to the data for chplvis to allow "view points" in the data.

  :arg tagname: name of the tag
*/
  proc tagVdebug ( tagname : string ) {
    //coforall l in Locales[1..] do
    //  on l do chpl_vdebug_tag (tagname.c_str(), 0);
    //chpl_vdebug_tag (tagname.c_str(), 0);
    VDebugTree (vis_op.v_tag, tagname, 0);
  }

/*
  Stop collecting VisualDebug data and close the data files.
*/
  proc stopVdebug () {
    // chpl_vdebug_stop();
    // coforall l in Locales[1..] do
    //  on l do chpl_vdebug_stop();
    VDebugTree (vis_op.v_stop, "", 0);
  }

/*
  Suspend collection of VisualDebug data.
*/
  proc pauseVdebug () {
    //coforall l in Locales[1..] do
    //  on l do chpl_vdebug_tag (tagname.c_str(), 1);
    //chpl_vdebug_tag(tagname.c_str(), 1);
    VDebugTree (vis_op.v_pause, "", 0);
  }

/*
  Resume collection of VisualDebug data for chplvis after a pauseVdebug().
  This also generates a tag record visible by chplvis.

  :arg tagname: name of the tag
*/
  proc resumeVdebug ( tagname : string ) {
    //coforall l in Locales[1..] do
    //  on l do chpl_vdebug_resume ();
    //chpl_vdebug_resume(tagname);
    VDebugTree (vis_op.v_tag, tagname, 0);
  }

}
