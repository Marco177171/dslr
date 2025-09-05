import sys, csv

def check_if_date(string):
	s = string.split('-')
	if not (len(s) == 3) or not s[2]:
		# print('The string does not represent a date. Exiting...')
		return False
	if (not s[0].isdigit() or len(s[0]) != 4):
		# print('first instance is not a year')
		return False
	elif (not s[1].isdigit() or int(s[1]) < 1 or int(s[1]) > 12):
		# print('ERROR:', s[1], 'is not a valid month')
		return False
	elif (not s[2].isdigit() or int (s[2]) < 1 or int(s[2]) > 31):
		# print('ERROR:', s[2], 'is not a valid month day')
		return False
	print(string, '| represents a proper date')
	return True

def check_if_float(string):
	try:
		converted = round(float(string), 2)
		print(converted, '| represents a floating point number')	
		return True
	except:
		# print('the string does not represent a floating point')
		return False

def check_if_integer(string):
	if string.isdigit():
		print(string, '| the string represents an integer number')
		return True
	else:
		# print('the string does not represent an integer number')
		return False

def check_if_string(string):
	if string.isalpha():
		print(string, '| the string only contains alphabetical characters')
		return True
	else:
		# print('the string does not only contain alphabetical characters')
		return False

with (open(sys.argv[1], 'r')) as file:
	matrix = csv.reader(file)
	for row in matrix:
		for col in row:
			check_if_date(col)
			check_if_float(col)
			check_if_integer(col)
			check_if_string(col)