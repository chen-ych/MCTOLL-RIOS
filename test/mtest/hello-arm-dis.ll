; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-arm"

define i32 @func(i32 %arg.1, i32 %arg.2, i32 %arg.3) {
EntryBlock:
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
  %6 = add i32 %arg.3, 0
  store i32 %4, i32* %stack.5, align 2
  store i32 %5, i32* %stack.6, align 2
  store i32 %6, i32* %stack.7, align 2
  %7 = add i32 1, 0
  %8 = add i32 %7, 0
  br label %9

9:                                                ; preds = %EntryBlock
  %10 = phi i32 [ %8, %EntryBlock ]
  ret i32 %10
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
  %6 = add i32 0, 0
  store i32 %6, i32* %stack.4, align 2
  %7 = load i32, i32* %stack.4, align 2
  %8 = sub i32 %7, %7
  %9 = add i32 %8, 0
  %10 = xor i32 %9, -1
  %11 = add i32 %7, 1
  %12 = call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %10, i32 %11)
  %13 = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 %10, i32 %11)
  %14 = extractvalue { i32, i1 } %12, 0
  %15 = lshr i32 %14, 31
  %16 = trunc i32 %15 to i1
  store i1 %16, i1* %0, align 1
  %17 = icmp eq i32 %14, 0
  store i1 %17, i1* %1, align 1
  %18 = extractvalue { i32, i1 } %12, 1
  store i1 %18, i1* %2, align 1
  %19 = extractvalue { i32, i1 } %13, 1
  store i1 %19, i1* %3, align 1
  %20 = load i1, i1* %1, align 1
  %21 = icmp eq i1 %20, false
  br i1 %21, label %22, label %25

22:                                               ; preds = %EntryBlock
  %23 = add i32 0, 0
  %24 = add i32 %23, 0
  br label %28

25:                                               ; preds = %EntryBlock
  %26 = add i32 1, 0
  %27 = call i32 @func(i32 %4, i32 %5, i32 %26)
  br label %28

28:                                               ; preds = %25, %22
  %29 = phi i32 [ %24, %22 ]
  ret i32 %29
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.sadd.with.overflow.i32(i32, i32) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.uadd.with.overflow.i32(i32, i32) #0

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }
