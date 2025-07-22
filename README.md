# SINP-G4CMP
A repository for simulations of phonon  transport and TES response in cryogenic detectors.

## How to use
- clone the repository
- go to the directory 'Caustics': ``` cd Caustics```
- make a `build' directory: ``` mkdir build```
- go inside the `build' directory: ``` cd build ```
- make the code: ``` cmake .. && cmake --build . -j6 ```
- After it is built, for **GUI mode** run the executable directly ```./g4cmpPhononCaustics```
- Additionally you may use the macro file to run in **batch mode** ```./g4cmpPhononCaustics ./Caustic.mac```


# Changes to Geant4 and/or G4CMP code 
## G4CMP
- Commented out the exceptions relating to **track escaped from volume** and **track inconsistent position**.
