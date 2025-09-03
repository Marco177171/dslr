import sys, csv, copy, math, pprint

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
	'stats' : None
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
				except:
					index += 1
					continue
				index += 1

for i in features:
	print(i['name'], end='\t')
print('\n')

print('\nCount: ', end='\t')
for i in features:
	print(round(i['stats']['count'], 2), end='\t')

print('\nMean: ', end='\t')
for i in features:
	print(round(i['stats']['mean'], 2), end='\t')

print('\nSTD: ', end='\t')
for i in features:
	print(round(i['stats']['std'], 2), end='\t')

print('\nMin: ', end='\t')
for i in features:
	print(round(i['stats']['min'], 2), end='\t')

print('\n25%: ', end='\t')
for i in features:
	print(round(i['stats']['25%'], 2), end='\t')

print('\n50%: ', end='\t')
for i in features:
	print(round(i['stats']['50%'], 2), end='\t')

print('\n75%: ', end='\t')
for i in features:
	print(round(i['stats']['75%'], 2), end='\t')

print('\nMax: ', end='\t')
for i in features:
	print(round(i['stats']['max'], 2), end='\t')

print('\n')