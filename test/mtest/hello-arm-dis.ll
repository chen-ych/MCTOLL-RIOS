; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm"

define i32 @main(i32 %arg.1, i32 %arg.2) {
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
  %4 = add i32 %arg.1, 0
  %5 = add i32 %arg.2, 0
  store i32 %4, i32* %stack.6, align 2
  store i32 %5, i32* %stack.7, align 2
  %6 = add i32 0, 0
  store i32 %6, i32* %stack.4, align 2
  %7 = add i32 1, 0
  store i32 %7, i32* %stack.5, align 2
  %8 = load i32, i32* %stack.4, align 2
  %9 = load i32, i32* %stack.5, align 2
  %10 = add i32 %8, %9
  store i32 %10, i32* %stack.4, align 2
  %11 = load i32, i32* %stack.4, align 2
  %12 = add i32 %11, 3
  store i32 %12, i32* %stack.4, align 2
  %13 = add i32 %6, 0
  br label %14

14:                                               ; preds = %EntryBlock
  %15 = phi i32 [ %13, %EntryBlock ]
  ret i32 %15
}
