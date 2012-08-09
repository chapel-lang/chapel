pragma "no use ChapelStandard"
module NetworkAtomicTypes {

  proc chpl__networkAtomicType(type base_type) type {
    if base_type==bool then return ratomicflag;
    else if base_type==uint(32) then return ratomic_uint32;
    else if base_type==uint(64) then return ratomic_uint64;
    else if base_type==int(32) then return ratomic_int32;
    else if base_type==int(64) then return ratomic_int64;
    else if base_type==real then return ratomic_real64;
    else {
      compilerWarning("Unsupported network atomic type");
      if base_type==uint(8) then return atomic_uint8;
      else if base_type==uint(16) then return atomic_uint16;
      else if base_type==int(8) then return atomic_int8;
      else if base_type==int(16) then return atomic_int16;
      else compilerError("Unsupported atomic type");
    }
  }

}
