#include <google/protobuf/stubs/strutil.h>

#include <enum.hh>

namespace chapel {

  EnumGenerator::EnumGenerator(const EnumDescriptor* descriptor) :
      descriptor_(descriptor) {
  }

  EnumGenerator::~EnumGenerator() {
  }

  void EnumGenerator::Generate(Printer* printer) {
    printer->Print("enum $name$ {\n",
                   "name", descriptor_->name());
    printer->Indent();
    
    for (int i = 0; i < descriptor_->value_count(); i++) {
      printer->Print("$name$ = $number$,\n",
                     "name", descriptor_->value(i)->name(),
                     "number", StrCat(descriptor_->value(i)->number()));
    }

    printer->Outdent();
    printer->Print("}\n"); 
              
  }

}  // namespace chapel
