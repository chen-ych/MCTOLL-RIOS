//===-- RISCV32ModuleRaiser.h ---------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of RISCV32ModuleRaiser class for use by
// llvm-mctoll.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_MCTOLL_RISCV32_RISCV32MODULERAISER_H
#define LLVM_TOOLS_LLVM_MCTOLL_RISCV32_RISCV32MODULERAISER_H

#include "ModuleRaiser.h"
#include "MachineFunctionRaiser.h"
using namespace llvm;

class RISCV32ModuleRaiser : public ModuleRaiser {
public:
  static bool classof(const ModuleRaiser *mr) {
    return mr->getArch() == Triple::riscv32;
  }
  RISCV32ModuleRaiser() : ModuleRaiser() { Arch = Triple::riscv32; };

  MachineFunctionRaiser *
  CreateAndAddMachineFunctionRaiser(Function *F, const ModuleRaiser *MR,
                                    uint64_t Start, uint64_t End) override;

   // Method to map syscall.
  void setSyscallMapping(uint64_t idx, Function *fn) { SyscallMap[idx] = fn; }
  
  void fillInstAddrFuncMap(uint64_t callAddr, Function *func) {
    InstAddrFuncMap[callAddr] = func;
  }

  bool collectDynamicRelocations() override;
private:
  // Commonly used data structures for ARM.
  // This is for call instruction. (BL instruction)
  DenseMap<uint64_t, Function *> InstAddrFuncMap;

  // Map index to its corresponding function.
  std::map<uint64_t, Function *> SyscallMap;
};

#endif // LLVM_TOOLS_LLVM_MCTOLL_RISCV32_RISCV32MODULERAISER_H
