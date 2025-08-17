import csv

# print numerical features
count = 0
with open('datasets/dataset_test.csv', 'r') as dataset: # open csv dataset
	readable_file = csv.reader(dataset, delimiter=',')
	for row in readable_file:
		count += 1
		column_index = 0
		for column_value in row:
			column_index += 1
			try:
				converted = float(column_value)
			except ValueError:
				print(column_index, '\t: ' ,"Not a Number")
				continue
			print(column_index, '\t: ', converted)
		column_index = 0
print('Count: ', count - 1)