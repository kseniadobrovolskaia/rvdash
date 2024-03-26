# 17 Test: sb


.global _start

_start: addi a7, x0, 0x64
        addi x8, x0, 0x81
        sb x8, 0x4(a7)
        ebreak
