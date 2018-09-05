#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

int main() {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("module", context);

    llvm::Function* main = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", module.get());
    llvm::BasicBlock* const current = llvm::BasicBlock::Create(context, "current", main);
    builder.SetInsertPoint(current);
    llvm::Value* a = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "a");
    llvm::Value* b = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "b");
    builder.CreateStore(builder.getInt32(1), a);
    builder.CreateStore(builder.getInt32(1), b);
    llvm::Value* valA = builder.CreateLoad(a);
    llvm::Value* valB = builder.CreateLoad(b);
    auto else_bb = llvm::BasicBlock::Create(context, "else", main);
    auto endif_bb = llvm::BasicBlock::Create(context, "endif", main);

    auto booltmp = builder.CreateICmpNE(valA, builder.getInt32(0), "icmp");
    builder.CreateCondBr(booltmp, endif_bb, else_bb);
    builder.SetInsertPoint(else_bb);
    builder.CreateBr(endif_bb);

    builder.SetInsertPoint(endif_bb);
    auto phi_node = builder.CreatePHI(builder.getInt32Ty(), 2, "phi");
    phi_node->addIncoming(valA, current);
    phi_node->addIncoming(valB, else_bb);
    builder.CreateRet(phi_node);
    llvm::outs() << *module;
}

