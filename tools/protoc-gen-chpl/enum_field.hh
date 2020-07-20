#ifndef PB_ENUM_FIELD_HH
#define PB_ENUM_FIELD_HH

#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

#include <field_base.hh>

namespace chapel {
  
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  
  class EnumFieldGenerator : public FieldGeneratorBase {
   public:
    EnumFieldGenerator(const FieldDescriptor* descriptor);
    ~EnumFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

}  // namespace chapelespace chapel

#endif  // PB_ENUM_FIELD_HH
