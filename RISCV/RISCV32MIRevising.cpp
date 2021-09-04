//===- RISCV32MIRevising.cpp - Binary raiser utility llvm-mctoll --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of RISCV32MIRevising class for use by
// llvm-mctoll.
//
//===----------------------------------------------------------------------===//

#include "RISCV32MIRevising.h"
#include "RISCV32ModuleRaiser.h"
#include "RISCVSubtarget.h"
#include "ExternalFunctions.h"
#include "MCInstRaiser.h"
#include "MachineFunctionRaiser.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Object/ELF.h"
#include "llvm/Object/ELFObjectFile.h"

#define DEBUG_TYPE "mctoll"

using namespace llvm;
using namespace llvm::object;

char RISCV32MIRevising::ID = 0;

RISCV32MIRevising::RISCV32MIRevising(RISCV32ModuleRaiser &MRsr) : RISCV32RaiserBase(ID, MRsr) {
  MCIR = nullptr;
}

RISCV32MIRevising::~RISCV32MIRevising() {}

void RISCV32MIRevising::init(MachineFunction *mf, Function *rf) {
  RISCV32RaiserBase::init(mf, rf);
}

void RISCV32MIRevising::setMCInstRaiser(MCInstRaiser *PMCIR) { MCIR = PMCIR; }

// Extract the offset of MachineInstr MI from the Metadata operand.
static uint64_t getMCInstIndex(const MachineInstr &MI) {
  unsigned NumExpOps = MI.getNumExplicitOperands();
  const MachineOperand &MO = MI.getOperand(NumExpOps);
  assert(MO.isMetadata() &&
         "Unexpected non-metadata operand in branch instruction!");
  const MDNode *MDN = MO.getMetadata();
  // Unwrap metadata of the instruction to get the MCInstIndex of
  // the MCInst corresponding to this MachineInstr.
  ConstantAsMetadata *CAM = dyn_cast<ConstantAsMetadata>(MDN->getOperand(0));
  assert(CAM != nullptr && "Unexpected metadata type!");
  Constant *CV = CAM->getValue();
  ConstantInt *CI = dyn_cast<ConstantInt>(CV);
  assert(CI != nullptr && "Unexpected metadata constant type!");
  APInt ArbPrecInt = CI->getValue();
  return ArbPrecInt.getSExtValue();
}

template <class ELFT>
uint64_t getLoadAlignProgramHeader(const ELFFile<ELFT> *Obj) {
  typedef ELFFile<ELFT> ELFO;
  auto ProgramHeaderOrError = Obj->program_headers();

  if (!ProgramHeaderOrError)
    report_fatal_error(
        errorToErrorCode(ProgramHeaderOrError.takeError()).message());

  for (const typename ELFO::Elf_Phdr &Phdr : *ProgramHeaderOrError) {
    if (Phdr.p_type == ELF::PT_LOAD)
      return (uint64_t)Phdr.p_align;
  }

  assert(false && "Failed to get Phdr p_align!");
  return 0;
}

/// Create function for external function.
uint64_t RISCV32MIRevising::getCalledFunctionAtPLTOffset(uint64_t PLTEndOff,
                                                     uint64_t CallAddr) {
  LLVM_DEBUG(dbgs() << "\n@getCalledFunctionAtPLTOffset\n<<PltEndOff="<<PLTEndOff<<",calladdr="<<CallAddr<<"\n");
  
  //return 0;
  /*
  To do : fix plt table

  For arm 
     10290:       e28fc600        add     ip, pc, #0, 12
     10294:       e28cca10        add     ip, ip, #16, 20 ; 0x10000
     10298:       e5bcf348        ldr     pc, [ip, #840]! ; 0x348
  */
  const ELF32LEObjectFile *Elf32LEObjFile =
      dyn_cast<ELF32LEObjectFile>(MR->getObjectFile());
  assert(Elf32LEObjFile != nullptr &&
         "Only 32-bit ELF binaries supported at present!");
  unsigned char ExecType = Elf32LEObjFile->getELFFile().getHeader().e_type;

  assert((ExecType == ELF::ET_DYN) || (ExecType == ELF::ET_EXEC));
  // Find the section that contains the offset. That must be the PLT section
  for (section_iterator SecIter : Elf32LEObjFile->sections()) {
    uint64_t SecStart = SecIter->getAddress();
    uint64_t SecEnd = SecStart + SecIter->getSize();
    if ((SecStart <= PLTEndOff) && (SecEnd >= PLTEndOff)) {
      StringRef SecName;
      if (auto NameOrErr = SecIter->getName())
        SecName = *NameOrErr;
      else {
        consumeError(NameOrErr.takeError());
        assert(false && "Failed to get section name with PLT offset");
      }
      if (SecName.compare(".plt") != 0) {
        assert(false && "Unexpected section name of PLT offset");
      }

      auto StrOrErr = SecIter->getContents();
      assert(StrOrErr && "Failed to get the content of section!");
      auto SecData = *StrOrErr;
      ArrayRef<uint8_t> Bytes(reinterpret_cast<const uint8_t *>(SecData.data()),
                              SecData.size());

      MCInst InstAUIPC;
      uint64_t InstAUIPCSz;
      bool Success = MR->getMCDisassembler()->getInstruction(
          InstAUIPC, InstAUIPCSz , Bytes.slice(PLTEndOff - SecStart),
          PLTEndOff , nulls());
      assert(Success && "Failed to disassemble instruction in PLT");
      LLVM_DEBUG(dbgs()<<"InstAUIPC="<<InstAUIPC<<"\n");
      unsigned int OpcAUIPC = InstAUIPC.getOpcode();
     // MCInstrDesc MCIDAddIP = MR->getMCInstrInfo()->get(OpcAddIP);

      if (OpcAUIPC != RISCV::AUIPC ) {
        assert(false && "Failed to find function entry from .plt.");
      }

      MCOperand OpdAUIPC= InstAUIPC.getOperand(1);
      assert(OpdAUIPC.isImm() && "Unexpected immediate for offset.");
      unsigned Bits = OpdAUIPC.getImm() & 0xFFFFF;
      int64_t RipOffset = (Bits << 12) + PLTEndOff;//static_cast<int64_t>(RISCV32_AM::rotr32(Bits, Rot));

      MCInst Inst;
      uint64_t InstSz;
      Success = MR->getMCDisassembler()->getInstruction(
          Inst, InstSz, Bytes.slice(PLTEndOff + 4 - SecStart), PLTEndOff + 4,
          nulls());
      assert(Success && "Failed to disassemble instruction in PLT");
      unsigned int Opcode = Inst.getOpcode();
      MCInstrDesc MCID = MR->getMCInstrInfo()->get(Opcode);
/*
      if (Opcode != RISCV32::LDRi12 && (MCID.getNumOperands() != 6)) {
        assert(false && "Failed to find function entry from .plt.");
      }*/
      LLVM_DEBUG(dbgs()<<"LW Inst="<<Inst << "\n");
      MCOperand Operand = Inst.getOperand(2);
      assert(Operand.isImm() && "Unexpected immediate for offset.");

      uint64_t Index = Operand.getImm();

      uint64_t GotPltRelocOffset = RipOffset+ Index;
      LLVM_DEBUG(dbgs()<<"LW Index="<<Index<< ",gotpltrelocoffset="<<GotPltRelocOffset<<"\n");
      const RelocationRef *GotPltReloc =
          MR->getDynRelocAtOffset(GotPltRelocOffset);
      assert(GotPltReloc != nullptr &&
             "Failed to get dynamic relocation for jmp target of PLT entry");

      //assert((GotPltReloc->getType() == ELF::R_RISCV32_JUMP_SLOT) &&
        //     "Unexpected relocation type for PLT jmp instruction");
      symbol_iterator CalledFuncSym = GotPltReloc->getSymbol();
      assert(CalledFuncSym != Elf32LEObjFile->symbol_end() &&
             "Failed to find relocation symbol for PLT entry");
      Expected<StringRef> CalledFuncSymName = CalledFuncSym->getName();
      assert(CalledFuncSymName &&
             "Failed to find symbol associated with dynamic "
             "relocation of PLT jmp target.");
      Expected<uint64_t> CalledFuncSymAddr = CalledFuncSym->getAddress();
      assert(CalledFuncSymAddr &&
             "Failed to get called function address of PLT entry");

      if (CalledFuncSymAddr.get() == 0) {
        // Set CallTargetIndex for plt offset to map undefined function symbol
        // for emit CallInst use.
        Function *CalledFunc =
            ExternalFunctions::Create(*CalledFuncSymName, *MR);
        // Bail out if function prototype is not available
        if (!CalledFunc)
          exit(-1);
        MR->setSyscallMapping(PLTEndOff, CalledFunc);
        MR->fillInstAddrFuncMap(CallAddr, CalledFunc);
      }
      return CalledFuncSymAddr.get();
    }
  }
  return 0;
}

/// Relocate call branch instructions in object files.
void RISCV32MIRevising::relocateBranch(MachineInstr &MInst) {
  int64_t relCallTargetOffset = MInst.getOperand(1).getImm();
  const ELF32LEObjectFile *Elf32LEObjFile =
      dyn_cast<ELF32LEObjectFile>(MR->getObjectFile());
  assert(Elf32LEObjFile != nullptr &&
         "Only 32-bit ELF binaries supported at present.");

  auto EType = Elf32LEObjFile->getELFFile().getHeader().e_type;
  if ((EType == ELF::ET_DYN) || (EType == ELF::ET_EXEC)) {
    int64_t textSectionAddress = MR->getTextSectionAddress();
    assert(textSectionAddress >= 0 && "Failed to find text section address");

    // Get MCInst offset - the offset of machine instruction in the binary
    // and instruction size
    int64_t MCInstOffset = getMCInstIndex(MInst);
    int64_t CallAddr = MCInstOffset + textSectionAddress;
    int64_t CallTargetIndex = CallAddr + relCallTargetOffset;
    assert(MCIR != nullptr && "MCInstRaiser was not initialized");
    int64_t CallTargetOffset = CallTargetIndex - textSectionAddress;
    if (CallTargetOffset < 0 || !MCIR->isMCInstInRange(CallTargetOffset)) {
      Function *CalledFunc = nullptr;
      uint64_t MCInstSize = MCIR->getMCInstSize(MCInstOffset);
      uint64_t Index = 1;
      CalledFunc = MR->getRaisedFunctionAt(CallTargetIndex);
      if (CalledFunc == nullptr) {
        CalledFunc =
            MR->getCalledFunctionUsingTextReloc(MCInstOffset, MCInstSize);
      }
      // Look up the PLT to find called function.
      if (CalledFunc == nullptr)
        Index = getCalledFunctionAtPLTOffset(CallTargetIndex, CallAddr);

      if (CalledFunc == nullptr) {
        if (Index == 0)
          MInst.getOperand(1).setImm(CallTargetIndex);
        else if (Index != 1)
          MInst.getOperand(1).setImm(Index);
        else
          assert(false && "Failed to get the call function!");
      } else
        MInst.getOperand(1).setImm(CallTargetIndex);
    } else if(MCIR->isMCInstInRange(CallTargetOffset)){
      MInst.getOperand(1).setImm(CallTargetIndex);
    }
  } else {
    uint64_t Offset = getMCInstIndex(MInst);
    const RelocationRef *reloc = MR->getTextRelocAtOffset(Offset, 4);
    auto ImmValOrErr = (*reloc->getSymbol()).getValue();
    assert(ImmValOrErr && "Failed to get immediate value");
    MInst.getOperand(1).setImm(*ImmValOrErr);
  }
}

/// Find global value by PC offset.
const Value *RISCV32MIRevising::getGlobalValueByOffset(int64_t MCInstOffset,
                                                   uint64_t PCOffset) {
  // const Value *GlobVal = nullptr;
  // const ELF32LEObjectFile *ObjFile =
  //     dyn_cast<ELF32LEObjectFile>(MR->getObjectFile());
  // assert(ObjFile != nullptr &&
  //        "Only 32-bit ELF binaries supported at present.");

  // // Get the text section address
  // int64_t TextSecAddr = MR->getTextSectionAddress();
  // assert(TextSecAddr >= 0 && "Failed to find text section address");

  // uint64_t InstAddr = TextSecAddr + MCInstOffset;
  // uint64_t Offset = InstAddr + PCOffset;

  // // Start to search the corresponding symbol.
  // const SymbolRef *Symbol = nullptr;
  // const RelocationRef *DynReloc = MR->getDynRelocAtOffset(Offset);
  // if (DynReloc && (DynReloc->getType() == ELF::R_RISCV32_ABS32 ||
  //                  DynReloc->getType() == ELF::R_RISCV32_GLOB_DAT))
  //   Symbol = &*DynReloc->getSymbol();

  // assert(MCIR != nullptr && "MCInstRaiser was not initialized!");
  // if (Symbol == nullptr) {
  //   auto Iter = MCIR->getMCInstAt(Offset - TextSecAddr);
  //   uint64_t OffVal = static_cast<uint64_t>((*Iter).second.getData());

  //   for (auto &Sym : ObjFile->symbols()) {
  //     if (Sym.getELFType() == ELF::STT_OBJECT) {
  //       auto SymAddr = Sym.getAddress();
  //       assert(SymAddr && "Failed to lookup symbol for global address!");

  //       if (OffVal >= SymAddr.get() &&
  //           OffVal < (SymAddr.get() + Sym.getSize())) {
  //         Symbol = &Sym;
  //         break;
  //       }
  //     }
  //   }
  // }

  // LLVMContext &LCTX = M->getContext();
  // if (Symbol != nullptr) {
  //   // If the symbol is found.
  //   Expected<StringRef> SymNameVal = Symbol->getName();
  //   assert(SymNameVal &&
  //          "Failed to find symbol associated with dynamic relocation.");
  //   auto SymName = SymNameVal.get();
  //   GlobVal = M->getGlobalVariable(SymName);
  //   if (GlobVal == nullptr) {
  //     DataRefImpl SymImpl = Symbol->getRawDataRefImpl();
  //     auto SymbOrErr = ObjFile->getSymbol(SymImpl);
  //     if (!SymbOrErr)
  //       consumeError(SymbOrErr.takeError());
  //     else {
  //       auto Symb = SymbOrErr.get();
  //       assert((Symb->getType() == ELF::STT_OBJECT) &&
  //              "Object symbol type is expected. But not found!");
  //       GlobalValue::LinkageTypes Linkage;
  //       switch (Symb->getBinding()) {
  //       case ELF::STB_GLOBAL:
  //         Linkage = GlobalValue::ExternalLinkage;
  //         break;
  //       default:
  //         assert(false && "Unhandled dynamic symbol");
  //       }
  //       uint64_t SymSz = Symb->st_size;
  //       Type *GlobValTy = nullptr;
  //       switch (SymSz) {
  //       case 4:
  //         GlobValTy = Type::getInt32Ty(LCTX);
  //         break;
  //       case 2:
  //         GlobValTy = Type::getInt16Ty(LCTX);
  //         break;
  //       case 1:
  //         GlobValTy = Type::getInt8Ty(LCTX);
  //         break;
  //       default:
  //         GlobValTy = ArrayType::get(Type::getInt8Ty(LCTX), SymSz);
  //         break;
  //       }

  //       auto SymOrErr = Symbol->getValue();
  //       if (!SymOrErr)
  //         report_error(SymOrErr.takeError(), "Can not find the symbol!");

  //       uint64_t SymVirtAddr = *SymOrErr;
  //       auto SecOrErr = Symbol->getSection();
  //       if (!SecOrErr)
  //         report_error(SecOrErr.takeError(),
  //                      "Can not find the section which is the symbol in!");

  //       section_iterator SecIter = *SecOrErr;
  //       Constant *GlobInit = nullptr;
  //       if (SecIter->isBSS()) {
  //         Linkage = GlobalValue::CommonLinkage;
  //         if (ArrayType::classof(GlobValTy))
  //           GlobInit = ConstantAggregateZero::get(GlobValTy);
  //         else
  //           GlobInit = ConstantInt::get(GlobValTy, 0);
  //       } else {
  //         auto StrOrErr = SecIter->getContents();
  //         if (!StrOrErr)
  //           report_error(StrOrErr.takeError(),
  //                        "Failed to get the content of section!");
  //         StringRef SecData = *StrOrErr;
  //         // Currently, Symbol->getValue() is virtual address.
  //         unsigned Index = SymVirtAddr - SecIter->getAddress();
  //         const unsigned char *Beg = SecData.bytes_begin() + Index;
  //         char Shift = 0;
  //         uint64_t InitVal = 0;
  //         while (SymSz-- > 0) {
  //           // We know this is little-endian
  //           InitVal = ((*Beg++) << Shift) | InitVal;
  //           Shift += 8;
  //         }
  //         GlobInit = ConstantInt::get(GlobValTy, InitVal);
  //       }

  //       auto GlobVar = new GlobalVariable(*M, GlobValTy, false /* isConstant */,
  //                                         Linkage, GlobInit, SymName);
  //       uint64_t Align = 32;
  //       switch (SymSz) {
  //       default:
  //       case 4:
  //         // When the symbol size is bigger than 4 bytes, identify the object as
  //         // array or struct and set alignment to 32 bits.
  //         Align = 32;
  //         break;
  //       case 2:
  //         Align = 16;
  //         break;
  //       case 1:
  //         Align = 8;
  //         break;
  //       }
  //       MaybeAlign MA(Align);
  //       GlobVar->setAlignment(MA);
  //       GlobVar->setDSOLocal(true);
  //       GlobVal = GlobVar;
  //     }
  //   }
  // } else {
  //   // If can not find the corresponding symbol.
  //   GlobVal = MR->getRODataValueAt(Offset);
  //   if (GlobVal == nullptr) {
  //     uint64_t Index = Offset - TextSecAddr;
  //     if (MCIR->getMCInstAt(Index) != MCIR->const_mcinstr_end()) {
  //       std::string LocalName("ROConst");
  //       LocalName.append(std::to_string(Index));
  //       // Find if a global value associated with symbol name is already
  //       // created
  //       StringRef LocalNameRef(LocalName);
  //       GlobVal = M->getGlobalVariable(LocalNameRef);
  //       if (GlobVal == nullptr) {
  //         MCInstOrData MD = MCIR->getMCInstAt(Index)->second;
  //         uint32_t Data = MD.getData();
  //         uint64_t DataAddr = (uint64_t)Data;
  //         // Check if this is an address in .rodata
  //         for (section_iterator SecIter : ObjFile->sections()) {
  //           uint64_t SecStart = SecIter->getAddress();
  //           uint64_t SecEnd = SecStart + SecIter->getSize();

  //           if ((SecStart <= DataAddr) && (SecEnd >= DataAddr)) {
  //             if (SecIter->isData()) {
  //               auto StrOrErr = SecIter->getContents();
  //               assert(StrOrErr && "Failed to get the content of section!");
  //               StringRef SecData = *StrOrErr;
  //               uint64_t DataOffset = DataAddr - SecStart;
  //               const unsigned char *RODataBegin =
  //                   SecData.bytes_begin() + DataOffset;

  //               unsigned char c;
  //               uint64_t argNum = 0;
  //               const unsigned char *str = RODataBegin;
  //               do {
  //                 c = (unsigned char)*str++;
  //                 if (c == '%') {
  //                   argNum++;
  //                 }
  //               } while (c != '\0');
  //               if (argNum != 0) {
  //                 MR->collectRodataInstAddr(InstAddr);
  //                 MR->fillInstArgMap(InstAddr, argNum + 1);
  //               }
  //               StringRef ROStringRef(
  //                   reinterpret_cast<const char *>(RODataBegin));
  //               Constant *StrConstant =
  //                   ConstantDataArray::getString(LCTX, ROStringRef);
  //               auto GlobalStrConstVal = new GlobalVariable(
  //                   *M, StrConstant->getType(), /* isConstant */ true,
  //                   GlobalValue::PrivateLinkage, StrConstant, "RO-String");
  //               // Record the mapping between offset and global value
  //               MR->addRODataValueAt(GlobalStrConstVal, Offset);
  //               GlobVal = GlobalStrConstVal;
  //               break;
  //             }
  //           }
  //         }

  //         if (GlobVal == nullptr) {
  //           Type *ty = Type::getInt32Ty(LCTX);
  //           Constant *GlobInit = ConstantInt::get(ty, Data);
  //           auto GlobVar = new GlobalVariable(*M, ty, /* isConstant */ true,
  //                                             GlobalValue::PrivateLinkage,
  //                                             GlobInit, LocalNameRef);
  //           MaybeAlign MA(32);
  //           GlobVar->setAlignment(MA);
  //           GlobVar->setDSOLocal(true);
  //           GlobVal = GlobVar;
  //         }
  //       }
  //     }
  //   }
  // }

  // return GlobVal;
}

/*
/// Address PC relative data in function, and create corresponding global value.
void RISCV32MIRevising::addressPCRelativeData(MachineInstr &MInst) {
  const Value *GlobVal = nullptr;
  int64_t Imm = 0;
  // To match the pattern: OPCODE Rx, [PC, #IMM]
  if (MInst.getNumOperands() > 2) {
    assert(MInst.getOperand(2).isImm() &&
           "The third operand must be immediate data!");
    Imm = MInst.getOperand(2).getImm();
  }
  // Get MCInst offset - the offset of machine instruction in the binary
  // and instruction size
  int64_t MCInstOffset = getMCInstIndex(MInst);
  GlobVal = getGlobalValueByOffset(MCInstOffset, static_cast<uint64_t>(Imm) + 8);

  // Check the next instruction whether it is also related to PC relative data
  // of global variable.
  // It should like:
  // ldr     r1, [pc, #32]
  // ldr     r1, [pc, r1]
  MachineInstr *NInst = MInst.getNextNode();
  // To match the pattern: OPCODE Rx, [PC, Rd], Rd must be the def of previous
  // instruction.
  if (NInst->getNumOperands() >= 2 && NInst->getOperand(1).isReg() &&
      NInst->getOperand(1).getReg() == RISCV::PC &&
      NInst->getOperand(2).isReg() &&
      NInst->getOperand(2).getReg() == MInst.getOperand(0).getReg()) {
    auto GV = dyn_cast<GlobalVariable>(GlobVal);
    if (GV != nullptr && GV->isConstant()) {
      // Firstly, read the PC relative data according to PC offset.
      auto Init = GV->getInitializer();
      uint64_t GVData = Init->getUniqueInteger().getZExtValue();
      int64_t MCInstOff = getMCInstIndex(*NInst);
      // Search the global symbol of object by PC relative data.
      GlobVal = getGlobalValueByOffset(MCInstOff, GVData + 8);
      // If the global symbol is exist, erase current ldr instruction.
      if (GlobVal != nullptr)
        NInst->eraseFromParent();
    }
  }

  assert(GlobVal && "A not addressed pc-relative data!");

  // Replace PC relative operands to symbol operand.
  // The pattern will be generated.
  // ldr r3, [pc, #20] => ldr r3, @globalvalue
  MInst.getOperand(1).ChangeToES(GlobVal->getName().data());

  if (MInst.getNumOperands() > 2) {
    MInst.RemoveOperand(2);
  }
}
*/

// /// Decode modified immediate constants in some instructions with immediate
// /// operand.
// void RISCV32MIRevising::decodeModImmOperand(MachineInstr &MInst) {
//   switch (MInst.getOpcode()) {
//   default:
//     break;
//   case RISCV32::ORRri:
//     MachineOperand &mo = MInst.getOperand(2);
//     unsigned Bits = mo.getImm() & 0xFF;
//     unsigned Rot = (mo.getImm() & 0xF00) >> 7;
//     int64_t Rotated = static_cast<int64_t>(RISCV32_AM::rotr32(Bits, Rot));
//     mo.setImm(Rotated);
//     break;
//   }
// }

/// Remove some useless operations of instructions. Some instructions like
/// NOP (mov r0, r0).
bool RISCV32MIRevising::removeNeedlessInst(MachineInstr *MInst) {
  if (MInst->getOpcode() == RISCV::ADDI && MInst->getNumOperands() >= 2 &&
      MInst->getOperand(0).isReg() && MInst->getOperand(1).isImm() &&
      MInst->getOperand(0).getImm() == 0) {
    return true;
  }

  return false;
}

/// The entry function of this class.
bool RISCV32MIRevising::reviseMI(MachineInstr &MInst) {
  // no orr in riscv
  // decodeModImmOperand(MInst);
  // Relocate BL target in same section.

  
  
  // LLVM_DEBUG(dbgs() << MInst.getOpcode() << "\n");
  // LLVM_DEBUG(dbgs() << RISCV::JAL << "\n");

  // To do : add other branch target function: jalr, beq, bne, bge, blt, bltu, begu
  // To do : fix plt function

  // jal rd(operand0), target(operand1)
  // j target -> jal x0(operand0), target(operand1)
  if (MInst.getOpcode() == RISCV::JAL) {
    MachineOperand &mo1 = MInst.getOperand(1);
    if (mo1.isImm()){
      LLVM_DEBUG(dbgs() << MInst << "\n");
      relocateBranch(MInst);
    }
  }

  // if (MInst.getOpcode() == RISCV32::LDRi12 || MInst.getOpcode() == RISCV32:: ) {
  //   if (MInst.getNumOperands() >= 2 && MInst.getOperand(1).isReg() &&
  //       MInst.getOperand(1).getReg() == ARM::PC) {
  //     addressPCRelativeData(MInst);
  //   }
  // }

  return true;
}

bool RISCV32MIRevising::revise() {
  bool rtn = false;
  LLVM_DEBUG(dbgs() << "RISCV32MIRevising start.\n");

  vector<MachineInstr *> RMVec;
  for (MachineFunction::iterator mbbi = MF->begin(), mbbe = MF->end();
       mbbi != mbbe; ++mbbi) {
    for (MachineBasicBlock::iterator mii = mbbi->begin(), mie = mbbi->end();
         mii != mie; ++mii) {
      if (removeNeedlessInst(&*mii)) {
        RMVec.push_back(&*mii);
        rtn = true;
      } else
        rtn = reviseMI(*mii);
    }
  }
  for (MachineInstr *PMI : RMVec)
    PMI->eraseFromParent();

  // For debugging.
  LLVM_DEBUG(MF->dump());
  LLVM_DEBUG(getCRF()->dump());
  LLVM_DEBUG(dbgs() << "RISCV32MIRevising end.\n");

  return rtn;
}

bool RISCV32MIRevising::runOnMachineFunction(MachineFunction &mf) {
  bool rtn = false;
  init();
  rtn = revise();
  return rtn;
}

#undef DEBUG_TYPE

#ifdef __cplusplus
extern "C" {
#endif

FunctionPass *InitializeRISCV32MIRevising(RISCV32ModuleRaiser &mr) {
  return new RISCV32MIRevising(mr);
}

#ifdef __cplusplus
}
#endif
