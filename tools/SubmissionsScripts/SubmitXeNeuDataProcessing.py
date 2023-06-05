#!/usr/local/bin/python

import os
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord_20210524/"
#datadir = "/p/lscratchh/lenardo1/simulations/AmBe_Xe137_ActivationSims/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/3deg_LSNeutronTagging/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_10percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_0p1percentDoping/"
datadir = "/p/lustre2/lenardo1/simulations/migdal_xenu/2deg_LSNeutronTagging_ej254_1percentDoping/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20210908_03/"
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220128_Four17deg_DoubleDistance/'
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220218_45deg_Four17deg_1p5distance_BKG/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220601_10deg_14det_17deg_1mdistance/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220602_10deg_14det_17deg_1mdistance/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_04/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220726_0p5deg_14det_17deg_1mdistance_30cm_closer_detector_save_seed/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_01/"
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_10deg_14det_95cmXdistance_07/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220901_2deg_14det_95cmXdistance_08/'
#datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220403_10deg_14det_95cmXdistance_06/'
#datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DDMigdal_20220901_10deg_14det_01/'
#datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DDMigdal_20221021_10deg_14det_8keV_01/' 
#datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20220604_10deg_14det_17deg_1mdistance_closer_detector/"
#datadir = "/p/lustre2/lenardo1//simulations/migdal_xenu/NeutronInitiatedInLSTagger/"

execdir = "/g/g20/lenardo1/Simulations/"
macro = "/g/g20/lenardo1/Simulations/XeNeuReductionMacroTemplate.C"
base = "XeNu_DT_processing_and_reduction_"

files = os.listdir(datadir)
binfiles = [f for f in files if (f.endswith('ls_hit.root') and not f.endswith('reduced.root'))]

#binfiles = [f for f in files if (f.endswith('.root') and not f.endswith('reduced.root') \
#							and not f.endswith('ls_hit.root') \
#							and not f.endswith('xe_hit.root'))]
#binfiles = [f for f in files if (f.endswith('xe_hit.root') and not f.endswith('reduced.root'))]

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
	
	# thescript = "#!/bin/bash\n" + \
	# 	"#SBATCH -t 00:05:00\n" + \
	# 	"#SBATCH -A redxenon\n" + \
	# 	"#SBATCH -e " + outfilename + "\n" + \
	# 	"#SBATCH -o " + outfilename + "\n" + \
	# 	"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
	# 	"#SBATCH -J " + basename + "\n" + \
	# 	"#SBATCH --export=ALL \n" + \
    #     "source ~/.profile.linux \n" + \
    #     "cd " + execdir + "\n" + \
	# 	"export STARTTIME=`date +%s`\n" + \
	# 	"echo Start time $STARTTIME\n" + \
	# 	"root -l -q \'" + macro + "(\"" + datadir + filetitle + \
	# 		"\",\"" + datadir+filetitle+"_reduced.root\")\'" + "\n" + \
	# 	"export STOPTIME=`date +%s`\n" + \
	# 	"echo Stop time $STOPTIME\n" + \
	# 	"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
	# 	"echo CPU time: $DT seconds\n"
	
	# scriptfile = open( scriptfilename, 'w' )
	# scriptfile.write( thescript )
	# scriptfile.close()
	
	# os.system( "sbatch " + scriptfilename )
 
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 00:10:00\n" + \
		"#SBATCH -A redxenon\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
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

