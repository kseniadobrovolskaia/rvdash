# 45 Test: nop


.global _start

_start: addi x0, x0, 0 # == nop
        ebreak

