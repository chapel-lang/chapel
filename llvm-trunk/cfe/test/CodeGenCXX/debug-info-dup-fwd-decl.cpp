// RUN: %clang_cc1 -emit-llvm -g -triple x86_64-apple-darwin -fstandalone-debug %s -o - | FileCheck %s

class Test
{
public:
    Test () : reserved (new data()) {}

    unsigned
    getID() const
    {
        return reserved->objectID;
    }
protected:
    struct data {
        unsigned objectID;
    };
    data* reserved;
};

Test t;

// CHECK: !DIDerivedType(tag: DW_TAG_pointer_type
// CHECK: !DICompositeType(tag: DW_TAG_structure_type, name: "data"
// CHECK-NOT: !DICompositeType(tag: DW_TAG_structure_type, name: "data"
