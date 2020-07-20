#include <string>

#include <google/protobuf/stubs/strutil.h>

#include <helpers.hh>
#include <primitive_field.hh>
#include <repeated_primitive_field.hh>
#include <enum_field.hh>
#include <repeated_enum_field.hh>
#include <field_base.hh>

namespace chapel {
  
  string StripDotProto(const std::string& proto_file) {
    int lastindex = proto_file.find_last_of(".");
    return proto_file.substr(0, lastindex);
  }

  string InvalidCharsToUnderscores(const string& input) {
    string result;
    for (int i = 0; i < input.size(); i++) {
      if ( isalnum(input[i]) || input[i] == '_') {
        result += input[i];
      } else {
        result += '_';
      }
    }
    return result;
  }

  string GetFileNameBase(const FileDescriptor* descriptor) {
      string proto_file = descriptor->name();
      int lastslash = proto_file.find_last_of("/");
      string base = proto_file.substr(lastslash + 1);
      return InvalidCharsToUnderscores(StripDotProto(base));
  }

  bool ValidateInputFileName(const FileDescriptor* descriptor) {
    string proto_file = descriptor->name();
    int lastindex = proto_file.find_last_of(".");
    if (proto_file.substr(lastindex+1) != "proto") {
      return false;
    }
    return true;
  }

  string GetPackageName(const FileDescriptor* descriptor) {
    return InvalidCharsToUnderscores(descriptor->package());
  }

  string GetModuleName(const FileDescriptor* descriptor) {
    string package_name = GetPackageName(descriptor);
    if (!package_name.empty()) {
      return package_name;
    }
    string proto_filename = GetFileNameBase(descriptor);
    return proto_filename;
  }

  string GetOutputFile(const FileDescriptor* descriptor, string* error) {
    string file_extension = ".chpl";

    bool valid_input_filename = ValidateInputFileName(descriptor);
    if (!valid_input_filename) {
      *error = "Input file should be a .proto file";
      return "";
    }

    string base_filename = GetModuleName(descriptor);
    return base_filename + file_extension;
  }

  string GetFieldName(const FieldDescriptor* descriptor) {
      return descriptor->name();
  }

  string GetPropertyName(const FieldDescriptor* descriptor) {
    string property_name = GetFieldName(descriptor);
    property_name += "_";
    return property_name;
  }

  FieldGeneratorBase* CreateFieldGenerator(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_ENUM:
        if (descriptor->is_repeated()) {
          return new RepeatedEnumFieldGenerator(descriptor);
        } else {
          return new EnumFieldGenerator(descriptor);
        }
      default:
        if (descriptor->is_repeated()) {
          return new RepeatedPrimitiveFieldGenerator(descriptor);
        } else {
          return new PrimitiveFieldGenerator(descriptor);
        }  
    }
  }

} // namespace chapel
