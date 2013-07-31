#!/usr/bin/env python

import os 
#from subprocess import call
#call(["gazebo","`rospack find sherpa_gazebo`/worlds/sherpa.world","-g","`rospack find sherpa_gazebo`/lib/libsherpa_human_gui_plugin.so"])

os.system("gazebo `rospack find sherpa_gazebo_plugins`/worlds/sherpa.world -g `rospack find sherpa_gazebo_plugins`/lib/libsherpa_human_gui_plugin.so `rospack find sherpa_gazebo_plugins`/media")
