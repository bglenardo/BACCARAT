import sys
import os
import numpy as np

outputdir = "/p/lustre1/lenardo1/simulations/CrystalSims/20210620_StanfordDetectorSims_1p5by1p5_bad_side/"
macrodir = "/g/g20/lenardo1/Simulations/BACCARAT/GammaEfficiencies/Macros/"
execdir = "/g/g20/lenardo1/Simulations/BACCARAT/"

energies = [energy for energy in np.logspace(1.,3.4,13)]

for energy in energies:

    ############################################################################
    basename = "Gammas_{:04}keV".format(int(energy))
    macroname = basename + ".mac"
    print('Creating {}'.format(macroname))

    themacro = "/run/verbose 0\n"+\
               "/control/verbose 0\n"+\
               "/grdm/verbose 0\n"+\
               "/process/verbose 0\n\n"+\
               "/run/initialize\n"+\
               "/Bacc/io/outputDir {}\n".format(outputdir)+\
               "/Bacc/io/outputName NaI_scintillator_StanfordCrystal_1p5by1p5_{:04}keV_\n".format(int(energy))+\
               "/Bacc/detector/select GammaEfficienciesDetector\n"+\
               "/Bacc/detector/update\n\n"+\
               "/Bacc/detector/recordLevel scintillator 2\n\n"+\
               "/gps/pos/type Volume\n"+\
               "/gps/pos/shape Cylinder\n"+\
               "/gps/pos/centre 0., 0., 0., cm\n"+\
               "/gps/pos/halfz 0.635 cm\n"+\
               "/gps/pos/radius 0.05 cm\n"+\
               "/gps/pos/rot1 0 0 1\n"+\
               "/gps/pos/rot2 0 1 0\n"+\
               "/gps/particle gamma\n"+\
               "/gps/ang/type iso\n"+\
               "/gps/energy {} keV\n\n".format(energy)+\
               "/Bacc/physicsList/useOpticalProcesses 0\n\n"+\
               "/Bacc/beamOn 1000000\n"+\
               "exit\n"

    macrofile = open( macrodir + macroname, 'w' )
    macrofile.write( themacro )
    macrofile.close()           
    
    ############################################################################       
    scriptfilename = outputdir + basename + ".sub"
    outfilename = outputdir + basename + ".out"
    
    thescript = "#!/bin/bash\n" + \
		"#SBATCH -t 00:30:00\n" + \
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
		"./BACCARATExecutable " + macrodir + macroname + "\n" + \
		"cd " + outputdir + "\n" + \
		"chmod g+rwx *\n" + \
		"export STOPTIME=`date +%s`\n" + \
		"echo Stop time $STOPTIME\n" + \
		"export DT=`expr $STOPTIME - $STARTTIME`\n" + \
		"echo CPU time: $DT seconds\n"

    scriptfile = open( scriptfilename, 'w' )
    scriptfile.write( thescript )
    scriptfile.close()
	
    os.system( "sbatch " + scriptfilename )



