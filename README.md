# FASERnuPilot
#Clone the FASERnuPilot git repo:

git clone https://github.com/SpencJ95/fasernupilot.git

#Setup the LHC computing grid:

cd fasernupilot1
source setupLCG.sh LCG_95

#Make the build directory:

mkdir build
cd build

#Build the project:

cmake -DCMAKE_INSTALL_PREFIX=../run ..;make -j8;make install -j8

#Run in batch mode:

cd ../run
bin/FASERnu -m ../run1.mac -t 100
