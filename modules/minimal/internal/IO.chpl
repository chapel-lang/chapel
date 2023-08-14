/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

module IO {

  extern "syserr" type errorCode;
  extern type qio_channel_ptr_t;
  private extern proc qio_int_to_err(a:int(32)):errorCode;
  // Using raw_c_void_ptr as we can't access c_ptr(void) in minimal modules.
  // TODO: Use c_ptr(void) instead, likely via compiler support for c_ptr, or
  // by making it accessible in minimal modules if we don't do compiler support.
  extern type raw_c_void_ptr = chpl__c_void_ptr;

  export proc chpl_qio_setup_plugin_channel(file:raw_c_void_ptr, ref plugin_ch:raw_c_void_ptr, start:int(64), end:int(64), qio_ch:qio_channel_ptr_t):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_read_atleast(ch_plugin:raw_c_void_ptr, amt:int(64)) {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_write(ch_plugin:raw_c_void_ptr, amt:int(64)) {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_channel_close(ch:raw_c_void_ptr):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_filelength(file:raw_c_void_ptr, ref length:int(64)):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_fsync(file:raw_c_void_ptr):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_get_chunk(file:raw_c_void_ptr, ref length:int(64)):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_get_locales_for_region(file:raw_c_void_ptr, start:int(64), end:int(64), ref localeNames:raw_c_void_ptr, ref nLocales:int(64)):errorCode {
    return qio_int_to_err(0);
  }
  export proc chpl_qio_file_close(file:raw_c_void_ptr):errorCode {
    return qio_int_to_err(0);
  }
}
