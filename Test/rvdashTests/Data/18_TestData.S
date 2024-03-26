# 18 Test: sh


.global _start

_start: lui a7, 0x10
        lui x2, 0x1
        addi x8, x2, 0x135
        sh x8, 0x12(a7)
        ebreak
