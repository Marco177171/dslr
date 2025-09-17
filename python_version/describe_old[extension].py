import csv, math, sys

class Values:
	mean = 0.0
	standard_deviation = 0.0
	min = 0.0
	twentyfive = 0.0
	fifty = 0.0
	seventyfive = 0.0
	max = 0.0
	count = 0.0
	def __init__(self):
		return None

class Table():
	def __init__(self, name):
		self.name = name
		self.values = Values()

def update_statistics(table, converted):
	table.values.count += 1
	table.values.mean = table.values.mean + converted / table.values.count
	table.values.standard_deviation = math.sqrt(pow(converted - table.values.standard_deviation, 2) / table.values.count)
	if table.values.min > converted or table.values.min == 0.0:
		table.values.min = converted
	if table.values.max < converted or table.values.max == 0.0:
		table.values.max = converted
	table.values.twentyfive = (table.values.max - table.values.min) / 100 * 25 + table.values.min
	table.values.fifty = (table.values.max - table.values.min) / 100 * 50 + table.values.min
	table.values.seventyfive = (table.values.max - table.values.min) / 100 * 75 + table.values.min

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

print('mean : \t')
print('std: \t')
print('min: \t')
print('25%: \t')
print('50%: \t')
print('75%: \t')
print('max: \t')

for table in Tables:
	if (float(table.values.mean)):
		print(table.name)
		print('mean : \t', round(table.values.mean, 6))
		print('std: \t', round(table.values.standard_deviation, 6))
		print('min: \t', round(table.values.min, 6))
		print('25%: \t', round(table.values.twentyfive, 6))
		print('50%: \t', round(table.values.fifty, 6))
		print('75%: \t', round(table.values.seventyfive, 6))
		print('max: \t', round(table.values.max, 6), '\n')