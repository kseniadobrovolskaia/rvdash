# 2 Test: Misaligned jal


.global _start

_start: jal  x0, 2
        addi x1, x0, 0x13
        addi x1, x0, 0x13
        ebreak

