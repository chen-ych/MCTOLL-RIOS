	.text
	.file	"hello-arm"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %EntryBlock
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	andq	$-32, %rsp
	subq	$128, %rsp
	movb	$0, 96(%rsp)
	movb	$0, 64(%rsp)
	movb	$0, 32(%rsp)
	movb	$0, (%rsp)
	movl	%edi, 104(%rsp)
	movl	%esi, 100(%rsp)
	movl	$0, 108(%rsp)
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
