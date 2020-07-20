#ifndef PB_ENUM_HH
#define PB_ENUM_HH

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace chapel {

  using namespace google::protobuf;
  using namespace google::protobuf::io;

  class EnumGenerator {
   public:
    EnumGenerator(const EnumDescriptor* descriptor);
    ~EnumGenerator();

    EnumGenerator(const EnumGenerator&) = delete;
    EnumGenerator& operator=(const EnumGenerator&) = delete;

    void Generate(Printer* printer);

   private:
    const EnumDescriptor* descriptor_;
  };

}  // namespace chapel

#endif  // PB_ENUM_HH
