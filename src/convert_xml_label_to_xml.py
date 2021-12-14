import glob

name = 'xmlGoldenSet'

files = sorted(glob.glob('../groundtruth/' + name + '/*.xml'))

file_xml = open('../groundtruth/' + name + '_clearmot.xml', 'w')

file_xml.write('<?xml version="1.0" encoding="utf-8"?>\n<dataset>\n')

counter = 0

for file_row in files:
	print('file_row: ', file_row)
	file = open(file_row, 'r')
	
	file_xml.write('  <frame number="' + str(counter) + '">\n')
	file_xml.write('    <objectlist>\n')
	
	for line in file:
		if 'xmin' in line:
			xmin = line.split('>')[1].split('<')[0]
		elif 'ymin' in line:
			ymin = line.split('>')[1].split('<')[0]
		elif 'xmax' in line:
			xmax = line.split('>')[1].split('<')[0]
		elif 'ymax' in line:
			ymax = line.split('>')[1].split('<')[0]
			file_xml.write('      <object id="-1">\n')
			
			h = abs(float(ymin) - float(ymax))
			w = abs(float(xmin) - float(xmax))
			xc = float(xmin) + (abs(float(xmin) - float(xmax)) / 2)
			yc = float(ymin) + (abs(float(ymin) - float(ymax)) / 2)
			
			file_xml.write('        <box h="' + str(h) + '" w="' + str(w) + '" xc="' + str(xc) + '" yc="' + str(yc) + '"/>\n')
			file_xml.write('      </object>\n')
	
	file_xml.write('    </objectlist>\n')
	file_xml.write('  </frame>\n')
	
	counter += 1

file_xml.write('</dataset>')

print('done!')