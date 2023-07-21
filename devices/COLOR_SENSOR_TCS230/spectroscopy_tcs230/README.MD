
https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf
I used Information from this manual.

Schematic of the "spectrometer" needs to be added here, but the IO can be gathered from the Arduino code itself
TODO: add Schematic

The Calibration, of the maximum and minimum sensory Input needs to be refined. At the moment, the brightness of the ambient lighting is not taken into account during calibration (light that interferes with my sensors as a side effect of a poor casing). Additionally, the values of the diodes would need to be pushed to their limits using stronger markers. A calibration function for a calibration standard (calibration solution) has already been implemented, but currently, at best, it has the potential to differentiate qualitatively, but not quantitatively.
TODO: develop better Calibration process

first Button is calibration
second button is fo pushing to Database (for this you would need to adapt the Insert String)

BUG: This Code is far from safe to use in a production system nor in any other way, without proper understanding what you are running.
- use the first button, to get to boot (this is not intentional but something is not initialized?) But with this you also calibrate the device to calibration standard

