#include "attribute.h"
#include "stringutil.h"

LLVMAttribute::LLVMAttribute(const char* key, LLVMAttributeType kind)
    : key(astr(key)), kind(kind) {}

LLVMAttributePtr LLVMAttribute::construct(const char* key) {
  return new LLVMAttribute(key, LAT_NO_VALUE);
}
LLVMAttributePtr LLVMAttribute::constructInt(const char* key, int64_t val) {
  auto la = new LLVMAttribute(key, LAT_INT);
  la->int_val = val;
  return la;
}
LLVMAttributePtr LLVMAttribute::constructBool(const char* key, bool val) {
  auto la = new LLVMAttribute(key, LAT_BOOL);
  la->bool_val = val;
  return la;
}
LLVMAttributePtr LLVMAttribute::constructString(const char* key, const char* val) {
  auto la = new LLVMAttribute(key, LAT_STRING);
  la->string_val = astr(val);
  return la;
}
LLVMAttributePtr LLVMAttribute::constructAttribute(const char* key, LLVMAttributePtr val) {
  auto la = new LLVMAttribute(key, LAT_ATTRIBUTE);
  la->attribute_val = val;
  return la;
}
