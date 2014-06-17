size_t __EndCount_offsets[] = { sizeof(__EndCount), 0 };
size_t _locale_offsets[] = { sizeof(_locale), 0 };
size_t _file_offsets[] = { sizeof(_file), 0 };
size_t _StringClass_offsets[] = { sizeof(_StringClass), 0 };
size_t _Writer_offsets[] = { sizeof(_Writer), 0 };
size_t _BaseArray_offsets[] = { sizeof(_BaseArray), 0 };
size_t _BaseDomain_offsets[] = { sizeof(_BaseDomain), 0 };
size_t _BaseArithmeticDomain_offsets[] = { sizeof(_BaseArithmeticDomain), 0 };
size_t _SingleLocaleDistribution_offsets[] = { sizeof(_SingleLocaleDistribution), 0 };
size_t _SeedGeneratorClass_offsets[] = { sizeof(_SeedGeneratorClass), 0 };
size_t _RandomStream_offsets[] = { sizeof(_RandomStream), 0 };
size_t _SingleLocaleArithmeticDomain_1_int32_t_0_offsets[] = { sizeof(_SingleLocaleArithmeticDomain_1_int32_t_0), (size_t)&((_SingleLocaleArithmeticDomain_1_int32_t_0*)NULL)->dist, 0 };
size_t _listNode_BaseArray_offsets[] = { sizeof(_listNode_BaseArray), (size_t)&((_listNode_BaseArray*)NULL)->data, (size_t)&((_listNode_BaseArray*)NULL)->next, 0 };
size_t _SingleLocaleArithmeticArray_locale_int32_t_0_1_0_offsets[] = { sizeof(_SingleLocaleArithmeticArray_locale_int32_t_0_1_0), (size_t)&((_SingleLocaleArithmeticArray_locale_int32_t_0_1_0*)NULL)->dom, (size_t)&((_SingleLocaleArithmeticArray_locale_int32_t_0_1_0*)NULL)->data, 0 };
size_t __ddata_locale_offsets[] = { sizeof(__ddata_locale), (size_t)-1,(size_t)&((__ddata_locale*)0)->size,(size_t)&((__ddata_locale*)0)->_data,0 };
size_t _SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0_offsets[] = { sizeof(_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0), (size_t)&((_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0*)NULL)->dom, (size_t)&((_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0*)NULL)->data, 0 };
size_t __ddata_int32_t_offsets[] = { sizeof(__ddata_int32_t), 0 };
size_t __sum_int64_t_offsets[] = { sizeof(__sum_int64_t), 0 };
size_t __min_int64_t_offsets[] = { sizeof(__min_int64_t), 0 };
size_t _SingleLocaleArithmeticDomain_1_int64_t_0_offsets[] = { sizeof(_SingleLocaleArithmeticDomain_1_int64_t_0), (size_t)&((_SingleLocaleArithmeticDomain_1_int64_t_0*)NULL)->dist, 0 };
size_t _SingleLocaleArithmeticArray__complex128_int64_t_0_1_0_offsets[] = { sizeof(_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0), (size_t)&((_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0*)NULL)->dom, (size_t)&((_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0*)NULL)->data, 0 };
size_t __ddata__complex128_offsets[] = { sizeof(__ddata__complex128), 0 };
size_t _SingleLocaleArithmeticDomain_1_int64_t_1_offsets[] = { sizeof(_SingleLocaleArithmeticDomain_1_int64_t_1), (size_t)&((_SingleLocaleArithmeticDomain_1_int64_t_1*)NULL)->dist, 0 };
size_t _SingleLocaleArithmeticArray__complex128_int64_t_0_1_1_offsets[] = { sizeof(_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1), (size_t)&((_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1*)NULL)->dom, (size_t)&((_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1*)NULL)->data, 0 };
size_t _SingleLocaleArithmeticArray__complex128_int32_t_1_1_0_offsets[] = { sizeof(_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0), (size_t)&((_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0*)NULL)->dom, (size_t)&((_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0*)NULL)->data, 0 };
size_t __max__real64_offsets[] = { sizeof(__max__real64), 0 };
size_t* cid2offsets(_class_id cid) {
  size_t* offsets = NULL;
  switch(cid) {
    case _e__EndCount:
    offsets = __EndCount_offsets;
    break;
    case _e_locale:
    offsets = _locale_offsets;
    break;
    case _e_file:
    offsets = _file_offsets;
    break;
    case _e_StringClass:
    offsets = _StringClass_offsets;
    break;
    case _e_Writer:
    offsets = _Writer_offsets;
    break;
    case _e_BaseArray:
    offsets = _BaseArray_offsets;
    break;
    case _e_BaseDomain:
    offsets = _BaseDomain_offsets;
    break;
    case _e_BaseArithmeticDomain:
    offsets = _BaseArithmeticDomain_offsets;
    break;
    case _e_SingleLocaleDistribution:
    offsets = _SingleLocaleDistribution_offsets;
    break;
    case _e_SeedGeneratorClass:
    offsets = _SeedGeneratorClass_offsets;
    break;
    case _e_RandomStream:
    offsets = _RandomStream_offsets;
    break;
    case _e_SingleLocaleArithmeticDomain_1_int32_t_0:
    offsets = _SingleLocaleArithmeticDomain_1_int32_t_0_offsets;
    break;
    case _e_listNode_BaseArray:
    offsets = _listNode_BaseArray_offsets;
    break;
    case _e_SingleLocaleArithmeticArray_locale_int32_t_0_1_0:
    offsets = _SingleLocaleArithmeticArray_locale_int32_t_0_1_0_offsets;
    break;
    case _e__ddata_locale:
    offsets = __ddata_locale_offsets;
    break;
    case _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0:
    offsets = _SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0_offsets;
    break;
    case _e__ddata_int32_t:
    offsets = __ddata_int32_t_offsets;
    break;
    case _e__sum_int64_t:
    offsets = __sum_int64_t_offsets;
    break;
    case _e__min_int64_t:
    offsets = __min_int64_t_offsets;
    break;
    case _e_SingleLocaleArithmeticDomain_1_int64_t_0:
    offsets = _SingleLocaleArithmeticDomain_1_int64_t_0_offsets;
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0:
    offsets = _SingleLocaleArithmeticArray__complex128_int64_t_0_1_0_offsets;
    break;
    case _e__ddata__complex128:
    offsets = __ddata__complex128_offsets;
    break;
    case _e_SingleLocaleArithmeticDomain_1_int64_t_1:
    offsets = _SingleLocaleArithmeticDomain_1_int64_t_1_offsets;
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1:
    offsets = _SingleLocaleArithmeticArray__complex128_int64_t_0_1_1_offsets;
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0:
    offsets = _SingleLocaleArithmeticArray__complex128_int32_t_1_1_0_offsets;
    break;
    case _e__max__real64:
    offsets = __max__real64_offsets;
    break;
    default:
    chpl_error("Bad cid in cid2offsets", 0, 0);
    break;
  }
  return offsets;
}
size_t cid2size(_class_id cid) {
  size_t size = 0;
  switch(cid) {
    case _e__EndCount:
    size = sizeof(__EndCount);
    break;
    case _e_locale:
    size = sizeof(_locale);
    break;
    case _e_file:
    size = sizeof(_file);
    break;
    case _e_StringClass:
    size = sizeof(_StringClass);
    break;
    case _e_Writer:
    size = sizeof(_Writer);
    break;
    case _e_BaseArray:
    size = sizeof(_BaseArray);
    break;
    case _e_BaseDomain:
    size = sizeof(_BaseDomain);
    break;
    case _e_BaseArithmeticDomain:
    size = sizeof(_BaseArithmeticDomain);
    break;
    case _e_SingleLocaleDistribution:
    size = sizeof(_SingleLocaleDistribution);
    break;
    case _e_SeedGeneratorClass:
    size = sizeof(_SeedGeneratorClass);
    break;
    case _e_RandomStream:
    size = sizeof(_RandomStream);
    break;
    case _e_SingleLocaleArithmeticDomain_1_int32_t_0:
    size = sizeof(_SingleLocaleArithmeticDomain_1_int32_t_0);
    break;
    case _e_listNode_BaseArray:
    size = sizeof(_listNode_BaseArray);
    break;
    case _e_SingleLocaleArithmeticArray_locale_int32_t_0_1_0:
    size = sizeof(_SingleLocaleArithmeticArray_locale_int32_t_0_1_0);
    break;
    case _e__ddata_locale:
    size = sizeof(__ddata_locale);
    break;
    case _e_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0:
    size = sizeof(_SingleLocaleArithmeticArray_int32_t_int32_t_0_1_0);
    break;
    case _e__ddata_int32_t:
    size = sizeof(__ddata_int32_t);
    break;
    case _e__sum_int64_t:
    size = sizeof(__sum_int64_t);
    break;
    case _e__min_int64_t:
    size = sizeof(__min_int64_t);
    break;
    case _e_SingleLocaleArithmeticDomain_1_int64_t_0:
    size = sizeof(_SingleLocaleArithmeticDomain_1_int64_t_0);
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0:
    size = sizeof(_SingleLocaleArithmeticArray__complex128_int64_t_0_1_0);
    break;
    case _e__ddata__complex128:
    size = sizeof(__ddata__complex128);
    break;
    case _e_SingleLocaleArithmeticDomain_1_int64_t_1:
    size = sizeof(_SingleLocaleArithmeticDomain_1_int64_t_1);
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1:
    size = sizeof(_SingleLocaleArithmeticArray__complex128_int64_t_0_1_1);
    break;
    case _e_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0:
    size = sizeof(_SingleLocaleArithmeticArray__complex128_int32_t_1_1_0);
    break;
    case _e__max__real64:
    size = sizeof(__max__real64);
    break;
    default:
    chpl_error("Bad cid in cid2size", 0, 0);
    break;
  }
  return size;
}
