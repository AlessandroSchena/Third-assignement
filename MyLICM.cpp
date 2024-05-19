#include "llvm/Transforms/Utils/MyLICM.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/ValueTracking.h"


using namespace llvm;

PreservedAnalyses MyLICM::run(Loop &L, LoopAnalysisManager &LAM, LoopStandardAnalysisResults &LAR,
LPMUpdater &LU) {
	outs() << "PRINT LOOP:\n";
	outs() << L.getHeader() << "\n";

	/*for(auto *i = L.block_begin(); i!=L.block_end(); ++i){
		BasicBlock *BB = dyn_cast<BasicBlock>(*i);
		BB->printAsOperand(errs());
		errs() << "\n getName: " << BB->getName().data() << "\n";
		//errs() << BB->getNameOrAsOperand() << "\n";
		errs() << "\nPred:";
		for(BasicBlock *pred : predecessors(BB)){
			errs() << "\n\t";
			pred->printAsOperand(errs());
		}
		errs() << "\n";
	}*/

	DominatorTree &DT = LAR.DT;
	bool isDom = true, isLoopInvariant = false;
	Instruction *lastInstPreHeaderBB = L.getLoopPreheader()->getTerminator();

	errs() << "Loop Header: " << L.getHeader()->getName() << "\n"; 
	errs() << "Loop Depth: " << L.getLoopDepth() << "\n"; 
	errs() << "Number of Blocks: " << L.getNumBlocks() << "\n"; errs() << "Loop Backedge: " << L.getLoopLatch()->getName() << "\n";
	
	//DT.print(errs());
	BasicBlock *ExitBB = L.getExitBlock();

	for(auto *BBLoop = L.block_begin(); BBLoop!=L.block_end(); ++BBLoop){
		BasicBlock *BB = dyn_cast<BasicBlock>(*BBLoop);
		(*BB).printAsOperand(errs());
		errs() << "\n";
		BasicBlockEdge *BBEdge = new BasicBlockEdge(BB, BB);
		for(auto i = (*BB).begin(); i!=(*BB).end();++i){
			Instruction *inst = dyn_cast<Instruction>(i);
			Value *V = dyn_cast<Value>(i);
			errs() << "\t" << (*V) << "\n";
			if(L.isLoopInvariant(V)){
				errs() << "\t\t is invariant\n";
				isLoopInvariant = true;
			}
			if(DT.dominates(*BBEdge, ExitBB)){
				errs() << "\t\t is dom\n";
				for(auto k=(*V).user_begin(); k!=(*V).user_end(); k++){
					if(!DT.dominates(V, dyn_cast<Instruction>(*k))){
						isDom = false;

					}
				}
				if(isDom && isLoopInvariant){
					errs() << "\t\t is dominator\n";
					(*inst).moveAfter(lastInstPreHeaderBB);
				}
			}
		}
	}
	return PreservedAnalyses::all();
}
