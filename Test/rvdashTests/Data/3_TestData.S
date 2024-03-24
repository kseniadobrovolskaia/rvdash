# 3 Test: xori


.global _start

_start: addi a1, x0, 15
        xori a3, a1, 993  # a3 <- 0x3ee
        ebreak

