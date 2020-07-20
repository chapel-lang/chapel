#include <sstream>
#include <algorithm>
#include <map>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/wire_format_lite.h>

#include <helpers.hh>
#include <message.hh>
#include <enum.hh>
#include <field_base.hh>

namespace chapel {

  bool CompareFieldNumbers(const FieldDescriptor* d1, const FieldDescriptor* d2) {
    return d1->number() < d2->number();
  }

  MessageGenerator::MessageGenerator(const Descriptor* descriptor)
        : descriptor_(descriptor) {
  }

  MessageGenerator::~MessageGenerator() {
  }

  string MessageGenerator::record_name() {
    return descriptor_->name();
  }

  void MessageGenerator::Generate(Printer* printer) {
    map<int, map<string, string>> vars;

    for (int i = 0; i < descriptor_->field_count(); i++) {
      const FieldDescriptor* fieldDescriptor = descriptor_->field(i);
      FieldGeneratorBase* field_obj;
      
      vars[i]["field_name"] = GetPropertyName(fieldDescriptor);
      vars[i]["field_number"] = StrCat(fieldDescriptor->number());
      vars[i]["type_name"] = field_obj->type_name(fieldDescriptor);
      vars[i]["proto_field_type"] = field_obj->proto_type_name(fieldDescriptor);
      vars[i]["wire_format"] = StrCat(WireFormat::WireTypeForField(fieldDescriptor));
      vars[i]["is_repeated"] = StrCat(fieldDescriptor->is_repeated());
    }
    
    printer->Print(
      "record $record_name$ {\n",
      "record_name", record_name());
    printer->Print("\n");
    printer->Indent();

    for (int i = 0; i < descriptor_->field_count(); i++) {
      const FieldDescriptor* fieldDescriptor = descriptor_->field(i);

      printer->Print(
        "// Field \"$field_name$\" \n",
        "field_name", fieldDescriptor->name());
      unique_ptr<FieldGeneratorBase> generator(
          CreateFieldGeneratorInternal(fieldDescriptor));
      generator->GenerateMembers(printer);
      printer->Print("\n");
    }

    printer->Print("var unknownFieldStream: bytes = \"\";\n");
    printer->Print("\n");

    printer->Print(
      "proc writeToOutputFile(ch) throws {\n"
      "  writeToOutputFileHelper(this, ch);\n"
      "}\n"
      "\n");

    printer->Print("proc _writeToOutputFile(binCh) throws {\n");
    printer->Indent();

    for (int i = 0; i < descriptor_->field_count(); i++) {
      if(vars[i]["proto_field_type"] == "enum") {
        if(vars[i]["is_repeated"] == "0") {
          printer->Print(vars[i],
            "$proto_field_type$Append($field_name$:uint(64), $field_number$, binCh);\n");
        } else {
          printer->Print(vars[i],
            "$proto_field_type$RepeatedAppend($field_name$, $field_number$, binCh);\n");
        }
      } else {
        if(vars[i]["is_repeated"] == "0") {
          printer->Print(vars[i],
            "$proto_field_type$Append($field_name$, $field_number$, binCh);\n");
        } else {
          printer->Print(vars[i],
            "$proto_field_type$RepeatedAppend($field_name$, $field_number$, binCh);\n");
        }
      }
    }

    printer->Print("binCh.write(unknownFieldStream);\n");
    printer->Outdent();
    printer->Print("}\n");

    printer->Print("\n");
    printer->Print(
      "proc parseFromInputFile(ch) throws {\n"
      "  parseFromInputFileHelper(this, ch);\n"
      "}\n"
      "\n");

    printer->Print(
      "proc _parseFromInputFile(binCh) throws {\n"
      "  while true {\n"
      "    var (fieldNumber, wireType) = tagConsume(binCh);\n"
      "    select fieldNumber {\n");
    printer->Indent();
    printer->Indent();
    printer->Indent();

    for (int i = 0; i < descriptor_->field_count(); i++) {
      printer->Print(vars[i],
        "when $field_number$ {\n");
        if(vars[i]["proto_field_type"] == "enum") {
          if(vars[i]["is_repeated"] == "0") {
            printer->Print(vars[i],
              "  $field_name$ = $proto_field_type$Consume(binCh):$type_name$;\n");
          } else {
            printer->Print(vars[i],
              "  $field_name$ = $proto_field_type$RepeatedConsume(binCh, $type_name$);\n");
          }
        } else {
          if(vars[i]["is_repeated"] == "0") {
            printer->Print(vars[i],
              "  $field_name$ = $proto_field_type$Consume(binCh);\n");
          } else {
            printer->Print(vars[i],
              "  $field_name$.extend($proto_field_type$RepeatedConsume(binCh));\n");
          }
        }

        printer->Print("}\n");
    }

    printer->Print(
      "when -1 {\n"
      "  break;\n"
      "}\n"
      "otherwise {\n"
      "  unknownFieldStream += consumeUnknownField(fieldNumber, wireType, binCh);\n"
      "}\n");

    printer->Outdent();
    printer->Print("}\n");
    printer->Outdent();
    printer->Print("}\n");
    printer->Outdent();
    printer->Print("}\n");

    printer->Print("\n");
    printer->Outdent();
    printer->Print("}\n");
  }

  FieldGeneratorBase* MessageGenerator::CreateFieldGeneratorInternal(
      const FieldDescriptor* descriptor) {
    return CreateFieldGenerator(descriptor);
  }

}  // namespace chapel
