# pioneer-g4bl

## Installation
### Prerequisites
- g4bl (from [muonsinc.com](https://muonsinc.com/Website1/tiki-index.php?page=G4beamline)) Note: On merlin, I am still at 3.06.
- ROOT
Much of muonsinc.com requires a login, but that is (used to be) free after registration.

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
```
./bin/convertRootToBLTrack2
./bin/convertRootToBLTrack2 -p -13 
./bin/convertRootToBLTrack2 -p -13  -f p0000-files.txt

*Muon production*
ls -1 ~/data/slurm/pioneer-g4bl/muprod0001/PIONEERMU_SlantedTgtE_muprod0001-400* > muprod0001-files.txt
./bin/convertRootToBLTrack2 -n muprod0001- -f muprod0001-files.txt -p -13

*How to set a parameter from the command line*
g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt HSC42cur=0.0

merlin-l-002>cd /data/user/langenegger/g4bl/pioneer-g4bl
merlin-l-002>source setup.csh

*Pion transport*
merlin-l-002>cd /data/user/langenegger/g4bl/pioneer-g4bl/pioneer/beamline
merlin-l-l002>g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt G4BLOUTPUTDIR=d0002  >& d0002/g4bl.log

```

*Batch submission*
```
replicate -f 40000 -l 40020 -p randomseed -t PIONEER_SlantedTgtE_prod-40000.i
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/prod0000%SITE T3_CH_PSI' PIONEER_SlantedTgtE_prod0000-4000*

run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/muontransport/m0000%G4BLOUTPUTDIR .%G4BLTRACKFILE p0000-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt' m0000.i

run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/transport/mu0001%G4BLOUTPUTDIR .%G4BLTRACKFILE p28-muprod0003-BLTrackFile2_PDGid-13_DetEMuPiFromTarget.txt%MOMENTUM 28' mu0001.i
```

*Fragments from my logfile* 
```
** Tables with all g4bl production settings
N(pi): at DetEMuPiFromTarget
Name:  PIONEER_SlantedTgtE_*
|-------+----------+------------------+------+-------------------------------------------------------------------------------|
| Name  | Date     | gen events       | HIMB | Comments                                                                      |
|-------+----------+------------------+------+-------------------------------------------------------------------------------|
| p0000 | 22/04/05 | 20*100e6 = 20e8  | no   | 1st batch. Issues with duplicate events/random seeds                          |
| p0001 | 22/04/13 | 12*100e6 = 12e8  | no   | no more issues with duplicate events(?) scaleMom = 2.7105 -> p = 75.9         |
| p0002 | 22/04/14 | 20*10e6  =  2e8  | 100  |                                                                               |
| p0003 | 22/09/20 | 51*100e6 = 51e8  | no   | InitKE>2, scaleMom = 65.0/28.0 QGSP_BERT_EMY                                  |
| p0004 | 22/09/21 |                  | no   | InitKE>2, scaleMom = 65.0/28.0 QGSP_BIC_HIMB_HYBRID_HP_BIAS_EMY               |
| p0005 | 22/12/14 |                  | no   | same as p0004, InitKE>2, DetFSH41 z=$posFSH41-40, DetEMuPiFromTarget z=150    |
| p0007 | 22/12/22 | 1e3*200e6 = 2e11 | no   | PIONEER_SlantedTgtE_prod0007-40000.i, DetPiE5 filled, QGSP_BERT_EMY, InitKE>2 |
| p0008 | 23/01/24 | 1e3*300e6 = 3e11 | no   | PIONEER_SlantedTgtE_prod0008-40000.i, DetPiE5 filled, FTFP_BERT_EMY, [[~/log/2023/230124-p0008-pdgid-initke-pz.pdf][InitKE>2]] |
|-------+----------+------------------+------+-------------------------------------------------------------------------------|



|--------------+----------+-----------+----------------+------+-----------------------------------------------------|
| Name         | Date     | N(mu) [k] | gen events     | HIMB | Comments                                            |
|--------------+----------+-----------+----------------+------+-----------------------------------------------------|
| muprod0000   | 22/05/03 |           |                |  100 | mistake! scaleMom=2.7105?!                          |
| muprod0001   | 22/05/04 |           |                |  100 | scaleMom=1                                          |
| p28_prod0002 | 22/05/06 |           |                |  100 | scaleMom calculated, 2212 included, shifted DetPiE5 |
| p45_prod0002 | 22/05/06 |           |                |  100 |                                                     |
| p55_prod0002 | 22/05/06 |           |                |  100 |                                                     |
| p65_prod0002 | 22/05/06 |           |                |  100 |                                                     |
| p75_prod0002 | 22/05/06 |           |                |  100 |                                                     |
| p85_prod0002 | 22/05/06 |           |                |  100 |                                                     |
|--------------+----------+-----------+----------------+------+-----------------------------------------------------|
| p28_prod0003 | 22/05/06 |           | 100*2e8 = 2e10 |      | same as prod0002, but without HIMB model            |
| p55_prod0003 | 22/05/19 |           | 100*2e8 = 2e10 |      | same as prod0002, but without HIMB model            |
| p85_prod0003 | 22/05/19 |           | 100*2e8 = 2e10 |      | same as prod0002, but without HIMB model            |
|--------------+----------+-----------+----------------+------+-----------------------------------------------------|


NEXT: 
cd /psi/home/langenegger/data/g4bl/pioneer-g4bl/pioneer/MuonProduction/jobs/220519
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/p28-muprod0003%SITE T3_CH_PSI' p28_prod0003-402*.i
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/p55-muprod0003%SITE T3_CH_PSI' p55_prod0003-402*.i
run -c ~/mu3e/mu3eanca/slurm/slurm-g4bl-pioneer.csh -r 'STORAGE1 /psi/home/langenegger/data/slurm/pioneer-g4bl/p85-muprod0003%SITE T3_CH_PSI' p85_prod0003-402*.i

N(mu) at DetEMuPiFromTarget 
|-------+----------+-----------+-----------------+------+----------|
| Name  | Date     | N(mu) [k] | gen events      | HIMB | Comments |
|-------+----------+-----------+-----------------+------+----------|
| p0002 | 22/04/25 |     69195 | 20*10e6  =  2e8 |  100 |          |
|-------+----------+-----------+-----------------+------+----------|

** Table with all g4bl transport settings for PIONS
Nend = wc CALOCNTR - 2 
|-------+-------+-------+------+----------+----------------------------------------------------------------------------------------------------------------------------------------------|
| Name  | Input | Stats | Nend |    Ratio | Comments                                                                                                                                     |
|       |       |   (k) |      |          |                                                                                                                                              |
|-------+-------+-------+------+----------+----------------------------------------------------------------------------------------------------------------------------------------------|
| d0000 | p0000 |   986 |  471 | 0.000478 |                                                                                                                                              |
| d0001 | p0000 |   986 |  545 | 0.000553 | HSC42cur=0.0                                                                                                                                 |
|-------+-------+-------+------+----------+----------------------------------------------------------------------------------------------------------------------------------------------|
| d0002 | p0002 |  6481 | 3861 | 0.000596 | g4bl G4V7M_piE5_pions.i G4BLTRACKFILE=p0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt G4BLOUTPUTDIR=d0002               >& d0002/g4bl.log |
| d0003 | p0002 |  6481 | 4414 | 0.000681 | g4bl G4V7M_piE5_pions.i  HSC42cur=0.0 G4BLTRACKFILE=p0002-BLTrackFile2_PDGid211_DetEMuPiFromTarget.txt G4BLOUTPUTDIR=d0003 >& d0003/g4bl.log |
|-------+-------+-------+------+----------+----------------------------------------------------------------------------------------------------------------------------------------------|

** Table with all g4bl transport settings for MUONS
Nend = wc CALOCNTR - 2 
|-------+------------------------+-------+------+----------+-----------------------------------------------------------------------|
| Name  | Input                  | Stats | Nend |    Ratio | Comments                                                              |
|       |                        |   (k) |      |          |                                                                       |
|-------+------------------------+-------+------+----------+-----------------------------------------------------------------------|
| m0000 | p0002                  | 69195 |      | 0.000478 | mu+                                                                   |
| m0001 | p0002                  | 69195 |  545 | 0.000553 | pi+ Zach's default                                                    |
| m0002 | muprod0000             |       |      |          |                                                                       |
| m0003 | muprod0001             |       |      |          |                                                                       |
| m0004 | m0000/muprod0001       |       |      |          | all particles (muprod0001-BLTrackFile2_PDGid0_DetEMuPiFromTarget.txt) |
| m0005 | m0001/muprod0001       |       |      |          | inverted QSK41set, QSK42set, QSK43set                                 |
|-------+------------------------+-------+------+----------+-----------------------------------------------------------------------|
|       | p28_m0001/p28_prod0002 |       |      |          |                                                                       |
|-------+------------------------+-------+------+----------+-----------------------------------------------------------------------|
```
