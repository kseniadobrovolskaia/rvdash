# 37 Test: xor


.global _start

_start: addi a1, x0, 15
        addi a3, x0, 993
        xor a3, a1, a3  # a3 <- 0x3ee
        ebreak

