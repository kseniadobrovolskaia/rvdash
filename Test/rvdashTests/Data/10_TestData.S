# 10 Test: sltiu


.global _start

_start: addi a1, x0, -18
        sltiu a3, a1, 2  # a3 <- 0x0
        ebreak

