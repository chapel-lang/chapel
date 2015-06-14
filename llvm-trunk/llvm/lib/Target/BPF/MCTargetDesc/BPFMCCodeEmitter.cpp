//===-- BPFMCCodeEmitter.cpp - Convert BPF code to machine code -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the BPFMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/BPFMCTargetDesc.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

namespace {
class BPFMCCodeEmitter : public MCCodeEmitter {
  BPFMCCodeEmitter(const BPFMCCodeEmitter &) = delete;
  void operator=(const BPFMCCodeEmitter &) = delete;
  const MCRegisterInfo &MRI;

public:
  BPFMCCodeEmitter(const MCRegisterInfo &mri) : MRI(mri) {}

  ~BPFMCCodeEmitter() {}

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  // getMachineOpValue - Return binary encoding of operand. If the machin
  // operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  uint64_t getMemoryOpValue(const MCInst &MI, unsigned Op,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};
}

MCCodeEmitter *llvm::createBPFMCCodeEmitter(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            MCContext &Ctx) {
  return new BPFMCCodeEmitter(MRI);
}

unsigned BPFMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return MRI.getEncodingValue(MO.getReg());
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  assert(MO.isExpr());

  const MCExpr *Expr = MO.getExpr();

  assert(Expr->getKind() == MCExpr::SymbolRef);

  if (MI.getOpcode() == BPF::JAL)
    // func call name
    Fixups.push_back(MCFixup::create(0, Expr, FK_SecRel_4));
  else if (MI.getOpcode() == BPF::LD_imm64)
    Fixups.push_back(MCFixup::create(0, Expr, FK_SecRel_8));
  else
    // bb label
    Fixups.push_back(MCFixup::create(0, Expr, FK_PCRel_2));

  return 0;
}

void BPFMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  unsigned Opcode = MI.getOpcode();
  support::endian::Writer<support::little> LE(OS);

  if (Opcode == BPF::LD_imm64 || Opcode == BPF::LD_pseudo) {
    uint64_t Value = getBinaryCodeForInstr(MI, Fixups, STI);
    LE.write<uint8_t>(Value >> 56);
    LE.write<uint8_t>(((Value >> 48) & 0xff));
    LE.write<uint16_t>(0);
    LE.write<uint32_t>(Value & 0xffffFFFF);

    const MCOperand &MO = MI.getOperand(1);
    uint64_t Imm = MO.isImm() ? MO.getImm() : 0;
    LE.write<uint8_t>(0);
    LE.write<uint8_t>(0);
    LE.write<uint16_t>(0);
    LE.write<uint32_t>(Imm >> 32);
  } else {
    // Get instruction encoding and emit it
    uint64_t Value = getBinaryCodeForInstr(MI, Fixups, STI);
    LE.write<uint8_t>(Value >> 56);
    LE.write<uint8_t>((Value >> 48) & 0xff);
    LE.write<uint16_t>((Value >> 32) & 0xffff);
    LE.write<uint32_t>(Value & 0xffffFFFF);
  }
}

// Encode BPF Memory Operand
uint64_t BPFMCCodeEmitter::getMemoryOpValue(const MCInst &MI, unsigned Op,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  uint64_t Encoding;
  const MCOperand Op1 = MI.getOperand(1);
  assert(Op1.isReg() && "First operand is not register.");
  Encoding = MRI.getEncodingValue(Op1.getReg());
  Encoding <<= 16;
  MCOperand Op2 = MI.getOperand(2);
  assert(Op2.isImm() && "Second operand is not immediate.");
  Encoding |= Op2.getImm() & 0xffff;
  return Encoding;
}

#include "BPFGenMCCodeEmitter.inc"
