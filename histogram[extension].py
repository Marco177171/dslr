import csv, math, sys

class Values:
	standard_deviation = 0.0
	count = 0.0
	def __init__(self):
		return None

class Table():
	def __init__(self, name):
		self.name = name
		self.values = Values()

def update_statistics(table, converted):
	table.values.count += 1
	table.values.standard_deviation = math.sqrt(pow(converted - table.values.standard_deviation, 2) / table.values.count)

with open(sys.argv[1], 'r') as dataset: # open csv dataset file
	Tables = []
	readable_file = csv.reader(dataset, delimiter = ',')
	row_index = 0
	for row in readable_file:
		column_index = 0
		for column_value in row:
			if row_index == 0:
				Tables.append(Table(column_value)) # genera una tavola per ogni colonna
			try:
				converted = float(column_value)
				update_statistics(Tables[column_index], converted)
			except ValueError:
				column_index += 1
				continue
			column_index += 1
		row_index += 1
		# print_statistics(Tables)

lowest_std_table = Tables[0]
for table in Tables:
	if (float(table.values.standard_deviation)):
		# print(table.name, 'STD:', table.values.standard_deviation)
		if table.values.standard_deviation < lowest_std_table.values.standard_deviation:
			lowest_std_table = table
print('LOWEST STD: ', lowest_std_table.name, 'with a score of', round(lowest_std_table.values.standard_deviation, 6))
