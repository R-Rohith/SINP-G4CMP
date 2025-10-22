# SINP-G4CMP
A repository for simulations of phonon  transport and TES response in cryogenic detectors.

## How to use the caustics code
- clone the repository
- go to the directory 'Caustics': ``` cd Caustics```
- make a `build' directory: ``` mkdir build```
- go inside the `build' directory: ``` cd build ```
- make the code: ``` cmake .. && cmake --build . -j6 ```
- After it is built, for **GUI mode** run the executable directly ```./g4cmpPhononCaustics```
- Additionally you may use the macro file to run in **batch mode** ```./g4cmpPhononCaustics ./Caustic.mac```

## Setting up GEANT4 and G4CMP
### GEANT4
- Clone/download the source code of the preferred GEANT4 version.
- Make a build directory and run cmake in it: `cmake -DCMAKE_INSTALL_PREFIX=$HOME/<geant4-install-path> -DGEANT4_INSTALL_DATA=ON -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_PYTHON=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_XM=OFF -DGEANT4_BUILD_BUILTIN_BACKTRACE=OFF -DGEANT4_INSTALL_EXAMPLES=ON  -DGEANT4_USE_SYSTEM_ZLIB=ON -DGEANT4_BUILD_TLS_MODEL=global-dynamic -DGEANT4_USE_GDML=ON $HOME/<geant4-source>`
- Then build & install it : `make -j10`,`make install`
- Add the following line to your `.bashrc` file: `source $path_to_geant_install/bin/
### G4CMP

## Changes to Geant4 and/or G4CMP code 
### G4CMP
- Commented out the exceptions relating to **track escaped from volume** and **track inconsistent position** in `library/src/G4CMPTrackLimiter.cc`; to improve simulation time.
- Until the main G4CMP is not corrected, switch the positions of ```forient``` and ```finverse``` in the ```G4LatticePhysical.cc```
