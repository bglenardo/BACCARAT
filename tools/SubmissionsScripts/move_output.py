import sys
import os
import shutil

input_dir = '/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord/'
destination_dir = '/p/lustre2/lenardo1/simulations/migdal_xenu/isotropic_24keV_OnlyXeRecord_20210521/'

dirfiles = os.listdir(input_dir)

binfiles = [thisfile for thisfile in dirfiles if thisfile.endswith('.bin')]
rootfiles = [thisfile for thisfile in dirfiles if thisfile.endswith('.root') and 'reduced' not in thisfile]

for rootfile in rootfiles:

    filename = rootfile.split('.')[0]
    binfiles.remove(filename+'.bin')

binfiles.sort()

for thisfile in binfiles:
    print('Moving {}'.format(thisfile))
    shutil.move(input_dir+thisfile,destination_dir+thisfile)

