# 16 Test: sw


.global _start

_start: addi a7, x0, 0x210
        sw a7, 0x0(a7)
        ebreak
