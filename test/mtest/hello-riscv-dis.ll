; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv"

define i32 @main(i32 %arg.1, i32 %arg.2) {
EntryBlock:
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
  %6 = add i32 0, 65536
  %7 = add i32 %6, 1180
  %8 = inttoptr i32 %7 to i8*
  %9 = call i32 @puts(i8* %8)
  %10 = add i32 0, 0
  %11 = add i32 %10, 0
  br label %12

12:                                               ; preds = %EntryBlock
  %13 = phi i32 [ %11, %EntryBlock ]
  ret i32 %13
}

declare dso_local i32 @puts(i8*)
