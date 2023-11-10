
source /data/user/langenegger/g4bl/root-062206/bin/thisroot.csh
source /data/user/langenegger/g4bl/geant4.10.05.p01/bin/geant4.csh /data/user/langenegger/g4bl/geant4.10.05.p01/bin

module use unstable
module load git/2.33.1
module load cmake/3.19.2
# -- "-s" suppress the warning about unstable Qt version
module load Qt/5.12.10

# -- problems with static assertion failed: comparison object must be invocable as const
#module -s load  gcc/9.3.0 openmpi/4.0.5-1_slurm fftw/3.3.10_merlin6 gsl
#module load gcc/9.3.0 openmpi/4.0.5-1_slurm fftw/3.3.9_merlin6  gsl
#module load gcc/8.4.0 openmpi/4.0.5-1_slurm fftw/3.3.9_merlin6 gsl
#module load gcc/10.3.0 openmpi/4.0.5-1_slurm  fftw/3.3.10_merlin6 gsl

# -- problems with GLIBCXX_3.4.2 (g4 installation is using newer, I guess)
module use deprecated; module load gcc/7.3.0 openmpi/3.1.3 fftw/3.3.8 gsl


#setenv  ROOT_DIR "/data/experiment/mu3e/code/external/root/root6-22-06/install"
#setenv BOOST_ROOT "/opt/psi/Compiler/boost/1.76.0/gcc/10.3.0"

# qt5
setenv PATH ${PATH}:/opt/psi/Programming/Qt/5.12.10/5.12.10/gcc_64/bin/
setenv LD_LIBRARY_PATH /opt/psi/Programming/Qt/5.12.10/5.12.10/gcc_64/lib:${LD_LIBRARY_PATH}

# g4bl stuff
setenv GEANT4_DIR /data/user/langenegger/g4bl/geant4.10.05.p01
setenv G4BL_DIR /data/user/langenegger/g4bl/G4beamline-3.06
setenv G4BLPIONEER /data/user/langenegger/g4bl/pioneer-g4bl

setenv PATH ${PATH}:${G4BL_DIR}/bin
#cd /psi/home/langenegger/data/g4bl/G4beamline-3.06/bin && source g4bl-setup.csh && cd /psi/home/langenegger
