import sys
import locale
import time
import os
import datetime
import decimal

ts = time.time()
timestamp = datetime.datetime.fromtimestamp(ts).strftime('%H:%M:%S')

SEARCH_LINE = "=== PACKET SIZE v DURATION ==="
TRANSMISSION_SEARCH = "TRANSMISSION TIME"
BACKOFF_SEARCH = "BACK OFF TIME"
FOLDER_NAME = "./Simulation_%s/" % (timestamp)
os.mkdir(FOLDER_NAME)

args = sys.argv
lastLine = ""
size_duration_agg = {}
duration_list_agg = []

aggrFilename = ""
notAggrFilename = ""
folderToOpen = args[1]

transmissionTimeAccum_agg = 0
transmissionCount_agg = 0
backoffAccum_agg = 0
backoffCount_agg = 0
transmissionTimeAccum_non_agg = 0
transmissionCount_non_agg = 0
backoffAccum_non_agg = 0
backoffCount_non_agg = 0

for eachfile in os.listdir(folderToOpen):
	if "Not_Aggregated" in eachfile:
		notAggrFilename = folderToOpen + "/" + eachfile
	else:
		aggrFilename = folderToOpen + "/" + eachfile
	if aggrFilename != "" and notAggrFilename != "":
		print ("Set up complete, beginning program")
		break

######## PACKET DELAYS MEASUREMENTS ########

if not "filesearcher.py" in aggrFilename:
	print ("Agg filename: " + aggrFilename)
	print ("Non filename: " + notAggrFilename)
	searchfile = open(aggrFilename, "r")
	print (aggrFilename + ":")
	for line in searchfile:
		if SEARCH_LINE in lastLine:
			newLine = line.strip().split(' ')
			if (len(newLine) > 4):
				duration = (newLine[3].replace(',', ""))
				size = (newLine[1].replace(',', ""))
				if (not size in size_duration_agg):
					duration_list_agg.append(duration)
					size_duration_agg[size] = duration_list_agg
					duration_list_agg = []
				else:
					if (not duration in size_duration_agg[size]): 
						size_duration_agg[size].append(duration)
		elif TRANSMISSION_SEARCH in lastLine:
			newLine = line.strip().split(' ')
			if (len(newLine) > 3):
				transmissionTimeAccum_agg = transmissionTimeAccum_agg + float(newLine[2])
				transmissionCount_agg = transmissionCount_agg + 1
		elif BACKOFF_SEARCH in lastLine:
			newLine = line.strip().split(' ')
			if (len(newLine) > 3):
				backoffAccum_agg = backoffAccum_agg + float(newLine[2])
				backoffCount_agg = backoffCount_agg + 1
		lastLine = line
print ("Finished reading in aggregated file.")

nonAggCount = 0
aggCount = 0
sameCount = 0
sizes_encountered_nonagg = {}
duration_list_nonagg = []
lastLine = ""
searchfile = open(notAggrFilename, "r")
delay_file = open(FOLDER_NAME + "PACKET_DELAY_OUTPUTS.txt", 'w')
for line in searchfile:
	if SEARCH_LINE in lastLine:
		newLine = line.strip().split(' ')
		if (len(newLine) > 4):
			duration = (newLine[3].replace(',', ""))
			size = (newLine[1].replace(',', ""))
			if (size in size_duration_agg):
				if (size in sizes_encountered_nonagg):
					if (not duration in sizes_encountered_nonagg[size]): 
						sizes_encountered_nonagg[size].append(duration)
				else:
					duration_list_nonagg.append(duration)
					sizes_encountered_nonagg[size] = duration_list_nonagg
					duration_list_nonagg = []
				delay_file.write("\n== AGGREGATED ALGORITHM ==")
				delay_file.write ("\nSize: %s bytes" % size)
				delay_file.write ("\nDuration(s): " + str(size_duration_agg[size]) + " (microseconds)")
				acc = 0
				listOfDurations = size_duration_agg[size]
				for x in listOfDurations:
					x = float(x)
					acc = acc + x
				average_agg = acc / len(listOfDurations)
				delay_file.write ("\nAverage duration: %f" % average_agg)

				delay_file.write ("\n== NON AGGREGATED ALGORITHM ==")
				delay_file.write ("\nSize: %s bytes" % size)
				delay_file.write ("\nDuration(s): " + str(sizes_encountered_nonagg[size]))
				acc = 0
				listOfDurations = sizes_encountered_nonagg[size]
				for x in listOfDurations:
					x = float(x)
					acc = acc + x
				average_nonagg = acc / len(listOfDurations)
				delay_file.write ("\nAverage duration: %f" % average_nonagg)

				if (average_nonagg < average_agg):
					nonAggCount = nonAggCount + 1
					delay_file.write ("\nNon-aggregated performed better")
				elif (average_nonagg > average_agg):
					aggCount = aggCount + 1
					delay_file.write ("\nAggregated algorithm performed better")
				else:
					sameCount = sameCount + 1
					delay_file.write ("\nBoth performed the same")
				delay_file.write ("\n\n")
	elif TRANSMISSION_SEARCH in lastLine:
		newLine = line.strip().split(' ')
		if (len(newLine) > 3):
			transmissionTimeAccum_non_agg = transmissionTimeAccum_non_agg + float(newLine[2])
			transmissionCount_non_agg = transmissionCount_non_agg + 1
	elif BACKOFF_SEARCH in lastLine:
		newLine = line.strip().split(' ')
		if (len(newLine) > 3):
			backoffAccum_non_agg = backoffAccum_non_agg + float(newLine[2])
			backoffCount_non_agg = backoffCount_non_agg + 1
	lastLine = line

total = nonAggCount + sameCount + aggCount
nonAggPercent = '{:.1%}'.format(float(nonAggCount)/float(total))
AggPercent = '{:.1%}'.format(float(aggCount)/float(total))
samePercent = '{:.1%}'.format(float(sameCount)/float(total))

delay_file.write ("\nSUMMARY:")
delay_file.write ("\n- Aggregated score: %d, Non-aggregated score: %d, Similar performance score: %d." % (aggCount, nonAggCount, sameCount))
delay_file.write ("\n- Non aggregated performed better %s of the time.\n- Both aggregated and non aggregated performed the same %s of the time." % (nonAggPercent, samePercent))
delay_file.close()
print ("Finished reading in non aggregated file.")
print ("Created delay file.")

## GENERAL STAT EXTRACTION ##
print ("Creating general statistics file...")
genstats_file = open(FOLDER_NAME + "GENERAL_STATISTICS.txt", 'w')

THROUGHTPUT_SEARCH = "The current average is:"
throughput_found = False
throughput_agg = 0
throughput_non_agg = 0

MEDIUM_ACCESS_SEARCH = "Total medium contentions:"
medium_access_found = False
medium_access_agg = 0
medium_access_non_agg = 0

DATA_PROCESSED_SEARCH = "Total data processed by server:"
data_processed_found = False
data_processed_agg = 0
data_processed_non_agg = 0

PACKETS_GEN_SEARCH = "Total count of packets generated:"
packets_gen_found = False
packets_gen_agg = 0
packets_gen_non_agg = 0

PACKETS_RECVD_SEARCH = "Total packets received:"
packets_recvd_found = False
packets_recvd_agg = 0
packets_recvd_non_agg = 0

ARRIVAL_TIME_SEARCH = "Arrival Time:"
arrival_found = False
arrival_agg = 0
arrival_non_agg = 0

PACKETS_PROC_SEARCH = "Packets processed:"
packets_proc_found = False
packets_proc_agg = 0

for line in reversed(open(aggrFilename).readlines()):
	newLine = line.strip()
	if (THROUGHTPUT_SEARCH in newLine and (not throughput_found)):
		line_array = newLine.split(":")
		throughput_agg = int(line_array[1])
		throughput_found = True
	elif MEDIUM_ACCESS_SEARCH in newLine and not medium_access_found:
		line_array = newLine.split(":")
		medium_access_agg = int(line_array[1])
		medium_access_found = True
	elif DATA_PROCESSED_SEARCH in newLine and not data_processed_found:
		line_array = newLine.split(":")
		data_processed_agg = int(line_array[1])
		data_processed_found = True
	elif PACKETS_GEN_SEARCH in newLine and not packets_gen_found:
		line_array = newLine.split(":")
		packets_gen_agg = int(line_array[1])
		packets_gen_found = True
	elif PACKETS_RECVD_SEARCH in newLine and not packets_recvd_found:
		line_array = newLine.split(":")
		packets_recvd_agg = int(line_array[1])
		packets_recvd_found = True
	elif PACKETS_PROC_SEARCH in newLine and not packets_proc_found:
		line_array = newLine.split(":")
		packets_proc_agg = int(line_array[1])
		packets_proc_found = True
	elif ARRIVAL_TIME_SEARCH in newLine and not arrival_found:
		line_array = newLine.split(" ")
		arrival_agg = float(line_array[4].replace(',', ""))
		arrival_found = True
	if (throughput_found and medium_access_found and data_processed_found and packets_gen_found and packets_recvd_found and packets_proc_found and arrival_found):
		break

print ("Finished reading in aggregated. Doing non aggregated...")
throughput_found = False
medium_access_found = False
data_processed_found = False
packets_gen_found = False
packets_recvd_found = False
packets_proc_found = False
arrival_found = False

for line in reversed(open(notAggrFilename).readlines()):
	newLine = line.strip()
	if THROUGHTPUT_SEARCH in newLine and not throughput_found:
		line_array = newLine.split(":")
		throughput_non_agg = int(line_array[1])
		throughput_found = True
	elif MEDIUM_ACCESS_SEARCH in newLine and not medium_access_found:
		line_array = newLine.split(":")
		medium_access_non_agg = int(line_array[1])
		medium_access_found = True
	elif DATA_PROCESSED_SEARCH in newLine and not data_processed_found:
		line_array = newLine.split(":")
		data_processed_non_agg = int(line_array[1])
		data_processed_found = True
	elif PACKETS_GEN_SEARCH in newLine and not packets_gen_found:
		line_array = newLine.split(":")
		packets_gen_non_agg = int(line_array[1])
		packets_gen_found = True
	elif PACKETS_RECVD_SEARCH in newLine and not packets_recvd_found:
		line_array = newLine.split(":")
		packets_recvd_non_agg = int(line_array[1])
		packets_recvd_found = True
	elif ARRIVAL_TIME_SEARCH in newLine and not arrival_found:
		line_array = newLine.split(" ")
		arrival_non_agg = float(line_array[4].replace(',', ""))
		arrival_found = True

	if (throughput_found and medium_access_found and data_processed_found and packets_gen_found and packets_recvd_found and packets_proc_found and arrival_found):
		break
print ("Finished reading in non aggregated. Writing final data...")

#medium utilisation calcs
average_transmission_agg = (transmissionTimeAccum_agg / transmissionCount_agg)
average_transmission_non_agg = (transmissionTimeAccum_non_agg / transmissionCount_non_agg)
average_backoff_agg = (backoffAccum_agg / backoffCount_agg)
average_backoff_non_agg = (backoffAccum_non_agg / backoffCount_non_agg)

aggMediumUtilisation = '{:.1%}'.format(average_transmission_agg/(average_transmission_agg + average_backoff_agg))
nonAggMediumUtilisation = '{:.1%}'.format(average_transmission_non_agg/(average_transmission_non_agg + average_backoff_non_agg))

locale.setlocale(locale.LC_ALL, 'en_US')
decDuration = decimal.Decimal(arrival_agg)
roundedOff = round(decDuration)
genstats_file.write("\nThis simulation ran for %f seconds.\n" % roundedOff)

genstats_file.write("\n== THROUGHPUT ==")
genstats_file.write("\nAggregated algorithm: (Average packets received in 20 seconds) " + (locale.format("%d", throughput_agg, grouping=True)))
genstats_file.write("\nNon aggregated algorithm: (Average packets received in 20 seconds) " + (locale.format("%d", throughput_non_agg, grouping=True)))
genstats_file.write("\n")

genstats_file.write("\n== MEDIUM ACCESS ATTEMPTS ==")
genstats_file.write("\nAggregated algorithm: " + (locale.format("%d", medium_access_agg, grouping=True)))
genstats_file.write("\nNon aggregated algorithm: " + (locale.format("%d", medium_access_non_agg, grouping=True)))
surplus = int(medium_access_non_agg) - int(medium_access_agg)
genstats_file.write("\nNon aggregated algorithm contended " + (locale.format("%d", surplus, grouping=True)) + " more times.")
genstats_file.write("\n")

genstats_file.write("\n== TOTAL DATA PROCESSED BY SERVER ==")
genstats_file.write("\nAggregated algorithm: " + (locale.format("%d", data_processed_agg, grouping=True)) + " bytes")
genstats_file.write("\nNon aggregated algorithm: " + (locale.format("%d", data_processed_non_agg, grouping=True)) + " bytes")
surplus = int(data_processed_non_agg) - int(data_processed_agg)
genstats_file.write("\nNon aggregated algorithm made servers process " + (locale.format("%d", surplus, grouping=True)) + " more bytes.")
packetRecvDiff = packets_recvd_non_agg - packets_proc_agg
genstats_file.write("\nThis means that there was " + (locale.format("%d", packetRecvDiff, grouping=True)) + " more packets in the non aggregated network.")
genstats_file.write("\n")

genstats_file.write("\n== PACKET GENERATION & RECEIPT ==")
genstats_file.write("\nAggregated algorithm: %d packets were generated and %d packets were received" % (packets_gen_agg, packets_recvd_agg))
genstats_file.write("\nNon aggregated algorithm: %d packets were generated and %d packets were received" % (packets_gen_non_agg, packets_recvd_non_agg))
genstats_file.write("\n")

genstats_file.write("\n== MEDIUM UTILISATION ==")
genstats_file.write ("\nAggregated medium utilisation: %s" % aggMediumUtilisation)
genstats_file.write ("\nNon Aggregated medium utilisation: %s" % nonAggMediumUtilisation)
genstats_file.write("\n")

print ("Done!")
