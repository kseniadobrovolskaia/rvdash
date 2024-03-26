# 41 Test: srl


.global _start

_start: addi a1, x0, -20
        addi a3, x0, 21
        srl a3, a1, a3  # a3 <- 0x7ff
        ebreak

