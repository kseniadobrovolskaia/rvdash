# 11 Test: lb


.global _start

_start: lb a1, 0x0(x0) # a1 <- 0x0
        ebreak

