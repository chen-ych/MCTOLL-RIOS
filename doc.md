# Build
See https://github.com/microsoft/llvm-mctoll

# Develop
First make sure your path is in `/home/ychen/LLVM`\

source code: /home/ychen/LLVM/llvm/tools/llvm-mctoll, after modifying the source code, use `cmake --build build  -- llvm-mctoll` to rebuild the project

other issues see /home/ychen/LLVM/makefile

1. build-xxx, compile
2. objudmp-xxx, disassemble the test binary file
3. test-xxx, use llvm-mctoll to lift the test binary file

# 8 Steps for lifting the binary For ARM
see /home/ychen/LLVM/llvm/tools/llvm-mctoll/ARM/ARMMachineInstructionRaiser.cpp
1. discover function prototype: discover how many arguments, what's their types
2. MIRevising:

    2.1 discover jump instruction `bl, bl_pred, bcc` 's jump target.  
    2.2 In arm, there may exist data in text section like this ![img](../img/693083DE-290B-442F-ACF6-5A6AAD6AB5F3.png), arm use ldr r3, [pc, #20] to load the data, and in this step, it transfer ldr r3, [pc, #20] to ldr r3, @globalvalue, this step is unnecessary for risc-v and x86

3. Eliminate PrologEpilog: when entering a function, we need to modify the sp and fp register, this is called prolog, when leaving a function, we also need to modify the sp and fp register, this is called epilog. In arm, `str fp, [sp, #-4]!` and `add fp, sp, #0` form a prolog, `sub sp, fp, #0` and `ldr fp, [sp], #4` form a epilog, also there exist other prologs and epilogs. In this step, we need to eliminate the prologs and epilogs, and save them in some data structure. 
4. CreateJumpTable, deal with long jump, still unsure in which situation 'add     r0, pc, #8' exists
5. ArgumentRaiser, add serveral machine instruction, move arugment data from stack to registers, like `$r0 = MOVr %stack.1`, `$r1 = MOVr %stack.2`.
6. FrameBuilder, search all stack objects and update them, STRi12 $r0, $r11, -16, 14, $noreg, <0x8b57068> -> STRi12 $r0, %stack.5.stack.5, 14, $noreg, <0x8b57068>.
7. InstructionSplitting, split complex instructions in arm to serveral simple instructions, unnecssary for risc-v
8. SelectionDAGISel, 

    8.1 machine instruction -> SDNode
    
    8.2 SDNode -> LLVM IR 