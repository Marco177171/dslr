import csv

# Print numerical features
count = 0
with open('datasets/dataset_test.csv', 'r') as dataset: # open csv dataset
	readable_file = csv.reader(dataset, delimiter = ',')
	header_dict = {}
	for row in readable_file:
		count += 1
		print('CYCLING AT : ', count)
		column_index = 0
		for column_value in row:
			if count == 1:
				header_dict[column_index] = column_value
			column_index += 1
			try:
				converted = float(column_value)
			except ValueError:
				print(column_index, ' : ', header_dict[column_index - 1 ], '\t:', "Not a Number")
				continue
			print(column_index, ' : ', header_dict[column_index - 1], '\t:', converted)
		print('\n')

print('count : ', count - 1)
# header_dictionary = {
# 	'Index' : 123,
# 	'Hogwarts House' : 123,
# 	'First Name' : 123,
# 	'Last Name' : 123,
# 	'Borthday' : 123,
# 	'Best Hand' : 123,
# 	'Arithmancy' : 123,
# 	'Astronomy' : 123,
# 	'Defense Against the Dark Arts' : 123,
# 	'Divination' : 123,
# 	'Muggle Studies' : 123,
# 	'Ancient Runes' : 123,
# 	'History of Magic' : 123,
# 	'Transfiguration' : 123,
# 	'Potions' : 123,
# 	'Care of Magical Creatures' : 123,
# 	'Charms' : 123,
# 	'Flying' : 123,
# }