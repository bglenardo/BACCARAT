#!/usr/local/bin/python

import os
geant4 = "geant4.10.02"
root = "root_v5.34.03"
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/"
#macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeu/Macros/MigdalNeutrons.mac"
#macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeu/Macros/MigdalNeutrons3deg_LSTagging.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_WiderAngle.mac"
#datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/3deg_LSNeutronTagging/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_10percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_0p1percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_ej254_1percentDoping/"
datadir = '/p/lustre2/lenardo1/simulations/XeNeu_DT_20210908_02/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20210908_03/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220128_Four17deg_DoubleDistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220218_45deg_Four17deg_1p5distance_BKG/'
#datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord_20210524/"
#base = "XeNeu_TUNL_24keV_iso"
base = "XeNeu_DT_"

for num in range(0,500):

	basename = base + str(num)
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 03:10:00\n" + \
		"#SBATCH -A nuphys\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
                "source /usr/gapps/nexo/geant4/geant4.10.05.p01/share/Geant4-10.5.1/geant4make/geant4make.sh \n" + \
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


