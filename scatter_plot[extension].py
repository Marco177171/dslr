import sys, csv

with (open(sys.argv[1], 'r')) as dataset:
	readable_file = csv.reader(dataset)
	index = 0
	matrix = []

	for line in readable_file:
		col = []

		for value in line:
			try:
				converted = float(value)
				col.append(converted)
			except:
				col.append(value)
		matrix.append(col)
		index += 1
		if index > 1:
			break

	for i in matrix:
		for j in i:
			print(j, type(j))

	print(matrix[1][1])