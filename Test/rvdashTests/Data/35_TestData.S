# 35 Test: jalr


.global _start

_start: addi x1, x0, 7
        jalr x2, 0x1(x1)
        addi x1, x0, 0x13

.L2:    ebreak

