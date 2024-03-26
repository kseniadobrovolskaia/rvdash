# 38 Test: or


.global _start

_start: addi a1, x0, 93
        addi a3, x0, 887
        or a3, a3, a1 # a3 <- 0x37f
        ebreak

