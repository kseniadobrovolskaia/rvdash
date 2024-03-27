# 3 Test: Misaligned jalr


.global _start

_start: addi x1, x0, 7
        jalr x2, 0x0(x1)
        addi x1, x0, 0x13

.L2:    ebreak

