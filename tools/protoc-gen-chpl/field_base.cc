#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/stubs/logging.h>

#include <field_base.hh>
#include <helpers.hh>

namespace chapel {
  

  void FieldGeneratorBase::SetCommonFieldVariables(
      map<string, string>* variables) {

    (*variables)["property_name"] = property_name();
    (*variables)["type_name"] = type_name(descriptor_);
    (*variables)["proto_type_name"] = proto_type_name(descriptor_);
    (*variables)["name"] = name();
    (*variables)["descriptor_name"] = descriptor_->name();
    (*variables)["number"] = number();
  }

  FieldGeneratorBase::FieldGeneratorBase(const FieldDescriptor* descriptor)
      : descriptor_(descriptor) {
    SetCommonFieldVariables(&variables_);
  }

  FieldGeneratorBase::~FieldGeneratorBase() {
  }

  string FieldGeneratorBase::property_name() {
    return GetPropertyName(descriptor_);
  }

  string FieldGeneratorBase::name() {
    return GetFieldName(descriptor_);
  }

  string FieldGeneratorBase::type_name(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_INT64:
        return "int(64)";
      case FieldDescriptor::TYPE_INT32:
        return "int(32)";
      case FieldDescriptor::TYPE_UINT64:
        return "uint(64)";
      case FieldDescriptor::TYPE_UINT32:
        return "uint(32)";
      case FieldDescriptor::TYPE_SINT32:
        return "int(32)";
      case FieldDescriptor::TYPE_SINT64:
        return "int(64)";
      case FieldDescriptor::TYPE_BOOL:
        return "bool";
      case FieldDescriptor::TYPE_BYTES:
        return "bytes";
      case FieldDescriptor::TYPE_STRING:
        return "string";
      case FieldDescriptor::TYPE_FIXED64:
        return "uint(64)";
      case FieldDescriptor::TYPE_FIXED32:
        return "uint(32)";
      case FieldDescriptor::TYPE_FLOAT:
        return "real(32)";
      case FieldDescriptor::TYPE_DOUBLE:
        return "real(64)";
      case FieldDescriptor::TYPE_SFIXED32:
        return "int(32)";
      case FieldDescriptor::TYPE_SFIXED64:
        return "int(64)";
      case FieldDescriptor::TYPE_ENUM:
        return descriptor->enum_type()->name();
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::proto_type_name(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_INT64:
        return "int64";
      case FieldDescriptor::TYPE_INT32:
        return "int32";
      case FieldDescriptor::TYPE_UINT64:
        return "uint64";
      case FieldDescriptor::TYPE_UINT32:
        return "uint32";
      case FieldDescriptor::TYPE_SINT32:
        return "sint32";
      case FieldDescriptor::TYPE_SINT64:
        return "sint64";
      case FieldDescriptor::TYPE_BOOL:
        return "bool";
      case FieldDescriptor::TYPE_BYTES:
        return "bytes";
      case FieldDescriptor::TYPE_STRING:
        return "string";
      case FieldDescriptor::TYPE_FIXED64:
        return "fixed64";
      case FieldDescriptor::TYPE_FIXED32:
        return "fixed32";
      case FieldDescriptor::TYPE_FLOAT:
        return "float";
      case FieldDescriptor::TYPE_DOUBLE:
        return "double";
      case FieldDescriptor::TYPE_SFIXED32:
        return "sfixed32";
      case FieldDescriptor::TYPE_SFIXED64:
        return "sfixed64";
      case FieldDescriptor::TYPE_ENUM:
        return "enum";
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::number() {
    return StrCat(descriptor_->number());
  }

}  // namespace chapel
