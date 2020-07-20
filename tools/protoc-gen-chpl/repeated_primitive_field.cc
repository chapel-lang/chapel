#include <repeated_primitive_field.hh>

namespace chapel {

  RepeatedPrimitiveFieldGenerator::RepeatedPrimitiveFieldGenerator(
      const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  RepeatedPrimitiveFieldGenerator::~RepeatedPrimitiveFieldGenerator() {

  }

  void RepeatedPrimitiveFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(
      variables_,
      "var $property_name$: list($type_name$);\n"
      "proc $name$ { return $property_name$; }\n"
      "proc ref $name$ ref { return $property_name$; }\n"
    );
  }

}  // namespace chapel
