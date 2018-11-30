import math
angle=int(input())
M = [325, 35, 35, 325]
sp_ref = 200
for i in M:
    print(math.cos(math.pi * (angle-i)/180) * sp_ref)
