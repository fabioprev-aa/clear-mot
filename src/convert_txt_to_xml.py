name = 'test_set_truth'

file = open('../predictions/' + name + '.txt', 'r')
file_xml = open('../predictions/' + name + '.xml', 'w')

file_xml.write('<?xml version="1.0" encoding="utf-8"?>\n<dataset>\n')

counter = 0

for line in file:
	file_xml.write('  <frame number="' + str(counter) + '">\n')
	file_xml.write('    <objectlist>\n')
	
	predictions = line.replace('[','').replace(']','').replace(' ','').split(';')[1:]
	
	for prediction in predictions:
		try:
			#topLeftX, bottomRightX, topLeftY, bottomRightY = prediction.replace('\n','').split(',')
			topLeftX, topLeftY, bottomRightX, bottomRightY, _ = prediction.replace('\n','').split(',')
			
			h = abs(float(topLeftY) - float(bottomRightY))
			w = abs(float(topLeftX) - float(bottomRightX))
			xc = float(topLeftX) + (abs(float(topLeftX) - float(bottomRightX)) / 2)
			yc = float(topLeftY) + (abs(float(topLeftY) - float(bottomRightY)) / 2)
			
			file_xml.write('      <object id="-1">\n')
			file_xml.write('        <box h="' + str(h) + '" w="' + str(w) + '" xc="' + str(xc) + '" yc="' + str(yc) + '"/>\n')
			file_xml.write('      </object>\n')
		except:
			continue
	
	file_xml.write('    </objectlist>\n')
	file_xml.write('  </frame>\n')
	
	counter += 1

file_xml.write('</dataset>')

print('done!')