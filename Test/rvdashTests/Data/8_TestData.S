# 8 Test: srai


.global _start

_start: addi a1, x0, -18
        srai a3, a1, 8  # a3 <- 0xffffffff
        ebreak

