#include <repeated_enum_field.hh>

namespace chapel {

  RepeatedEnumFieldGenerator::RepeatedEnumFieldGenerator(
      const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  RepeatedEnumFieldGenerator::~RepeatedEnumFieldGenerator() {

  }

  void RepeatedEnumFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(
      variables_,
      "var $property_name$: list($type_name$);\n"
      "proc $name$ { return $property_name$; }\n"
      "proc ref $name$ ref { return $property_name$; }\n"
    );
  }

}  // namespace chapel
