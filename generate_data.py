from random import *

query_dim = [2, 3, 5, 10, 15, 20]

N=100000
for dim in query_dim:
	f = open("data_" + str(dim) + ".txt","w")
	f.write(str(dim))
	f.write(" ")
	f.write(str(N))
	f.write('\n')
	for i in range(100000):
		for j in range(dim):
			f.write(str(random()) + ' ')
		f.write('\n')

	f.close()

for dim in query_dim:	
	f = open("query_" + str(dim) + ".txt","w")
	f.write(str(dim))
	f.write(" ")
	f.write(str(100))
	f.write('\n')
	for i in range(100):
		for j in range(dim):
			f.write(str(random()) + ' ')
		f.write('\n')

	f.close()

