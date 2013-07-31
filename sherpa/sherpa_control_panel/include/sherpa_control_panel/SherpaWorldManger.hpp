#ifndef __SHERPA_WORLD_MANAGER_HPP__
#define __SHERPA_WORLD_MANAGER_HPP__


#define WORLD_HEADER "<?xml version=\"1.0\" ?>\n\
<sdf version=\"1.4\">\n\
  <world name=\"%1\">\n\
    <include>\n\
      <uri>model://sun</uri>\n\
    </include>\n"
    
#define WORLD_FOOTER "\
  </world>\n\
</sdf>"

#define MAP_MODEL "\
    <model name=\"sherpa_map\">\n\
      <static>true</static>\n\
      <link name=\"link\">\n\
        <collision name=\"collision\">\n\
          <geometry>\n\
            <heightmap>\n\
              <uri>file://%1</uri>\n\
              <size>%2 %3 %4</size>\n\
              <pos>0 0 -100</pos><!-- FIXED at the moment -->\n\
            </heightmap>\n\
          </geometry>\n\
        </collision>\n\
        <!--FIXED TEXTURES AT THE MOMENT-->\n\
        <visual name=\"visual\">\n\
          <geometry>\n\
            <heightmap>\n\
              <texture>\n\
                <diffuse>file://media/materials/textures/Snow.png</diffuse>\n\
                <normal>file://media/materials/textures/flat_normal.png</normal>\n\
                <size>50</size>\n\
              </texture>\n\
              <uri>file://%1</uri>\n\
	      <size>%2 %3 %4</size>\n\
              <pos>0 0 -100</pos><!-- FIXED at the moment -->\n\
            </heightmap>\n\
          </geometry>\n\
        </visual>\n\
      </link>\n\
    </model>\n"

#define INCLUDE_BASE_CAMP "\
    <include>\n\
      <uri>model://sherpa_base_camp</uri>\n\
      <name>sherpa_base_camp</name>\n\
      <pose>%1 %2 %3 0 0 0</pose>\n\
    </include>\n"


#define SPAWN_ROVER ""
#define SPAWN_HECTOR ""
#define SPAWN_HUMAN ""
#define SPAWN_HELI ""
    
    
#include "Singleton.hpp"
#include "SherpaWorldStateListener.hpp"



#include <QString>

#include <QList>


namespace sherpa_control_panel{
  class SherpaWorldManager : public Singleton<SherpaWorldManager>, public SherpaWorldStateListener{
    
    void registerWorldStateListener(SherpaWorldStateListener listener){listeners << listener;}
    void deregisterWorldStateListener(SherpaWorldStateListener listener){listeners.removeAll(listener);}
    int convert_world_to_localspace(double pos){return (int) pos/map_scale;}
    double convert_local_to_worldspace(int pos){return pos*map_scale;}
    
  private:
    QString world_description;
    
    QString world_name;
    
    int world_size;//must be n^2 + 1
    bool include_human;
    bool include_rover;
    bool include_hector;
    bool include_heli;
        
    double base_camp_x,base_camp_y;
    //MAP SETTINGS
    QString image_filename;
    double map_scale;
    double map_height;
    
    
    //FILE SETTINGS
    QString world_filename;
    
    
    QList<SherpaWorldStateListener> listeners;
  };
  
//#define WORLD_MANAGER SherpaWorldManager.getInstance()
}

#endif