# 36 Test: add


.global _start

_start: addi a1, x0, 10
        addi a2, x0, -5 
        add a0, a1, a2 # a0 <- 0x5
        ebreak
