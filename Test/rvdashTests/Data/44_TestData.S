# 44 Test: sltu


.global _start

_start: addi a1, x0, -18
        addi a3, x0, 2
        sltu a3, a1, a3  # a3 <- 0x0
        ebreak

