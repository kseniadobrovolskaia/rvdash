# 34 Test: jal


.global _start

_start: jal  x0, .L2
        addi x1, x0, 0x13

.L2:    ebreak

