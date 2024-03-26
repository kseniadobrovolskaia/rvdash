# 20 Test: auipc


.global _start

_start: auipc x3, 0x1
        auipc x2, 0x12
        auipc x1, 0x135
        ebreak
