	.text
	.file	"hello-x86"
	.globl	func                            # -- Begin function func
	.p2align	4, 0x90
	.type	func,@function
func:                                   # @func
	.cfi_startproc
# %bb.0:                                # %entry
	movl	%edi, -4(%rsp)
	movl	%esi, -8(%rsp)
	movl	%edx, -20(%rsp)
	movl	$1, %eax
	retq
.Lfunc_end0:
	.size	func, .Lfunc_end0-func
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	%edi, 20(%rsp)
	movq	%rsi, (%rsp)
	movl	$0, 36(%rsp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB1_2
# %bb.1:                                # %bb.1
	movl	$1, %edi
	movl	$1, %esi
	movl	$1, %edx
	callq	func
.LBB1_2:                                # %bb.2
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
