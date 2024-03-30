# 4 Test: Illegal instruction: 00000000000000000000


.global _start

_start: addi x1, x0, 7
        jal  x0, .L2 + 4

.L2:    ebreak

