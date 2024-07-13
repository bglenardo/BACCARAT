# BACCARAT

BACCARAT is a Geant4 application based on LUXSim (https://arxiv.org/abs/1111.2074), 
the simulation package developed for the LUX dark matter experiment.
Before it was adopted by LZ, it was briefly released on BitBucket
as an open-source code. This repository is a copy of that (pre-LZ) 
BACCARAT, with updates made by me over the years.

## Installing BACCARAT

I (Brian) use BACCARAT on the LLNL cluster `borax`. It requires **Geant4** and **ROOT** to be installed already.
Building the code is accomplished by going into the top-level directory of the repository `BACCARAT` and typing `make`. I usually use `make -j4` 
to make it go a bit faster. 

I run with:
* ROOT v6.24/08 (which I installed myself)
* Geant4.10.7.2 (maintained by the nEXO Spack environment)


## Running the code

Running the code is done using the executable coupled to a macro file
```
./BACCARATExecutable <my_macro_file>.mac
```


## Project hierarchy

The program was written in such a way that the simulation code (i.e. the physics etc.) is kept separate from the geometry, 
making it easier to code up custom geometries for new projects. Each specific project gets its own subdirectory in
the main parent directory. Existing projects include:
* **XeNeu** - contains the geometry for the LLNL-led neutron scattering experiments conducted at TUNL in 2019
* **XeNeuDT** - contains the geometry for the DT yields experiments and the DT Migdal measurements done at LLNL in 2022/2023
* **GammaCounter** - geometry for some simulations Brian did for the neutron source used in the TMS TPC experiments in Giorgio's lab (~2020)
* **AmBeXe127Source** - for simulating activation of Xe127 using the Stanford AmBe source
* **AmBeXe137Source** - for simulating activation of Xe137 using the Stanford AmBe source
* **Mossbauer** - for simulating the transmission of Fe57 gammas for the Stanford Mossbauer experiment
* **NeutronAttenuationTest**
* **ThermalFluxCalc**
* **TMS**
* **Xe127Radioassay**
* **TestShield**
* **KTonXe**
* **GammaEfficiencies**
  
Within each project folder, there is both an `include` and a `source` directory, in which there are three files that are absolutely necessary:
* **`<ProjectName>Detector.cc/h`** which contains the geometry specific to that project. If the project is complicated, sub-geometries can be given their own class and can then be called here. But this file is absolutely necessary.
* **`<ProjectName>Materials.cc/h`** which contains any project-specific material information. Maybe you need to include some weird borated polyethlyene in your geometry... you can add that here.
* **`<ProjectName>Messenger.cc/h`** which defines how the specific project interacts with the rest of the BACCARAT code. Generally you don't need to change this, unless you want to add some macro commands.

A good example to take a look at is the [**XeNeuDT**](https://github.com/bglenardo/BACCARAT/tree/main/XeNeuDT) project, which is probably the one most recently used.

Inside the project folder, there is also a `Macros` directory, which contains the macro files used to actually run the simulation. 

## Macros

The Macro file defines the simulation parameters; which geometry to use, which volumes to record data from, what the properties of the source are, etc. Below are links to example macros that include comments which hopefully serve as sufficient documentation.
* `XeNeuDT/Macros/DTNeutrons_10deg_95cmX_Migdal_EXAMPLE.mac` - illustrates a real simulation that I ran to generate a neutron beam for the DT Migdal experiment at LLNL.
* `XeNeuDT/Macros/GeantinoLauncher.mac` - illustrates the use of a geantino as well as tracking information printed out to the terminal to get information about the geometry. Also saves a .wrl visualization of the geometry, which can be useful for debugging.

## RecordLevels

A key aspect of optimizing simulations is the `recordLevel`, which is set in the Macro file. The one I use most commonly is "2", which records each step in the volume in which energy is deposited.

(more documentation needed)

## Output and workflow specific to the LLNL Migdal effort

There are several steps in processing the simulations after they are run. The initial output is in the form of `.bin` binary files, which are not human-readable.
1. **Run the simulation**. This of course is accomplished using your own macro and the `BACCARATExecutable`.
2. **Convert the binary file format to a ROOT tree**. This conversion is accomplished via the `BaccRootConverter` executable, which is compiled in the `BACCARAT/tools/` directory when building the code.
3. **Reduce the ROOT files by throwing away events that don't hit the liquid scintillators**. In the LLNL-based experiments, we use liquid scintillator detectors to tag the interactions in the TPC. However, most of the events that we simulate do not contain any interactions in the liquid scintillator. Therefore, we can quickly make the simulations easier to work with by immediately throwing away all the simulated events in which there were no interactions in the liquid scintillator. This is done using the `/tools/LSDetetorHitSelection` executable, which can be compiled using the `tools/compile_LSHitSelection.sh` bash script.
4. **Do some preliminary event building using XeNeuAnalysisSimsEnvironment**. The `XeNeuAnalysisSimsEnvironment` (stored in the `AnalysisTools/` directory)  takes the track information form the `BaccRootConverter` output and produces a ROOT tree with the useful information from the event (stuff like the total energy deposited in the liquid xenon and scintillators, the hit times of each volume, etc.).

Each step is typically done by submitting lots of parallel jobs to the cluster. I've added my job submission scripts to the `JobSubmissionScripts` directory; these automate the process of writing a bash script for each job and submitting it to the cluster.


