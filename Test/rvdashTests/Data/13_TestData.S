# 13 Test: lw


.global _start

_start: lw a1, 0x0(x0) # a1 <- 0x2583
        ebreak

