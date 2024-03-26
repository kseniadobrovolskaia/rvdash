# 28 Test: bge (take the branch)


.global _start

_start: addi x3, x0, -10
        addi x2, x0, -10
        bge x3, x2, .L2
        addi x1, x0, 0x13

.L2:    ebreak

