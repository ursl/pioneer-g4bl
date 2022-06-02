# Application: Setpoint / V2.7.2-0-g5e7000b
#
# Created: Wed Jun 1 23:47:48 2022
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
timestamp "Wed Jun 1 23:47:48 2022"
desc      "pie5 setpoint config file"
version   "1.0"


text "Beamline Pie5" { {ontop "true"} }
value MHC4 "IST:2" { {ontop "true"} }
value ZPIE5CNT "INP2" { {desc "Gate   100Hz/µA"} {ontop "true"} }
value ZPIE5CNT "INP3" { {desc "Scaler"} {ontop "true"} }
value AHSW41 "IST:2" { {ontop "true"} }
magnet QSF41 -249.81 "A" 0.5
magnet HSC41 -45.5334 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF42 224.988 "A" 0.5
magnet QSF43 -174.098 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC42 0 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF44 219.617 "A" 1 { {scale2 "SCALE:2"} }
magnet QSF45 230.345 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC43 131.243 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF46 -218.39 "A" 1 { {scale2 "SCALE:2"} }
magnet QSF47 253.355 "A" 1 { {scale2 "SCALE:2"} }
magnet HSC44 0 "A" 0.5 { {scale2 "SCALE:2"} }
magnet QSF48 -137.896 "A" 1 { {scale2 "SCALE:2"} }
magnet AST41 -262.535 "A" 0.5 { {scale2 "SCALE:2"} }
magnet ASC41 299.763 "A" 1 { {scale2 "SCALE:2"} }
magnet SSL41 0 "A" 1
magnet QSB41 101.138 "A" 1
magnet QSB42 -128.565 "A" 1.5
magnet QSB43 40.1765 "A" 1
magnet SEP41 -38.356 "A" 0.5
magnet QSK41 -36.351 "A" 1
magnet QSK42 86.55 "A" 1
magnet QSK43 -72.702 "A" 0.5
slit FSH41-L 0 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH41-R 0 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-L 150 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-R 150 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FS42-V 50 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH43-L 150 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
slit FSH43-R 150 "mm" 0.5 { {desc ""} {ist2 "POSA"} {sol2 "SOL"} {sol2changecmd "MOVE_TO_SOL"} }
text ""
text "SEP41 HV block (be patient....)"
value SEP41VHVP "STA:2" { {desc "HV+ Status OK?"} }
magnet SEP41VHVP 150 "" 0.5 { {commandbutton "off"} {desc "HV+ "} {hopr "200.0"} {ist2 "ISTV:2"} {lopr "0.0"} {sol2 "SOLV:2"} {sta2 ""} }
value SEP41VHVP "ISTI:2" { {desc "Actual I+"} }
value SEP41VHVN "STA:2" { {desc "HV+ Status OK?"} }
magnet SEP41VHVN 150 "" 0.5 { {commandbutton "off"} {desc "HV+ "} {hopr "200.0"} {ist2 "ISTV:2"} {lopr "0.0"} {sol2 "SOLV:2"} {sta2 ""} }
value SEP41VHVN "ISTI:2" { {desc "Actual I+"} }
command SEP41VHV "COM:2" { {cmd {"HVON" "1" "HVOFF" "2" "HVSET" "3" } } {desc "Don't forget to set HV"} }
text ""
value KSF41 "STA:1" { {bitvalue {"2" "PSA OK" "PSA Not OK" } } }
shutter KSF41
value KSV41 "STA:1" { {bitvalue {"2" "PSA OK" "PSA Not OK" } } }
shutter KSV41



