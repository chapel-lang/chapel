#include <enum_field.hh>

namespace chapel {

  EnumFieldGenerator::EnumFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  EnumFieldGenerator::~EnumFieldGenerator() {
  }

  void EnumFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(
      variables_,
      "var $property_name$: $type_name$;\n"
      "proc $name$ { return $property_name$; }\n"
      "proc ref $name$ ref { return $property_name$; }\n"
    );
  }

}  // namespace chapel
