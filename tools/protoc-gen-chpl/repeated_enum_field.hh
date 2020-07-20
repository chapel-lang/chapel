#ifndef PB_REPEATED_ENUM_FIELD_HH
#define PB_REPEATED_ENUM_FIELD_HH

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

#include <field_base.hh>

namespace chapel {
  
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  
  class RepeatedEnumFieldGenerator : public FieldGeneratorBase {
   public:
    RepeatedEnumFieldGenerator(const FieldDescriptor* descriptor);
    ~RepeatedEnumFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

}  // namespace chapel

#endif /* PB_REPEATED_ENUM_FIELD_HH */
