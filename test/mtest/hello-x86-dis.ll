; ModuleID = './test/mtest/hello-x86'
source_filename = "./test/mtest/hello-x86"

define dso_local i32 @main(i32 %arg1, i64 %arg2) {
entry:
  %stktop_8 = alloca i8, i32 40, align 1
  %RBP_N.32 = bitcast i8* %stktop_8 to i64*
  %tos = ptrtoint i8* %stktop_8 to i64
  %0 = add i64 %tos, 20
  %RBP_N.20 = inttoptr i64 %0 to i32*
  %1 = add i64 %tos, 36
  %RBP_N.4 = inttoptr i64 %1 to i32*
  %2 = add i64 %tos, 0
  %RSP_P.0 = inttoptr i64 %2 to i64*
  %RBP = ptrtoint i64* %RSP_P.0 to i64
  store i32 %arg1, i32* %RBP_N.20, align 1
  store i64 %arg2, i64* %RBP_N.32, align 1
  store i32 0, i32* %RBP_N.4, align 1
  br label %bb.2

bb.2:                                             ; preds = %bb.1, %entry
  %3 = load i32, i32* %RBP_N.4, align 1
  %4 = zext i32 %3 to i64
  %5 = zext i32 499999 to i64
  %6 = sub i64 %4, %5
  %7 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %4, i64 %5)
  %CF = extractvalue { i64, i1 } %7, 1
  %ZF = icmp eq i64 %6, 0
  %highbit = and i64 -9223372036854775808, %6
  %SF = icmp ne i64 %highbit, 0
  %8 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %4, i64 %5)
  %OF = extractvalue { i64, i1 } %8, 1
  %CmpZF_JLE = icmp eq i1 %ZF, true
  %CmpOF_JLE = icmp ne i1 %SF, %OF
  %ZFOrSF_JLE = or i1 %CmpZF_JLE, %CmpOF_JLE
  br i1 %ZFOrSF_JLE, label %bb.1, label %bb.3

bb.3:                                             ; preds = %bb.2
  ret i32 0

bb.1:                                             ; preds = %bb.2
  %9 = load i32, i32* %RBP_N.4, align 1
  %10 = zext i8 1 to i32
  %11 = add i32 %9, %10
  store i32 %11, i32* %RBP_N.4, align 1
  br label %bb.2
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.usub.with.overflow.i64(i64, i64) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.ssub.with.overflow.i64(i64, i64) #0

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }
