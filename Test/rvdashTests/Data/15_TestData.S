# 15 Test: lhu


.global _start

_start: lhu a1, 0x1(x0) # a1 <- 0x1055
        ebreak

