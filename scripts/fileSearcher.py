import sys

SEARCH_LINE = "=== PACKET SIZE v DURATION ==="
args = sys.argv
lastLine = ""
size_duration_agg = {}
duration_list_agg = []
aggrFilename = args[1]



notAggrFilename = args[2]

if not "filesearcher.py" in aggrFilename:
	print ("filename: " + aggrFilename)
	searchfile = open(aggrFilename, "r")
	print (aggrFilename + ":")
	for line in searchfile:
		if SEARCH_LINE in lastLine:
			newLine = line.strip().split(' ')
			if (len(newLine) > 4):
				duration = float(newLine[3].replace(',', ""))
				size = int(newLine[1].replace(',', ""))
				if (not size in size_duration_agg):
					duration_list_agg.append(duration)
					size_duration_agg[size] = duration_list_agg
					duration_list_agg = []	
				else:
					if (not duration in size_duration_agg[size]): 
						size_duration_agg[size].append(duration)
		lastLine = line


sizes_encountered_nonagg = {}
duration_list_nonagg = []
lastLine = ""
searchfile = open(notAggrFilename, "r")
for line in searchfile:
	if SEARCH_LINE in lastLine:
		newLine = line.strip().split(' ')
		if (len(newLine) > 4):
			duration = float(newLine[3].replace(',', ""))
			size = float(newLine[1].replace(',', ""))
			if (size in size_duration_agg):
				if (size in sizes_encountered_nonagg):
					if (not duration in sizes_encountered_nonagg[size]): 
						sizes_encountered_nonagg[size].append(duration)
				else:
					duration_list_nonagg.append(duration)
					sizes_encountered_nonagg[size] = duration_list_nonagg
					duration_list_nonagg = []
				print ("== AGGREGATED ALGORITHM ==")
				print ("Size: %d bytes" % size)
				print ("Duration(s): " + str(size_duration_agg[size]) + " (microseconds)")
				acc = 0
				listOfDurations = size_duration_agg[size]
				for x in listOfDurations:
					acc = acc + x
				average = acc / len(listOfDurations)
				print ("Average duration: %f" % average)

				print ("== NON AGGREGATED ALGORITHM ==")
				print ("Size: %d bytes" % size)
				print ("Duration(s): " + str(sizes_encountered_nonagg[size]))
				acc = 0
				listOfDurations = sizes_encountered_nonagg[size]
				for x in listOfDurations:
					acc = acc + x
				average = acc / len(listOfDurations)
				print ("Average duration: %f" % average)
				print ("\n\n")
	lastLine = line


