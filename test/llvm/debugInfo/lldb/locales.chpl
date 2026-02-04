proc main() {
  for l in Locales {
    writeln("Locale ", l.id);
  }
}

// CHECK: Target
// CHECK-SAME: stopped

// CHECK: p Locales
// CHECK-NEXT: (ChapelArray::[domain(1,int(64),one)] locale) [0..0] ChapelLocale::locale {
// CHECK-NEXT: [0] = {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }

// CHECK: target var Locales
// CHECK-NEXT: (_ref(_array(unmanaged [domain(1,int(64),one)] locale))) Locales = 0x{{0*[0-9a-fA-F]+}} [0..0] ChapelLocale::locale: {
// CHECK-NEXT: [0] = {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }

// CHECK: target var Locales[0]
// CHECK-NEXT: (ChapelLocale::locale) Locales[0] = {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }

// CHECK: target var *Locales[0]._instance
// CHECK-NEXT: (ChapelLocale::BaseLocale) *Locales[0]._instance = {
// CHECK-NEXT: super = (cid = {{[0-9]+}})
// CHECK-NEXT: parent = {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }
// CHECK-NEXT: nPUsLogAcc = {{[0-9]+}}
// CHECK-NEXT: nPUsLogAll = {{[0-9]+}}
// CHECK-NEXT: nPUsPhysAcc = {{[0-9]+}}
// CHECK-NEXT: nPUsPhysAll = {{[0-9]+}}
// CHECK-NEXT: maxTaskPar = {{[0-9]+}}
// CHECK-NEXT: numColocales = {{[0-9]+}}
// CHECK-NEXT: runningTaskCounter = {{[0-9]+}}
// CHECK-NEXT: }

// CHECK: p l
// CHECK-NEXT: (_ref(locale)) 0x{{0*[0-9a-fA-F]+}}: {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }

// CHECK: p *l._instance
// CHECK-NEXT: (ChapelLocale::BaseLocale) {
// CHECK-NEXT: super = (cid = {{[0-9]+}})
// CHECK-NEXT: parent = {
// CHECK-NEXT: _instance = 0x{{0*[0-9a-fA-F]+}}
// CHECK-NEXT: }
// CHECK-NEXT: nPUsLogAcc = {{[0-9]+}}
// CHECK-NEXT: nPUsLogAll = {{[0-9]+}}
// CHECK-NEXT: nPUsPhysAcc = {{[0-9]+}}
// CHECK-NEXT: nPUsPhysAll = {{[0-9]+}}
// CHECK-NEXT: maxTaskPar = {{[0-9]+}}
// CHECK-NEXT: numColocales = {{[0-9]+}}
// CHECK-NEXT: runningTaskCounter = {{[0-9]+}}
// CHECK-NEXT: }

// CHECK: p l.id()
// CHECK-NEXT: (int(64)) 0

// CHECK: p l.maxTaskPar()
// CHECK-NEXT: (int(64)) {{[0-9]+}}

// CHECK: p here().maxTaskPar()
// CHECK-NEXT: (int(64)) {{[0-9]+}}
