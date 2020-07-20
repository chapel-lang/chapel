#include <primitive_field.hh>

namespace chapel {

  PrimitiveFieldGenerator::PrimitiveFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  PrimitiveFieldGenerator::~PrimitiveFieldGenerator() {
  }

  void PrimitiveFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(
      variables_,
      "var $property_name$: $type_name$;\n"
      "proc $name$ { return $property_name$; }\n"
      "proc ref $name$ ref { return $property_name$; }\n"
    );
  }

}  // namespace chapel
