#!/usr/local/bin/python

import os
import pandas as pd

geant4 = "geant4.10.02"
execdir = "/g/g20/lenardo1/Simulations/"
datadir = "/g/g20/lenardo1/Simulations/FitResults_2-14-2019/"
executable = "/g/g20/lenardo1/Simulations/FitYieldMCMCParallel"
basename = "Fits_2-14-2019_"

# Here are the different settings for the fits
offsets = ['0mm','2p5mm','5mm']
datasets = {'Dataset':['100','101','102','103'],\
            'ELife':['10000.','426.','158.','144.'],\
            'ELifeLo':['10000.','174.','108.','96.'],\
            'ELifeHi':['10000.','838.','308.','296.'],\
            'ELifeAvg':['10000.','232.','232.','232.'],\
            'ELifeAvgUp':['10000.','526.','526.','526.'],\
            'ELifeAvgLo':['10000.','150.','150.','150.'],\
            'DriftSpeed':['3.','1.546384','1.883312','2.283947']}
df_datasets = pd.DataFrame(data=datasets)
print(df_datasets)

eee = 0.955
eeelo = 0.938
eeehi = 0.969

d = {'Channel':['11','12','13','14','9','10','5','6'],\
     'Low':['8.','6.','4.','2.','2.','6.','5.','10.'],\
     'High':['60.','50.','40.','40.','40.','60.','50.','80.']}

df_ch = pd.DataFrame(data=d)
print(df_ch)


i=0

for offset in offsets:
	#if False:
	if ('2p5mm' not in offset):
		print('Skipping offset {}'.format(offset))
		continue
	for datidx,datrow in df_datasets.iterrows():
		#if False:
		if ('100' in datrow['Dataset']):
			print('DS: {}....skipping'.format(datrow['Dataset']))
			continue
		for chidx,chrow in df_ch.iterrows():
			if False:
			#if ('5' not in chrow['Channel']): 
				print('Ch {}....skipping...'.format(chrow['Channel']))
				continue
#			print('Channel: {}'.format(chrow['Channel']))
#			print('Dataset: {}'.format(datrow['Dataset']))
#			print('Offset: {}'.format(offset))
	
			scriptfilename = datadir + basename + "_ch_{}".format(chrow['Channel']) + \
					"_ds_{}".format(datrow['Dataset']) + "_OS_{}".format(offset) + ".sub"
			os.system( "rm -f " + scriptfilename )
			outfilename = datadir + basename + "_ch_{}".format(chrow['Channel']) + \
					"_ds_{}".format(datrow['Dataset']) + "_OS_{}".format(offset) + \
					"_ch{}_".format(chrow['Channel']) + "_{}".format(datrow['Dataset']) + ".out"
			os.system( "rm -f " + outfilename )
	
			thescript = "#!/bin/bash\n" + \
				"#SBATCH -t 1:35:00\n" + \
				"#SBATCH -A ared\n" + \
				"#SBATCH -e " + outfilename + "\n" + \
				"#SBATCH -o " + outfilename + "\n" + \
				"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
				"#SBATCH -J " + basename + "\n" + \
				"#SBATCH --export=ALL \n" + \
				"source /usr/gapps/cern/root_v6.12.02/setup \n" + \
				"cd " + execdir + "\n" + \
				"export STARTTIME=`date +%s`\n" + \
				"echo Start time $STARTTIME\n" + \
				executable + " " + chrow['Channel'] + " " +  "1" + \
				" " + chrow['Low'] + " " + chrow['High'] + \
				" " + datrow['Dataset'] + " " + offset + \
				" " + datrow['DriftSpeed'] + " " + datrow['ELifeAvgLo'] + \
				" " + str(eee) + "\n" + \
				"export STOPTIME=`date +%s`\n" + \
				"echo Stop time $STOPTIME\n" + \
				"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
				"echo CPU time: $DT seconds\n"
			scriptfile = open( scriptfilename, 'w' )
			scriptfile.write( thescript )
			scriptfile.close()
	
			os.system( "sbatch " + scriptfilename )

	#"chmod g+rwx *\n" + \
