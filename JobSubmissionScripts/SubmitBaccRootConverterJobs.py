#!/usr/local/bin/python

import os
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/tools"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/"
executable = "/g/g20/lenardo1/Simulations/BACCARAT/tools/BaccRootConverter"
base = "XeNu_DT_rootified_"

files = os.listdir(datadir)
binfiles = [f for f in files if f.endswith('bin')]

numfiles = len(binfiles)
print('{} bin files to process...'.format(numfiles))


i=0

for f in binfiles:
	i=i+1
	basename = base + '{}'.format(i) 
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:15:00\n" + \
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
		executable + ' ' + datadir + f + "\n" + \
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


