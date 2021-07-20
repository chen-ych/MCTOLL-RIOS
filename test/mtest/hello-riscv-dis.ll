; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv"

define void @main(i32 %arg.1, i32 %arg.2) {
EntryBlock:
  %stack.4 = alloca i32, align 4
  %stack.5 = alloca i32, align 4
  %stack.6 = alloca i32, align 4
  %stack.7 = alloca i32, align 4
  %0 = alloca i1, align 32
  store i1 false, i1* %0, align 1
  %1 = alloca i1, align 32
  store i1 false, i1* %1, align 1
  %2 = alloca i1, align 32
  store i1 false, i1* %2, align 1
  %3 = alloca i1, align 32
  store i1 false, i1* %3, align 1
  %4 = load i32, i32* %stack.4, align 2
  %5 = load i32, i32* %stack.5, align 2
  br label %6

6:                                                ; preds = %EntryBlock
  phi void 
  ret void <badref>
}
