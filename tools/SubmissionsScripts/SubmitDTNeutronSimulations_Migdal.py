#!/usr/local/bin/python

import os
geant4 = "geant4.10.02"
root = "root_v5.34.03"
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/"
#macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeu/Macros/MigdalNeutrons.mac"
#macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeu/Macros/MigdalNeutrons3deg_LSTagging.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_WiderAngle.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_10deg.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_10deg_closer_detector_05.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_0p5deg_closer_detector_01.mac"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_10deg_95cmX_Migdal_06.mac"
#macro = '/g/g20/lenardo1/Simulations/BACCARAT/XeNeuDT/Macros/DTNeutrons_2deg_95cmX_Migdal_09.mac'
#datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/3deg_LSNeutronTagging/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_10percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_0p1percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_ej254_1percentDoping/"
datadir = '/p/lustre2/lenardo1/simulations/XeNeu_DT_20210908_02/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20210908_03/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220128_Four17deg_DoubleDistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220218_45deg_Four17deg_1p5distance_BKG/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220602_10deg_14det_17deg_1mdistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220604_10deg_14det_17deg_1mdistance_closer_detector/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_01/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_02/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_03/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_04/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_05/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220726_0p5deg_14det_17deg_1mdistance_30cm_closer_detector_save_seed/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220809_0p5deg_14det_17deg_1mdistance_30cm_closer_detector_save_seed/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_10deg_14det_95cmXdistance_07/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_2deg_14det_95cmXdistance_09/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/'
#datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_02/'
#datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord_20210524/"
#base = "XeNeu_TUNL_24keV_iso"
base = "XeNeu_DT_"

#for num in range(0,500):
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


