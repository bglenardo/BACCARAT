#!/usr/local/bin/python

import os

datadir = '/p/lustre1/lenardo1/simulations/KTonXe/ElectronGammaPositionDiscrimination/HighStats_20250928/'

execdir = "/g/g20/lenardo1/Simulations/BACCARAT/tools/"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/tools/XeNeuSimpleTrackAnalysisTemplate.C"
base = "Topology_processing_and_reduction_"

files = os.listdir(datadir)

binfiles = [f for f in files if (f.endswith('.root') and not f.endswith('reduced.root'))]

numfiles = len(binfiles)
print('{} bin files to process...'.format(numfiles))

i=0
print(datadir)

for i,f in enumerate(binfiles):
	#print(f)
	#if i > 1: break
	#print(f)
	basename = base + '{}'.format(i) 
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )

	filetitle = f.split('.')[0]
	 
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 00:30:00\n" + \
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
		"root -l -q \'" + macro + "(\"" + datadir + filetitle + \
		"\",\"" + datadir+filetitle+"_reduced.root\")\'" + "\n" + \
		"cd $OLDPWD \n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )

