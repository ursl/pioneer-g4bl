# Application: Setpoint / V2.7.2-0-g5e7000b
#
# Created: Fri May 27 01:32:42 2022
#
# syntax:
# text    textstring
# value   *device *attr *flags
# shutter *device
# magnet  *device *sol2_value *sol2_unit *precision *flags
# slit    *device *sol2_value *sol2_unit *precision *flags
# command *device *attr *flags
#
# parameter:
#    *device      - device name   (e.g QTB12)
#    *attr        - ist attribut name (e.g IST:2)
#    *sol2_value  - sol2 value
#    *sol2_unit   - sol2 unit
#    *precision   - allowed difference between sol2 and ist2
#    *flags       - list of flags  { {flagname value} {flagname value} ... }
#                       flag {ontop trueÂ¦false} - always show on top (default is false) (all object types)
#                       flag {desc desctext} - description text (value/magnet/shutter/slit)
#                       flag {compare trueÂ¦false} - compare sol and ist value (default is true) (magnet)
#                       flag {ist2 attrname} - set ist2 attr name (default is IST:2) (magnet/slits)
#                       flag {sol2 attrname} - set sol2 attr name (default is SOL:2) (magnet/slits)
#                       flag {sol2changecmd command} - write 'command' to cmd2 after sol2 has change (default is <no command>) (magnet/slits)
#                       flag {com2 attrname} - set COM attr name (default is COM:2) (magnet/slits)shutter)
#                       flag {sta2 attrname} - set sta2 attr name (default is STA:2) (magnet/slit/shutter)
#                       flag {unit unittext} - set default unit text for sol2 and ist2 (magnet/slit/value)
#                       flag {lopr value}    - overwrite Field LOPR/minimum for sol2 and ist2 (magnet/slit/value)
#                       flag {hopr value}    - overwrite Field HOPR/maximum for sol2 and ist2 (magnet/slit/value)
#                       flag {commandbutton onÂ¦off}  - command button on/off (flag == on/off) default is on (magnet/shutter/slit)
#                       flag {bitvalue {bitnumber ontext offtext}} - define a bit value (value)
#                       flag {scale2 attrname} - set scale2 attr name (default is SCALE:2) (magnet)
#                       flag {scalevalue cmdvalue} - magnet scale factor zb. 100% (device:SCALE:2 command)
#                       flag {invertfactor trueÂ¦false} - invert file set correction factor (default is false)
#                 - list of obsolete flags
#                       flag {nocommand}               - converted to {commandbutton off} 
#                       flag {cmd {name1 value1 ...} } - list of commands (command)
#

title     "pie5"
timestamp "Fri May 27 01:32:42 2022"
desc      "pie5 setpoint config file"
version   "1.0"


text "Beamline Pie5" { {ontop "true"} }
value MHC4 "IST:2" { {ontop "true"} }
value ZPIE5CNT "INP2" { {desc "Gate   100Hz/µA"} {ontop "true"} }
value ZPIE5CNT "INP3" { {desc "Scaler"} {ontop "true"} }
value AHSW41 "IST:2" { {ontop "true"} }
magnet QSF41 -210.62 "A" 0.5
magnet HSC41 -15.13 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF42 206.6 "A" 0.5
magnet QSF43 -159.56 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC42 0 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF44 168.54 "A" 1 { {scale2 "SCALE:2"} }
magnet QSF45 161.47 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC43 62.75 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF46 -184.26 "A" 1 { {scale2 "SCALE:2"} }
magnet QSF47 214.66 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC44 -17 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF48 -133.5 "A" 1 { {scale2 "SCALE:2"} }
magnet AST41 -226.1 "A" 0.5 { {scale2 "SCALE:2"} }
magnet ASC41 243.3 "A" 1 { {scale2 "SCALE:2"} }
magnet SSL41 0 "A" 1
magnet QSB41 58 "A" 1
magnet QSB42 -126.4 "A" 1.5
magnet QSB43 67.3 "A" 1
magnet SEP41 0 "A" 0.5
magnet QSK41 0 "A" 1
magnet QSK42 0 "A" 1
magnet QSK43 0 "A" 0.5
slit FSH41-L 10 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH41-R 10 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-L 80 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-R 80 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-V 160 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH43-L 80 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH43-R 80 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
text ""
text "SEP41 HV block (be patient....)"
value SEP41VHVP "STA:2" { {desc "HV+ Status OK?"} }
magnet SEP41VHVP 0 "" 0.5 { {commandbutton "off"} {desc "HV+ "} {hopr "200.0"} {ist2 "ISTV:2"} {lopr "0.0"} {sol2 "SOLV:2"} {sta2 ""} }
value SEP41VHVP "ISTI:2" { {desc "Actual I+"} }
value SEP41VHVN "STA:2" { {desc "HV+ Status OK?"} }
magnet SEP41VHVN 0 "" 0.5 { {commandbutton "off"} {desc "HV+ "} {hopr "200.0"} {ist2 "ISTV:2"} {lopr "0.0"} {sol2 "SOLV:2"} {sta2 ""} }
value SEP41VHVN "ISTI:2" { {desc "Actual I+"} }
command SEP41VHV "COM:2" { {cmd {"HVON" "1" "HVOFF" "2" "HVSET" "3" } } {desc "Don't forget to set HV"} }
text ""
value KSF41 "STA:1" { {bitvalue {"2" "PSA OK" "PSA Not OK" } } }
shutter KSF41
value KSV41 "STA:1" { {bitvalue {"2" "PSA OK" "PSA Not OK" } } }
shutter KSV41



