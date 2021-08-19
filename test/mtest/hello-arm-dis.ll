; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm"

define i32 @fn(i32 %arg.1) {
EntryBlock:
  %stack.3 = alloca i32, align 4
  %0 = alloca i1, align 32
  store i1 false, i1* %0, align 1
  %1 = alloca i1, align 32
  store i1 false, i1* %1, align 1
  %2 = alloca i1, align 32
  store i1 false, i1* %2, align 1
  %3 = alloca i1, align 32
  store i1 false, i1* %3, align 1
  %4 = add i32 %arg.1, 0
  store i32 %4, i32* %stack.3, align 2
  %5 = load i32, i32* %stack.3, align 2
  %6 = add i32 %5, 0
  br label %7

7:                                                ; preds = %EntryBlock
  %8 = phi i32 [ %6, %EntryBlock ]
  ret i32 %8
}

define i32 @main(i32 %arg.1, i32 %arg.2) {
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
  %6 = add i32 5, 0
  %7 = call i32 @fn(i32 %6)
  store i32 %7, i32* %stack.4, align 2
  %8 = load i32, i32* %stack.4, align 2
  %9 = add i32 %8, 0
  br label %10

10:                                               ; preds = %EntryBlock
  %11 = phi i32 [ %9, %EntryBlock ]
  ret i32 %11
}
