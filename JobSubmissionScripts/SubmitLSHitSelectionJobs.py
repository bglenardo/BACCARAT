#!/usr/local/bin/python

import os

execdir = "/g/g20/lenardo1/"

datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/'

executable = "/g/g20/lenardo1/Simulations/BACCARAT/tools/LSDetectorHitSelection"
base = "XeNeu_DDMigdal_Apr_03_2023_"

files = os.listdir(datadir)
rootfiles = [f for f in files if f.endswith('root') and 'ls_hit' not in f]

numfiles = len(rootfiles)
print('{} bin files to process...'.format(numfiles))


i=0

for f in rootfiles:
	#if i > 0: break
	i=i+1
	basename = base + '{}'.format(i) 
#for num in range(1,201):
#for num in range(201,401):
#for num in range(401,601):
#for num in range(601,801):
#for num in range(801,1001):
#for num in range(1001,1201):
#for num in range(1201,1401):
#for num in range(1401,1601):
#for num in range(1601,1801):
#for num in range(1801,2001):
#for num in range(2001,2201):
#for num in range(2201,2401):
#for num in range(2401,2601):
#for num in range(2601,2801):
#for num in range(2801,3001):
	
	scriptfilename = datadir + basename + "LS_Hit.sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + "LS_Hit.out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:03:00\n" + \
		"#SBATCH -A mlodd\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
		"source ~/.profile.linux \n" + \
                "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/g/g20/lenardo1/Simulations/BACCARAT/tools \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		executable + ' ' + datadir + f + " " + datadir + basename + "_ls_hit.root" + "\n" + \
		"cd " + datadir + "\n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )

