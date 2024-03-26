# 42 Test: sra


.global _start

_start: addi a1, x0, -18
        addi a3, x0, 8
        sra a3, a1, a3  # a3 <- 0xffffffff
        ebreak

