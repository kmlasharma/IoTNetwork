import os                                                                                                             

heading = "== MEDIUM UTILISATION =="

base = "/Users/KmlaSharma/Downloads/samples/IoTNetwork/scripts/VARIED_CONGESTION"
searchfor1 = "Aggregated"
searchfor2 = "Non aggregated"
searchGeneral = True
if (searchGeneral):
	subdirs = [x[0] for x in os.walk(base)]
	for eachDir in subdirs:
		for file in os.listdir(eachDir):
			if (file == "GENERAL_STATISTICS.txt"):
				openFile = eachDir + "/" + file
				searchFile = open(openFile, "r")
				lines = searchFile.readlines()
				for x in range(len(lines)):
					if heading in lines[x]:
						# convertagg = lines[x+1].split(' ')
						convertdisagg = lines[x+2].split(' ')
						# print(convertagg[2].strip()) 
						print(convertdisagg[4].strip())
else:
	subdirs = [x[0] for x in os.walk(base)]
	for eachDir in subdirs:
		for file in os.listdir(eachDir):
			if (file == "PACKET_DELAY_OUTPUTS.txt"):
				openFile = eachDir + "/" + file
				searchFile = open(openFile, "r")
				lines = searchFile.readlines()
				for x in range(len(lines)):
					if heading in lines[x]:
						convertdisagg = lines[x+3].split(' ')
						# convertdisagg = lines[x+2].split(' ')
						# print(convertagg[2].strip()) 
						print(convertdisagg[9].strip()[:-1])
						# print("\n")
