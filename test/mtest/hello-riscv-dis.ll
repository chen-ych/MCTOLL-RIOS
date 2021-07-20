; ModuleID = '/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv'
source_filename = "/home/ychen/LLVM/llvm/tools/llvm-mctoll/test/mtest/hello-riscv"

define void @func(i32 %arg.1, i32 %arg.2, i32 %arg.3) {
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
  br label %4

4:                                                ; preds = %EntryBlock
  phi void 
  ret void <badref>
}

define void @main(i32 %arg.1, i32 %arg.2) {
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
  br label %6

4:                                                ; No predecessors!
  br label %6

5:                                                ; No predecessors!
  br label %6

6:                                                ; preds = %5, %4, %EntryBlock
  phi void 
  ret void <badref>
}
