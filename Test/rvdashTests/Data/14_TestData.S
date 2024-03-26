# 14 Test: lbu


.global _start

_start: lbu a1, 0x1(x0) # a1 <- 0x45
        ebreak

