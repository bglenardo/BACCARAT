#!/usr/local/bin/python

import os
geant4 = "geant4.10.02"
#execdir = "/g/g20/lenardo1/Simulations/"
#datadir = "/p/lscratchh/lenardo1/simulations/XeNeu_TUNL_579keV_pm10keV_04/batch_10/"
execdir = "/g/g20/lenardo1/"
#datadir = "/p/lustre1/lenardo1/sims/TMS_DD/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_15_2020_2p5MeVNeutrons_40degAngle/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_down1p5cm/Bin/"
datadir= "/p/lustre1/lenardo1/sims/TMS_DD/April_19_2020_70deg_source_left_1p5cm/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_24_2020_70deg_source_offset_sheilding_offset_1cm/Bin/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_centered_gas_pocket/Bin/"
datadir = "/p/lustre1/lenardo1/simulations/XeNeu_DT_20210908_03/" 
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220128_Four17deg_DoubleDistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220218_45deg_Four17deg_1p5distance_BKG/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220601_10deg_14det_17deg_1mdistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220602_10deg_14det_17deg_1mdistance/'
datadir = '/p/lustre1/lenardo1/simulations/XeNeu_DT_20220623_10deg_14det_17deg_1mdistance_30cm_closer_detector_01/'
executable = "/g/g20/lenardo1/Simulations/BACCARAT/tools/XeDetectorHitSelection"
base = "XeNeu_DT_June_4_2022_XeHit_"

files = os.listdir(datadir)
rootfiles = [f for f in files if f.endswith('root') and \
				'ls_hit' not in f and \
				'xe_hit' not in f and \
				'reduced' not in f]

numfiles = len(rootfiles)
print('{} root files to process...'.format(numfiles))


i=0

for f in rootfiles:

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
	
	scriptfilename = datadir + basename + "Xe_Hit.sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + "Xe_hit.out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 0:05:00\n" + \
		"#SBATCH -A nuphys\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
		"source /usr/gapps/nexo/setup.sh \n" + \
                "source /usr/gdata/cern/geant4.10.04.p01/setup \n" + \
		"source ~/.profile.linux \n" + \
		"module load gcc/10.2.1 \n" + \
                "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/g/g20/lenardo1/Simulations/BACCARAT/tools \n" + \
                "cd " + execdir + "\n" + \
		"export STARTTIME=`date +%s`\n" + \
		"echo Start time $STARTTIME\n" + \
		executable + ' ' + datadir + f + " " + datadir + basename + "_xe_hit.root" + "\n" + \
		"cd " + datadir + "\n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"
	
	scriptfile = open( scriptfilename, 'w' )
	scriptfile.write( thescript )
	scriptfile.close()
	
	os.system( "sbatch " + scriptfilename )

	#"chmod g+rwx *\n" + \
