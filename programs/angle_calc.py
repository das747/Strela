import math
angle=int(input())
M=[45,135,225,315]
for i in M:
    print(math.cos(math.pi*(angle-i)/180))
