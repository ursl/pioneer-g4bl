# pioneer-g4bl

## Installation
### Prerequisites
- g4bl (from [muonsinc.com](https://muonsinc.com/Website1/tiki-index.php?page=G4beamline)) 
- ROOT
Much of muonsinc.com requires a login, but that is (used to be) free after registration.

Note: On merlin, I am still at 3.06. This version should be useable by any merlin user.

I am (still) trying to install the current most advanced version 3.08 on merlin (the PSI cluster and attached SLURM batch system) but so far I am stuck with issues related to multithreading (job finishing)/gcc versions/dependencies.

### PIONEER g4bl repository
```
git clone git@github.com:ursl/g4bl-pioneer
cd g4bl-pioneer
source setup.csh (translate for bash)
```
On merlin, [setup.csh](https://github.com/ursl/pioneer-g4bl/blob/master/setup.csh) will provide a working environment. The above repository is a clone/combination of Zach Hodge's repositories [pie5](https://gitlab.com/zhodge/pie5), [g4bl_himbmodel](https://gitlab.com/zhodge/g4bl_himbmodel), and  [g4bl_pioneer_example](https://gitlab.com/zhodge/g4bl_pioneer_example).


## Operations
`g4bl` studies normally split into two modes

### Production
Run the primary proton beam onto the target and store track files for propagation studies. Various options are of interest
- Enhance muon production, with `QGSP_BIC_HIMB_HYBRID_HP_BIAS_EMY`, e.g. in [MuonProduction/PIONEERMU_SlantedTgtE_muprod0000-40000.i](https://github.com/ursl/pioneer-g4bl/blob/master/pioneer/MuonProduction/PIONEERMU_SlantedTgtE_muprod0000-40000.i)
- Disable pion decays, with `physics QGSP_BERT_EMY disable=Decay`, e.g. in [PionTransport/G4V7M_piE5_pions.i](https://github.com/ursl/pioneer-g4bl/blob/master/pioneer/PionTransport/G4V7M_piE5_pions.i)

Example on merlin
```
cd /data/user/langenegger/g4bl/pioneer-g4bl/pioneer/PionProduction
g4bl PIONEER_SlantedTgtE_prod0008-40000.i
```

<details>
<summary>Batch submission example</summary>
  
```
[edit PIONEER_SlantedTgtE_prod0008-40000.i and test it]
mkdir jobs/230124-p8 && cd jobs/230124-p8
replicate -f 40000 -l 41000 -p randomseed -t ../../PIONEER_SlantedTgtE_prod0008-40000.i
cd ~/data/g4bl/pioneer-g4bl/pioneer/PionProduction/jobs/230124-p8
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/p65-prod0008/%SITE T3_CH_PSI' PIONEER_SlantedTgtE_prod0008-40[8,9]*.i
rm /data/project/general/pioneer/g4bl/bl2/p0008-p65BLTrackFile2_PDGid0_DetPiE5.txt
cd ~/data/g4bl/pioneer-g4bl/macros/
./bin/convertRootToBLTrack2 -p 0 -d /psi/home/langenegger/data/slurm/pioneer-g4bl/p65-prod0008 -n p0008-p65 -o /data/project/general/pioneer/g4bl/bl2 -v DetPiE5
```

(The above relies heavily on my old-fashioned run setup and depends on [auxiliary tools](https://github.com/ursl/mu3eanca/tree/master/slurm))
</details>

This saves tracks in a (combined, if you ran many batch jobs) "BLTrack2" file (ASCII) that serves as input to the next step.

### Propagation
The beam line setup/study/optimization normally starts from the "BLTrack2" file produced in the production step. 

```
g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0008-p65BLTrackFile2_PDGid0_DetPiE5.txt G4BLOUTPUTDIR=testG histoFile=testG/g4beamline.root >& testG/bla.log &
```




### Various notes
- The piE5 beamline parameters (magnet field maps, positions, scale factors, etc) are in [pioneer-g4bl/pie5](https://github.com/ursl/pioneer-g4bl/tree/master/pie5)
- In some of the input files the magnets are not configured using (scaled) currents but rather with `set` parameters (e.g. [pioneer-g4bl/pie5/Scalefactors.txt#L60](https://github.com/ursl/pioneer-g4bl/blob/master/pie5/Scalefactors.txt#L60))
- There are parameter variations between Zach and Giovanni (dal Maso). The latter is not interested in pions, but surface muons for mu3e.
- SSL41 is missing (between ASC41 and QSB41)
- For reference the beamline element positions:

![https://github.com/ursl/pioneer-g4bl/assets/5073648/8a2d43f4-66d6-45fd-90ee-e84e26862db1](https://github.com/ursl/pioneer-g4bl/files/14998185/doc-pie5-Positions.pdf)

[doc-pie5-Positions.pdf](https://github.com/ursl/pioneer-g4bl/files/14998185/doc-pie5-Positions.pdf)

