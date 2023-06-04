#!/usr/local/bin/python

import os
geant4 = "geant4.10.02"
root = "root_v5.34.03"
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/"
#datadir = "/p/lustre1/lenardo1/sims/TMS_DD/"
#datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_up1p5cm/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_17_2020_70deg_source_centered_gas_pocket/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_24_2020_70deg_source_offset_sheilding_offset_1cm/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/April_28_2020_isotropic_neutrons/"
datadir = "/p/lustre1/lenardo1/sims/TMS_DD/June_10_2020_conical_neutrons/"
datadir = "/p/lustre2/lenardo1/simulations/TMS_DD/Jan16_2p8MeV_neutrons/"
macro = "/g/g20/lenardo1/Simulations/BACCARAT/TMS/Macros/NeutronsDD_Conical_Source.mac"
base = "TMS_DD_June_10_2020_2p5MeVNeutrons_conical_monoE"
#base = "TMS_DD_April_12_2020_"

for num in range(1,21):
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
                #   "export CPLUS_INCLUDE_PATH=/g/g17/lenardo1/CLHEP-local\n" + \
                # "export LD_LIBRARY_PATH=/g/g17/lenardo1/CLHEP-local/lib:$LD_LIBRARY_PATH\n" + \

	basename = base + str(num)
	
	scriptfilename = datadir + basename + ".sub"
	os.system( "rm -f " + scriptfilename )
	outfilename = datadir + basename + ".out"
	os.system( "rm -f " + outfilename )
	
	thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 4:00:00\n" + \
		"#SBATCH -A nuphys\n" + \
		"#SBATCH -e " + outfilename + "\n" + \
		"#SBATCH -o " + outfilename + "\n" + \
		"#SBATCH --mail-user=bglenardo@gmail.com --mail-type=fail\n" + \
		"#SBATCH -J " + basename + "\n" + \
		"#SBATCH --export=ALL \n" + \
                "source /usr/gapps/nexo/geant4/geant4.10.05.p01/share/Geant4-10.5.1/geant4make/geant4make.sh\n" + \
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


