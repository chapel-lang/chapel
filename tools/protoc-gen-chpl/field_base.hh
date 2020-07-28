#ifndef PB_FIELD_BASE_HH
#define PB_FIELD_BASE_HH

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace chapel {
  
  using namespace std;

  using namespace google::protobuf;
  using namespace google::protobuf::io;
  using namespace google::protobuf::internal;

  class FieldGeneratorBase {
   public:
    FieldGeneratorBase(const FieldDescriptor* descriptor);
    ~FieldGeneratorBase();

    virtual void GenerateMembers(Printer* printer) = 0;
    string name();
    string type_name(const FieldDescriptor* descriptor);
    string proto_type_name(const FieldDescriptor* descriptor);
    string default_value(const FieldDescriptor* descriptor);
    string number();

    const FieldDescriptor* descriptor_;
    map<string, string> variables_;

   private:
     void SetCommonFieldVariables(map<string, string>* variables);
  };

}  // namespace chapel

#endif  // PB_FIELD_BASE_HH
