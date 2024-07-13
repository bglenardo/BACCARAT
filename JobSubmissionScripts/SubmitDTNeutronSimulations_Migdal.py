#!/usr/local/bin/python

import os
geant4 = "geant4.10.02"
root = "root_v5.34.03"
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/"

macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_10deg_95cmX_Migdal_06.mac"

datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/'
base = "XeNeu_DT_"

for num in range(0,1000):
	#if num > 0: break
	basename = base + str(num)

	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 04:30:00\n" + \
		"#SBATCH -A redxenon\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
                "source ~/.profile.linux \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		"./BACCARATExecutable " + macro + "\n" + \
		"cd " + datadir + "\n" + \
		"chmod g+rwx *\n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )


