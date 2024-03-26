# 23 Test: beq (don't take the branch)


.global _start

_start: addi x3, x0, 0x1
        addi x2, x0, 0x2
        beq x3, x2, .L2
        addi x1, x0, 0x13

.L2:    ebreak

