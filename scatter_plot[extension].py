import sys, csv

titles = []

integers = []
floating_points = []
strings = []

row_index = 0
column_index = 0

with (open(sys.argv[1], 'r')) as dataset:
	readable_file = csv.reader(dataset)
	for i in readable_file:
		if row_index == 0:
			for j in i:
				titles.append(j)
				column_index += 1
			row_index += 1
			continue
		for j in i:
			# try with integers
			# print(j)
			if j.isdigit():
				integers.append(j)
			else:
				# try floating points
				try:
					converted = round(float(j), 2)
					floating_points.append(converted)
				# update strings
				except:
					strings.append(j)
		if (row_index == 1):
			break

print('titles:', titles)

print('integers: \t\t', len(integers), integers)
print('floating_points: \t', len(floating_points), floating_points)
print('strings: \t\t', len(strings), strings)

for i in strings:
	i.split('-')

if (len(integers) == 2):
	print('integers is a couple')
if (len(floating_points) == 2):
	print('floating_points is a couple')
if (len(strings) == 2):
	print('strings is a couple')