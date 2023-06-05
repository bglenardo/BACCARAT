#!/usr/local/bin/python

import os
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/tools"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_15_2020_2p5MeVNeutrons_40degAngle/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_up1p5cm/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_19_2020_70deg_source_left_1p5cm/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_24_2020_70deg_source_offset_sheilding_offset_1cm/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_centered_gas_pocket/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/June_9_2020_isotropic_neutrons/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/June_10_2020_conical_neutrons/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord_20210524/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/3deg_LSNeutronTagging/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_10percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_0p1percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_ej254_1percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/XeNeu_DT_20210908_02/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20210908_03/"
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220128_Four17deg_DoubleDistance/'
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220218_45deg_Four17deg_1p5distance_BKG/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220601_10deg_14det_17deg_1mdistance/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220602_10deg_14det_17deg_1mdistance/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220604_10deg_14det_17deg_1mdistance_closer_detector/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_05/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220726_0p5deg_14det_17deg_1mdistance_30cm_closer_detector_save_seed/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_10deg_14det_95cmXdistance_07/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_2deg_14det_95cmXdistance_09/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/"
#datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DDMigdal_20220901_10deg_14det_03/"
#datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DDMigdal_20221021_10deg_14det_8keV_01/"
#datadir = "/p/lscratchh/lenardo1/simulations/AmBe_Xe137_ActivationSims/"
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


