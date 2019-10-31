import random

qnt = 100

print(qnt)

for i in range(qnt):

	signal1 = 1 if random.random() > 0.5 else -1;
	signal2 = 1 if random.random() > 0.5 else -1;

	print(f"{signal1*random.random()*100} {signal2*random.random()*100}")