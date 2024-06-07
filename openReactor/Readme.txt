Use Blender for the main file, export singular selected models as .stl files and use Ultimaker Cura to slice the model creating a gcode file. Use this file to print the model using a 3D printer.

Assembly.drawio file can be extended/adapted further, the jpg is a visual.

Use the eval.ipynb jupyter notebook to evaluate the data collected.

Use the master.ino file to upload the programm to the nodeMCU, set the current network details, goal temperature and goal motor speed in the code.
Once the correct temperature is reached and the button has been pressed once the nodemcu will collect data and send it to the console/database(edit as needed). Finish the experiment by pressing the button again.

For the Arrhenius experiment you can bake Natron (Natriumhydrogencarbonat, PH = 8.5), spread out on a baking sheet, on 200 celsius for 30 minutes. This will result in Soda (Natriumcarbonat, PH = 11,5).

On driver error on Windows 11, use CH341SER.exe to reset your driver to a 2019 version. Check the current version using the device manager.

Set the Lab PSU to 12V, 6A. If there are Arduino IDE upload connection problems try writing to the NodeMCU while PSU off.
