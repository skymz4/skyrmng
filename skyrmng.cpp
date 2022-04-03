#include <fstream>
#include <iostream>
#include <math.h>
#include <cstring>
#include "ini.h"

/*
arg0 = filei
arg1 = appname
arg2 = reponame
arg3 = repouser
arg4 = tag
arg5 = commit
arg6 = desc
arg7 = auth
arg8 = sys
arg9 = filedata
*/
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

void SplitData(std::string dat, std::vector<std::string> split)
{
    std::vector<string> v;
 
    std::stringstream ss(dat);
 
    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        v.push_back(substr);
    }
 
    split = v;
}
DATA_ GenData(AppData appd, Sys type, std::string data)
{
  DATA_ d;
  d.link = "https://github.com/" + appd.Devname + "/" + appd.Repo + "/releases/download/" + appd.Tag + "/" + data + ".7z";
  d.filename = data;
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

  file.write(ini);
}
int main(int argc, char* argv[]) {  
  if(argc < 9) {
    std::cout << "Version: " << ver << std::endl;
    std::cout << "git stage command is included in this programm" << std::endl;
		std::cout << "usage: nightlytool File.ini AppName RepoUser Repo Tag Commit_Hash Desc seelist\n0:3dsx and cia\n1:3dsxonly\n2:ciaonly\n3:firm\n4:Firm and cia\n5:Firm and 3dsx\n6:cfw(boot.firm)\n7:3dsx, cia,firm\nAuthor\n" << std::endl;
		return 1;
	}
  AppData appd = {
    argv[0],
    argv[1],
    argv[4],
    argv[3],
    argv[5],
    argv[6],
    argv[7],
    argv[8]
  };
  std::vector<DATA_> dataf;
  std::vector<std::string> datax;
  
  Sys type = GetType(argv[9]);
  SplitData(argv[10], datax);
  for (int ll = 0; ll < datax.size(); ll++)
  {
      dataf.push_back(GenData(appd, type, datax[ll]));
  }
  
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
