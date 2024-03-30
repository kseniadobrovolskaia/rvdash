# 5 Test: Address exceeds ram size


.global _start

_start: lui x1, 0x1000
        lhu a1, 0x100(x1)
        ebreak

