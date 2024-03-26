# 2 Test: addi


.global _start

_start: addi a1, x0, 10
        addi a0, a1, -5 # a0 <- 0x5
        ebreak
