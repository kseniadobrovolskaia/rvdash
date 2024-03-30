# 9 Test: slti


.global _start

_start: addi a1, x0, -18
        slti a3, a1, 2  # a3 <- 0x1
        ebreak
