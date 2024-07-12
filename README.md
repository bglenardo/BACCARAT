# BACCARAT

BACCARAT is a Geant4 application based on LUXSim (https://arxiv.org/abs/1111.2074), 
the simulation package developed for the LUX dark matter experiment.
Before it was adopted by LZ, it was briefly released on BitBucket
as an open-source code. This repository is a copy of that (pre-LZ) 
BACCARAT, with updates made by me over the years.

## Installing BACCARAT

I (Brian) use BACCARAT on the LLNL cluster `borax`. It requires **Geant4** and **ROOT** to be installed already.
is done by going into the top-level directory of the repository `BACCARAT` and typing `make`. I usually use `make -j4` 
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
* A good example to take a look at is the [**XeNeuDT**](https://github.com/bglenardo/BACCARAT/tree/main/XeNeuDT) project, which is probably the one most recently used.

* Inside the project folder, there is also a `Macros` directory, which contains the macro files used to actually run the simulation. 

(More documentation coming soon)

## RecordLevels

A key aspect of optimizing simulations is the `recordLevel`, which is set in the Macro file. 

(more documentation coming soon)


