import sys, csv, copy, math

statistics = {
	'count': 0,
	'mean': 0,
	'std': 0,
	'min': 0,
	'25%': 0,
	'50%': 0,
	'75%': 0,
	'max': 0,
}

feature = {
	'name' : None,
	'stats' : None,
}

with open(sys.argv[1], 'r') as dataset:
	readable = csv.reader(dataset)
	features_set = False
	for line in readable:
		if not features_set:
			features = []
			for name in line:
				new_feature = {
					'name': name,
					'stats': copy.deepcopy(statistics)
				}
				features.append(new_feature)
			features_set = True
		else:
			index = 0
			for value in line:
				try:
					converted = float(value)
				except:
					index += 1
					continue
				features[index]['stats']['count'] += 1
				features[index]['stats']['mean'] = features[index]['stats']['mean'] + converted / features[index]['stats']['count']
				features[index]['stats']['std'] = math.sqrt(pow(converted - features[index]['stats']['std'], 2) / features[index]['stats']['count'])
				if converted < features[index]['stats']['min']:
					features[index]['stats']['min'] = converted
				features[index]['stats']['25%'] = (features[index]['stats']['max'] - features[index]['stats']['min']) / 100 * 25 + features[index]['stats']['min']
				features[index]['stats']['50%'] = (features[index]['stats']['max'] - features[index]['stats']['min']) / 100 * 50 + features[index]['stats']['min']
				features[index]['stats']['75%'] = (features[index]['stats']['max'] - features[index]['stats']['min']) / 100 * 75 + features[index]['stats']['min']
				if converted > features[index]['stats']['max']:
					features[index]['stats']['max'] = converted
				index += 1

for i in features:
	if (i['stats']['count'] != 0):
		print(i['name'], len(i['name']), end='\t|')

print('\nCount: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['count'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\nMean: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['mean'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\nSTD: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['std'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\nMin: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['min'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\n25%: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['25%'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\n50%: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['50%'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\n75%: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['75%'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\nMax: ', end='\t|')
for i in features:
	try:
		cipher = float(i['stats']['max'])
		if (cipher != 0):
			print(round(cipher, 2), end='\t|')
	except:
		continue

print('\n')