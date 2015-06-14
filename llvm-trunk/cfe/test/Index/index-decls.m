@interface I
@property (readonly) id prop;
 -(id)prop;
@end

@interface I()
@property (assign,readwrite) id prop;
@end

@implementation I
@synthesize prop = _prop;
@end

// rdar://11015325
@interface I1
__attribute__((something)) @interface I2 @end
@end

@interface I3
@property (assign,readwrite) id auto_prop;
@end

@implementation I3
-(void)meth {
  _auto_prop = 0;
}
@end

int test1() {
  extern int extvar;
  extvar = 2;
  extern int extfn();
  return extfn();
}

@interface I4
@property (assign, nonatomic) id prop;
-(id)prop;
-(void)setProp:(id)p;
@end

@implementation I4
@synthesize prop = _prop;
-(id)prop {
  return 0;
}
-(void)setProp:(id)p {
}
@end

// RUN: c-index-test -index-file %s -target x86_64-apple-macosx10.7 > %t
// RUN: FileCheck %s -input-file=%t
// CHECK: [indexDeclaration]: kind: objc-class | name: I | {{.*}} | loc: 1:12
// CHECK: [indexDeclaration]: kind: objc-instance-method | name: prop | {{.*}} | loc: 3:2
// CHECK: [indexDeclaration]: kind: objc-property | name: prop | {{.*}} | loc: 2:25
// CHECK: [indexDeclaration]: kind: objc-category | name:  | {{.*}} | loc: 6:12
// CHECK: [indexDeclaration]: kind: objc-instance-method | name: setProp: | {{.*}} | loc: 7:33
// CHECK: [indexDeclaration]: kind: objc-property | name: prop | {{.*}} | loc: 7:33

// CHECK: [indexDeclaration]: kind: objc-ivar | name: _prop | {{.*}} | loc: 11:20
// CHECK: [indexDeclaration]: kind: objc-instance-method | name: prop | {{.*}} | loc: 11:13 | {{.*}} | lexical-container: [I:10:17]
// CHECK: [indexDeclaration]: kind: objc-instance-method | name: setProp: | {{.*}} | loc: 11:13 | {{.*}} | lexical-container: [I:10:17]

// CHECK: [indexDeclaration]: kind: objc-ivar | name: _auto_prop | {{.*}} | loc: 20:33
// CHECK: [indexEntityReference]: kind: objc-ivar | name: _auto_prop | {{.*}} | loc: 25:3

// CHECK: [indexDeclaration]: kind: function | name: test1 | {{.*}} | loc: 29:5
// CHECK: [indexDeclaration]: kind: variable | name: extvar | {{.*}} | loc: 30:14
// CHECK: [indexEntityReference]: kind: variable | name: extvar | {{.*}} | loc: 31:3
// CHECK: [indexDeclaration]: kind: function | name: extfn | {{.*}} | loc: 32:14
// CHECK: [indexEntityReference]: kind: function | name: extfn | {{.*}} | loc: 33:10

// CHECK: [indexDeclaration]: kind: objc-class | name: I4 | {{.*}} | loc: 36:12
// CHECK-NOT: [indexDeclaration]: kind: objc-instance-method {{.*}} loc: 37:
// CHECK-NOT: [indexDeclaration]: kind: objc-instance-method {{.*}} loc: 43:
