#ifndef PB_PRIMITIVE_FIELD_HH
#define PB_PRIMITIVE_FIELD_HH

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

#include <field_base.hh>

namespace chapel {
  
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  
  class PrimitiveFieldGenerator : public FieldGeneratorBase {
   public:
    PrimitiveFieldGenerator(const FieldDescriptor* descriptor);
    ~PrimitiveFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

}  // namespace chapel

#endif /* PB_PRIMITIVE_FIELD_HH */
