/*
 * Copyright 2004-2020 Cray Inc.
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

   Support for the visualization tool :ref:`chplvis`.

   This module provides access to and enables hooks to dump out
   task and communication information for post-run visualization
   of the tasks and communication.

*/
module VisualDebug
{

  use String;

  /*
    This is the default setting for :var:`VisualDebugOn`.  It defaults
    to `true`, but can be changed at compile time.
  */
  config param DefaultVisualDebugOn = true;

  /*
    If this is `true`, data collection to support :ref:`chplvis` reporting is
    turned on.
    It defaults to the value of :param:`DefaultVisualDebugOn`, but can
    be changed at execution time.
  */
  config const VisualDebugOn = DefaultVisualDebugOn;

  private extern proc chpl_now_time():real;

  //
  // Data Generation for the Visual Debug tool  (offline)
  //

  private extern proc chpl_vdebug_start (rootname: c_string, time:real);

  private extern proc chpl_vdebug_stop ();

  private extern proc chpl_vdebug_tag (tagno: int);

  private extern proc chpl_vdebug_pause (tagno: int);

  private extern proc chpl_vdebug_mark ();

  private extern proc chpl_vdebug_tagname (tagname: c_string, tagno: int);

  private var tagno: atomic int;


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

private proc VDebugTree (what: vis_op, name: string, time: real, tagno: int,
                id: int = 0, n: int = numLocales, off: int = -1) {
      var offset = 1;
      if off < 0 then
         while offset >> 1 + id < n do offset = offset << 1;
      else
         offset = off;

      chpl_vdebug_mark();
      coforall (rid, shift) in hc_id2com(id, offset) do
         if rid < n then
             on Locales[rid] do VDebugTree (what, name, time, tagno, rid, n, offset >> shift);

     /* Do the op at the root  */
     select what {
         when vis_op.v_start    do chpl_vdebug_start (name.localize().c_str(), time);
         when vis_op.v_stop     do chpl_vdebug_stop ();
         when vis_op.v_tag      do chpl_vdebug_tag (tagno);
         when vis_op.v_pause    do chpl_vdebug_pause (tagno);
     }
}


/*
  Start logging events for VisualDebug.  Open a new set of data
  files, one for each locale, for :ref:`chplvis`.  This routine should be
  called only once for each program.  It creates a directory with the
  rootname and creates the files in that directory.  The files are
  named with the rootname and "-n" is added where n is the locale
  number.

  :arg rootname:  Directory name and rootname for files.
*/
  proc startVdebug ( rootname : string ) {
    var now = chpl_now_time();
    if (VisualDebugOn) {
      tagno.write(0);
      VDebugTree (vis_op.v_start, rootname, now, 0);
    }
  }

/*
  Add a tag to the data for :ref:`chplvis` to allow "view points" in the data.

  :arg tagname: name of the tag
*/
  proc tagVdebug ( tagname : string ) {
    if (VisualDebugOn) {
       var ttag = tagno.fetchAdd(1);
       chpl_vdebug_tagname (tagname.c_str(), ttag);
       VDebugTree (vis_op.v_tag, "", 0, ttag);
    }
  }

/*
  Stop collecting VisualDebug data and close the data files.
*/
  proc stopVdebug () {
    if (VisualDebugOn) {
       VDebugTree (vis_op.v_stop, "", 0, 0);
    }
  }

/*
  Suspend collection of VisualDebug data.
*/
  proc pauseVdebug () {
    if (VisualDebugOn) {
       VDebugTree (vis_op.v_pause, "", 0, tagno.read()-1);
    }
  }

}
