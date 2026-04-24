/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

module ChapelProgramRegistration {
  use ChapelBase, CTypes;

  // This is a header defined in the runtime.
  require "chpl-prginfo.h";

  // The type of a program ID - this is defined in the runtime header.
  extern type chpl_rt_prg_id = uint(64);

  // Contains information that the runtime needs from a Chapel program.
  // Defined in 'runtime/include/chpl-prginfo.h'. Intentionally opaque.
  extern type chpl_rt_prginfo;

  // We maintain a program info for each locale.
  record chpl_programInfo {
    var _info: chpl_rt_prginfo;
    var _isPrepared: bool;

    inline proc type nullId param: uint do return 0;
    inline proc type rootId param: uint do return 1;
    inline proc nullId param do return this.type.nullId;
    inline proc rootId param do return this.type.rootId;

    inline proc ref asPtr() do return c_ptrTo(_info);

    /** Get the unique ID of this Chapel program. */
    inline proc id: chpl_rt_prg_id {
      param cname = 'chpl_rt_prginfo_id';
      extern cname proc id(const ref chpl_rt_prginfo): chpl_rt_prg_id;
      return id(_info);
    }

    /** Get the load path of this Chapel program. */
    inline proc path: string {
      param cname = 'chpl_rt_prginfo_load_path';
      extern cname proc fn(const ref chpl_rt_prginfo): c_ptrConst(c_char);
      const ptr = fn(_info);
      const ret = try! string.createBorrowingBuffer(ptr);
      return ret;
    }

    inline proc ref _callSetter(param name: string, in val) {
      // Use 'const ref val' to avoid any copies at this time.
      param cname = 'chpl_rt_prginfo_data_entry_set_' + name;

      // Setter is in the runtime and knows the number of bytes to copy.
      extern cname proc setter(ref info: chpl_rt_prginfo,
                               data: c_ptrConst(void)): void;
      const ptrToData = c_addrOfConst(val);

      // Invoke the appropriate setter in the module code.
      setter(info=this._info, data=ptrToData);
    }

    inline proc ref setCallback(param name: string) {
      // NOTE: The type need not match up - we just need the local address...
      extern name proc callback(): void;

      // NOTE: This absolutely, should not, under ANY circumstances, create a
      //       Chapel procedure pointer from 'callback'! The only thing that
      //       should be happening here is to cast the local address of the
      //       'callback' to a 'void*'! (Otherwise, everything explodes...)
      const ptr1: c_fn_ptr = __primitive("capture fn", callback, true);
      const ptr2 = ptr1 : c_ptr(void);

      _callSetter(name, ptr2);
    }

    inline proc ref setConstant(param name: string, in val) {
      // TODO: If we could just be able to write something like this...
      /*
      // This type is defined in 'chpl-program-registration.h'.
      param type_c_name = name + '_type';
      extern type_c_name type dataType;
      // This value ought to exist and have external linkage.
      // TODO: We can also just use a bogus type...
      extern name const data: dataType;
      _callSetter(name, data);
      */

      _callSetter(name, val);
    }

    inline proc infoFromIdHere(prg: chpl_rt_prg_id) {
      param cname = 'chpl_rt_prginfo_from_id_here';
      extern cname proc get(prg: chpl_rt_prg_id): c_ptr(chpl_rt_prginfo);
      return get(prg);
    }

    inline proc ref prepare(): void {
      if !_isPrepared {
        setProgramInfoDataFieldsHere(this);
        _isPrepared = true;
      }
    }

    // TODO: Just pass the root program to 'chpl_rt_init'.
    inline proc ref registerAsRoot(): bool {
      param cname = 'chpl_rt_prginfo_register_root_here';
      extern cname proc register(ref info: chpl_rt_prginfo): c_int;

      prepare();
      const ret = register(_info) : bool;

      return ret;
    }
  }

  pragma "no init"          /** Initialized manually at program startup.  */
  pragma "locale private"   /** One per locale, per program.              */
  var chpl_programInfoHere: chpl_programInfo;

  // TODO: This function will bind symbols to fields in the struct. Currently
  //       it is almost empty, but it will be populated later. Set some fields
  //       just to leave an example of what it looks like.
  //
  // TODO: In the future we can generate a module containing this function by
  //       using a script, similiar to what we do for 'SysCTypes.chpl'.
  proc setProgramInfoDataFieldsHere(ref info: chpl_programInfo) {
    extern type chpl_rt_mainHasArgs_type;
    extern const mainHasArgs: chpl_rt_mainHasArgs_type;
    info.setConstant('mainHasArgs', mainHasArgs);
    extern type chpl_rt_mainPreserveDelimiter_type;
    extern const mainPreserveDelimiter: chpl_rt_mainPreserveDelimiter_type;
    info.setConstant('mainPreserveDelimiter', mainPreserveDelimiter);
    info.setCallback('chpl_program_about');
    // More calls to follow...one for every entry in the X-macro.
  }

  export proc chpl_prepareProgramInfoHere(): c_ptr(chpl_rt_prginfo) {
    ref prg = chpl_programInfoHere;
    prg.prepare();
    return prg.asPtr();
  }
}
