#ifndef LLVM_TRANSFORMS_MYLICM_H
#define LLVM_TRANSFORMS_MYLICM_H
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
namespace llvm {
    class MyLICM : public PassInfoMixin<MyLICM> {
    public:
        PreservedAnalyses run(Loop &L, LoopAnalysisManager &LAM,LoopStandardAnalysisResults &LAR,
                              LPMUpdater &LU);
    };
} // namespace llvm
#endif