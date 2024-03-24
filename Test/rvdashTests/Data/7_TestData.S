# 7 Test: srli


.global _start

_start: addi a1, x0, -20
        srli a3, a1, 21  # a3 <- 0x7ff
        ebreak

