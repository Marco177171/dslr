import csv

with open('datasets/dataset_test.csv', newline='\n') as csvfile:
	readable_file = csv.reader(csvfile, delimiter=' ', quotechar='|')
	for row in readable_file:
		print(' '.join(row))