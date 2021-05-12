; ModuleID = 'hello'
source_filename = "hello"

@ROConst428 = private constant i32 499999, align 32

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
  br label %7

7:                                                ; preds = %EntryBlock
  %8 = load i32, i32* %stack.4, align 2
  %9 = ptrtoint i32* @ROConst428 to i32
  %10 = xor i32 %9, -1
  %11 = add i32 %8, 1
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
  %20 = load i1, i1* %0, align 1
  %21 = load i1, i1* %1, align 1
  %22 = load i1, i1* %3, align 1
  %23 = icmp eq i1 %21, true
  %24 = icmp ne i1 %20, %22
  %25 = xor i1 %23, %24
  br i1 %25, label %26, label %31

26:                                               ; preds = %7
  %27 = add i32 1, 0
  %28 = call i32 @func(i32 %4, i32 %5, i32 %27)
  %29 = load i32, i32* %stack.4, align 2
  %30 = add i32 %29, 1
  store i32 %30, i32* %stack.4, align 2
  br label %34

31:                                               ; preds = %7
  %32 = add i32 0, 0
  %33 = add i32 %32, 0
  br label %34

34:                                               ; preds = %31, %26
  %35 = phi i32 [ %33, %31 ]
  ret i32 %35
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.sadd.with.overflow.i32(i32, i32) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.uadd.with.overflow.i32(i32, i32) #0

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }
