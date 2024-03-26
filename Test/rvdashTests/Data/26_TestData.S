# 26 Test: blt (take the branch)


.global _start

_start: addi x3, x0, -5
        addi x2, x0, -1
        blt x3, x2, .L2
        addi x1, x0, 0x13

.L2:    ebreak

