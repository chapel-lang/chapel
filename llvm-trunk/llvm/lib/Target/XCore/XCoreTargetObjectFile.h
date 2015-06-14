//===-- XCoreTargetObjectFile.h - XCore Object Info -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_XCORE_XCORETARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_XCORE_XCORETARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

static const unsigned CodeModelLargeSize = 256;

  class XCoreTargetObjectFile : public TargetLoweringObjectFileELF {
    MCSection *BSSSectionLarge;
    MCSection *DataSectionLarge;
    MCSection *ReadOnlySectionLarge;
    MCSection *DataRelROSectionLarge;

  public:
    void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

    MCSection *getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                                        Mangler &Mang,
                                        const TargetMachine &TM) const override;

    MCSection *SelectSectionForGlobal(const GlobalValue *GV, SectionKind Kind,
                                      Mangler &Mang,
                                      const TargetMachine &TM) const override;

    MCSection *getSectionForConstant(SectionKind Kind,
                                     const Constant *C) const override;
  };
} // end namespace llvm

#endif
