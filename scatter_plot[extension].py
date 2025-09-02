# Scatter plot
# Create a script called scatter_plot.[extension] which displays
# a scatter plot answering the following question:
# What are the two features that are similar?

# xxxx-xx-xx
# xx-xx-xxxx
# xx-xx-xxxx
# xx-xx-xx

import sys, csv

def return_type(string):
	try:
		floating_point = float(string)
		if (floating_point):
			return ('floating_point')
	except ValueError:
		if string.isdigit():
			return ('integer')
		if string.isalpha():
			return ('alphabetical')
		if string.isalnum():
			return ('alpha numeric')

with (open(sys.argv[1], 'r')) as dataset:

	readable_file = csv.reader(dataset, delimiter = ',')
	index = 0
	keys = []
	values = []

	for line in readable_file:

		for content in line:
			if index == 0:
				keys.append(content)
			else:
				if not content:
					values.append('missing')
				else:
					values.append(return_type(content))

		index += 1
		if index > 1:
			break

for item in keys:
	print(item)

print('-------')

for item in values:
	print(item, ' : ', type(item))

# leggi la prima riga
# aggiorna chiavi del dizionario
# leggi la seconda riga
# accoppia chiavi:valori