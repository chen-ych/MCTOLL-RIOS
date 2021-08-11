; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv"

define void @main(i32 %arg.1, i32 %arg.2) {
EntryBlock:
  %stack.4 = alloca i32, align 4
  %stack.5 = alloca i32, align 4
  %stack.6 = alloca i32, align 4
  %0 = alloca i1, align 32
  store i1 false, i1* %0, align 1
  %1 = alloca i1, align 32
  store i1 false, i1* %1, align 1
  %2 = alloca i1, align 32
  store i1 false, i1* %2, align 1
  %3 = alloca i1, align 32
  store i1 false, i1* %3, align 1
  %4 = add i32 %arg.1, 0
  %5 = add i32 %arg.2, 0
  store i32 %4, i32* %stack.5, align 2
  store i32 %5, i32* %stack.6, align 2
  %6 = add i32 3, 0
  store i32 %6, i32* %stack.4, align 2
  %7 = load i32, i32* %stack.4, align 2
  %8 = add i32 %7, 6
  store i32 %8, i32* %stack.4, align 2
  %9 = add i32 0, 0
  %10 = add i32 %9, 0
  br label %11

11:                                               ; preds = %EntryBlock
  phi void 
  ret void <badref>
}
