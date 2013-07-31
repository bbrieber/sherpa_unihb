sherpa_unihb v 0.1.0
============

Sherpa Repository of the University Bremen. 


and a pre-release of some components of the sherpa Project.




This Project is still in an early development state.

The Project currently contains a bundle of robots for gazebo.



GAZEBO_PLUGIN_DIRECTORY must point to catkins devel directory


run:

To start the animation:

roslaunch sherpa_gazebo sherpa.launch

To start the human teleop:

rosrun sherpa_human_teleop sherpa_keyboard_teleop.py


Dependecies:

Gazebo 1.9
SDFORMAT
QT-Libs
python-tk
ros-groovy(might run under hydro)

urdfs of hector/ardrone and the corresponding plugins (catkinized versions that are compatible with gazebo 1.9 will be published soon...)