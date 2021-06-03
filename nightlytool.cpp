#include <fstream>
#include <iostream>
#include <math.h>
#include <cstring>
#include "ini.h"


int main(int argc, char* argv[]) {  
  std::string ver = "1.0.0";
  std::string Devname;
  std::string Repo;
  std::string fname;
  std::string appname;
  std::string Tag;
  std::string desc= "No Description";
  std::string committag;
  
  if(argc < 6) {
    std::cout << "Version: " << "1.0.0" << std::endl;
		std::cout << "usage: nightlytool File.ini AppName RepoUser Repo Tag Commit_Hash Desc" << std::endl;
		return 1;
	}

  fname = argv[1];
  appname = argv[2];
  Devname = argv[3];
  Repo = argv[4];
  Tag= argv[5];
  if (argc > 6) desc= argv[7];
  committag= argv[6];

  mINI::INIFile file(fname);
  mINI::INIFile datafile("database.ini");
  mINI::INIStructure ini;
  mINI::INIStructure database;

  file.read(ini);
  
  std::string _3dsx = "https://github.com/" + Devname + "/" + Repo + "/releases/download/" + Tag + "/" + appname + ".3dsx";
  std::string cia = "https://github.com/" + Devname + "/" + Repo + "/releases/download/" + Tag + "/" + appname + ".cia";

  ini[Tag]["name"] = appname;
  ini[Tag]["desc"] = desc;
  ini[Tag]["Version"] = Tag;
  ini[Tag]["commit_tag"] = committag;
  ini[Tag]["3dsx"] =   _3dsx;
  ini[Tag]["cia"] =   cia;

  file.write(ini);

  //generate database
  std::string dllink = "https://github.com/" + Devname + "/" + Repo + "/raw/master/" + fname;
  database[appname]["data"] = dllink;

  datafile.write(database);
  return 0;
}