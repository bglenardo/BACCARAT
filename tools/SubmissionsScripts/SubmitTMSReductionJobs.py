#!/usr/local/bin/python

import os
datadir = "/p/lustre2/lenardo1/simulations/TMS_DD/Jan16_2p8MeV_neutrons/OtherRoot/"
#datadir = "/p/lscratchh/lenardo1/simulations/AmBe_Xe137_ActivationSims/"
execdir = "/g/g20/lenardo1/Simulations/"
macro = "/g/g20/lenardo1/Simulations/TMSReductionMacroTemplate.C"
base = "TMSMigdal_Feb_21_2021_processing_"

files = os.listdir(datadir)
binfiles = [f for f in files if (f.endswith('root') and not f.endswith('reduced.root'))]

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

	filetitle = f.split('.')[0]
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:30:00\n" + \
		"#SBATCH -A nuphys\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
		"source /usr/gapps/nexo/setup.sh \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		"root -l \'" + macro + "(\"" + datadir + filetitle + \
			"\",\"" + datadir+filetitle+"\")\'" + "\n" + \
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


