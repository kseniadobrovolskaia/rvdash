# 12 Test: lh


.global _start

_start: lh a1, 0x0(x0) # a1 <- 0x1583
        ebreak

