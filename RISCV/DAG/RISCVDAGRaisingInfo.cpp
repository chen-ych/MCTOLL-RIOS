//===- RISCVDAGRaisingInfo.cpp - Binary raiser utility llvm-mctoll -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementaion of RISCVDAGRaisingInfo class for use
// by llvm-mctoll.
//
//===----------------------------------------------------------------------===//

#include "RISCVDAGRaisingInfo.h"

using namespace llvm;

RISCVDAGRaisingInfo::RISCVDAGRaisingInfo(SelectionDAG &dag) : DAG(dag) {}

/// Gets the related IR Value of given SDNode.
Value *RISCVDAGRaisingInfo::getRealValue(SDNode *Node) {
  assert(Node != nullptr && "Node cannot be nullptr!");
  assert(NPMap[Node] != nullptr &&
         "Cannot find the corresponding node proprety!");
  return NPMap[Node]->Val;
}

/// Set the related IR Value to SDNode.
void RISCVDAGRaisingInfo::setRealValue(SDNode *N, Value *V) {
  if (NPMap.count(N) == 0)
    NPMap[N] = new NodePropertyInfo();

  NPMap[N]->Val = V;
}

void RISCVDAGRaisingInfo::clear() {
  for (auto &elmt : NPMap)
    delete elmt.second;

  NPMap.clear();
}
