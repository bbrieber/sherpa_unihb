
#include <tinyxml.h>
#include <math.h>
#include <string.h>
#include "gazebo.hh"

namespace gazebo
{
class SherpaColladaLoader
{
private:
  std::string filename;
  TiXmlElement* collada;
public:
  void Load(std::string _filename, sdf::SDF _sdf, std::string output_file){
    
  TiXmlDocument xmlDoc;
  this->filename = _filename;
  if (!xmlDoc.LoadFile(_filename))
    gzerr << "Unable to load collada file[" << _filename << "]\n";

  this->collada = xmlDoc.FirstChildElement("COLLADA");
  if (!this->collada)
    gzerr << "Missing COLLADA tag\n";

  TiXmlElement *sceneXml = this->collada->FirstChildElement("library_visual_scenes");
  //TiXmlElement *tmpXml = sceneXml->FirstChildElement("instance_visual_scene");
//  std::string sceneURL =
//    sceneXml->FirstChildElement("instance_visual_scene")->Attribute("url");

  TiXmlElement *visSceneXml = sceneXml->FirstChildElement("visual_scene");

  if (!visSceneXml)
  {
    gzerr << "Unable to find visual_scene '\n";
    return;
  }

  TiXmlElement *nodeXml = visSceneXml->FirstChildElement("node");
  while (nodeXml)
  {
    std::cerr<< "loading node " ;
    nodeXml = nodeXml->NextSiblingElement("node");
  }
  
  }
  
};
}