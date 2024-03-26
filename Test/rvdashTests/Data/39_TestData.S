# 39 Test: and


.global _start

_start: addi a1, x0, -1
        addi a3, x0, 37
        and a3, a3, a1 # a3 <- 0x25
        ebreak

