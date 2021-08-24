; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm"

@RO-String = private constant [12 x i8] c"hello world\00"

define i32 @main(i32 %arg.1, i32 %arg.2) {
EntryBlock:
  %stack.4 = alloca i32, align 4
  %stack.5 = alloca i32, align 4
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
  store i32 %4, i32* %stack.4, align 2
  store i32 %5, i32* %stack.5, align 2
  %6 = ptrtoint [12 x i8]* @RO-String to i32
  %7 = inttoptr i32 %6 to i8*
  %8 = call i32 @puts(i8* %7)
  %9 = add i32 0, 0
  %10 = add i32 %9, 0
  br label %11

11:                                               ; preds = %EntryBlock
  %12 = phi i32 [ %10, %EntryBlock ]
  ret i32 %12
}

declare dso_local i32 @puts(i8*)
