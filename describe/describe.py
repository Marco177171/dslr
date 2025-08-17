import csv

# print numerical features
with open('datasets/dataset_test.csv', 'r') as dataset: # open csv dataset
	readable_file = csv.reader(dataset, delimiter=',')
	for row in readable_file:
		for column_value in row:
			# if (isinstance(column_value, float)):
			print(column_value)