#include "metadata.h"
#include "stringutil.h"

// default initialize by calling shared_ptr constructor
LLVMMetadata::LLVMMetadata(const char* key, LLVMMetadataType kind)
    : key(astr(key)), kind(kind), attribute_val(nullptr) {}

LLVMMetadata::~LLVMMetadata() {
  if(kind == LAT_ATTRIBUTE) {
    attribute_val.reset();
  }
}

LLVMMetadataPtr LLVMMetadata::construct(const char* key) {
  return std::make_shared<LLVMMetadata>(key, LAT_NO_VALUE);
}
LLVMMetadataPtr LLVMMetadata::constructInt(const char* key, int64_t val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_INT);
  la->int_val = val;
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructBool(const char* key, bool val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_BOOL);
  la->bool_val = val;
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructString(const char* key, const char* val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_STRING);
  la->string_val = astr(val);
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructMetadata(const char* key, LLVMMetadataPtr val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_ATTRIBUTE);
  la->attribute_val = val;
  return la;
}
