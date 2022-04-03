#include <fstream>
#include <iostream>
#include <math.h>
#include <cstring>
#include "ini.h"

struct AppData
{
  std::string fname;
  std::string appname;
  std::string Devname;
  std::string Repo;
  std::string Tag;
  std::string committag;
  std::string desc= "No Description";
  std::string Author = "None";
};
std::string ver = "1.0.0";
enum AppType
{
  HB_Both,
  _3DSXONLY,
  CIAONLY,
  FIRM,
  FIRMCIA,
  FIRM3DSX,
  CFW,
  EVERYTHING,
  NOTHING
};

enum Sys
{
  WINDOWS,
  LINUX,
  EVERYTHING,
  NONE
};

struct DATA_
{
  std::string link;
  std::string filename;
  std::string system_s;
};

Sys GetType(std::string tp)
{
  if (tp == "0")return WINDOWS;
  if (tp == "1")return LINUX;
  if (tp == "2")return EVERYTHING;
  else return NOTHING;
}

DATA_ GenData(AppData appd, Sys type)
{
  DATA_ d;
  d.link = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + appd.appname + ".7z";
  d.filename = appd.fname;
  d.system_s = "windows";
}

void GenFile(AppData appd, Sys type)
{
  mINI::INIFile file(appd.fname);
  mINI::INIStructure ini;

  file.read(ini);
  ini[appd.Tag]["name"] = appd.appname;
  ini[appd.Tag]["desc"] = appd.desc;
  ini[appd.Tag]["Version"] = appd.Tag;
  ini[appd.Tag]["commit_tag"] = appd.committag;
  ini[appd.Tag]["Author"] = appd.Author;

  std::string _3dsx = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + appd.appname + ".3dsx";
  std::string cia = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + appd.appname + ".cia";
  std::string firm = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + appd.appname + ".firm";
  std::string cfwfirm = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + "boot.firm";

  switch (type)
  {
  case HB_Both:
    ini[appd.Tag]["3dsx"] =   _3dsx;
    ini[appd.Tag]["cia"] =   cia;
    break;
  case _3DSXONLY:
    ini[appd.Tag]["3dsx"] =   _3dsx;
    break;
  case CIAONLY:
    ini[appd.Tag]["cia"] =   cia;
    break;
  case FIRM:
    ini[appd.Tag]["firm"] =   firm;
    break;
  case FIRMCIA:
    ini[appd.Tag]["firm"] =   firm;
    ini[appd.Tag]["cia"] =   cia;
    break;
  case FIRM3DSX:
    ini[appd.Tag]["3dsx"] =   _3dsx;
    ini[appd.Tag]["firm"] =   firm;
    break;
  case CFW:
    ini[appd.Tag]["cfwfirm"] =   cfwfirm;
    break;
  case EVERYTHING:
    ini[appd.Tag]["3dsx"] =   _3dsx;
    ini[appd.Tag]["cia"] =   cia;
    ini[appd.Tag]["firm"] = firm;
    break;
  case NOTHING:
    break;
  }
  file.write(ini);
}
int main(int argc, char* argv[]) {  
  if(argc < 9) {
    std::cout << "Version: " << "1.0.0" << std::endl;
    std::cout << "git stage command is included in this programm" << std::endl;
		std::cout << "usage: nightlytool File.ini AppName RepoUser Repo Tag Commit_Hash Desc seelist\n0:3dsx and cia\n1:3dsxonly\n2:ciaonly\n3:firm\n4:Firm and cia\n5:Firm and 3dsx\n6:cfw(boot.firm)\n7:3dsx, cia,firm\nAuthor\n" << std::endl;
		return 1;
	}
  AppData appd = {
    argv[1],
    argv[2],
    argv[3],
    argv[4],
    argv[5],
    argv[6],
    argv[7],
    argv[9]
  };

  AppType type = GetType(argv[8]);
  GenFile(appd, type);
  //generate database
  std::string databasefilenaem = appd.Repo+ "-Database.ini";
  std::string dllink = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/raw/master/" + appd.fname;
  mINI::INIFile datafile(databasefilenaem); 
  mINI::INIStructure database;
  datafile.read(database);
  database["info"]["repository"] = appd.Repo;
  database["info"]["user"] = appd.Devname;

  database[appd.appname]["Name"] = appd.appname;
  database[appd.appname]["data"] = dllink;
  datafile.write(database);
  std::string command = "git stage " + databasefilenaem;
  system(command.c_str());
  std::string command2 = "git stage " + appd.fname;
  system(command2.c_str());
  return 0;
}
