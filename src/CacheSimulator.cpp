/*
	Cache Simulator Implementation by Justin Goins
	Oregon State University
	Fall Term 2018
*/

#include "CacheSimulator.h"
#include "CacheStuff.h"
#include "CacheController.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <string>

using namespace std;

/*
	* Get the filename given a path
*/
string getLastDelim(const string &path) {
	char sep = '/';
	#ifdef _WIN32
		sep = '\\';
	#endif

   size_t i = path.rfind(sep, path.length());
	 if (i != string::npos) {
		 return path.substr(i+1, path.length() - i);
	 }
	 return "";
}

// vector<string> getSplits(const string &path) {
// 	string tmpPath = path;
// 	string delimiter = "/";
//
// 	size_t pos = 0;
// 	vector<string> splits;
// 	while ((pos = tmpPath.find(delimiter)) != string::npos) {
// 	    splits.push_back( tmpPath.substr(0, pos) );
// 	    tmpPath.erase(0, pos + delimiter.length());
// 	}
// 	return splits;
// }
/*
	This function creates the cache and starts the simulator.
	Accepts core ID number, configuration info, and the name of the tracefile to read.
*/
void initializeCache(int id, ConfigInfo config, string tracefile, string filename, string dir) {
	CacheController singlecore = CacheController(config, tracefile, filename, dir);
	singlecore.runTracefile();
}

/*
	This function accepts a configuration file and a trace file on the command line.
	The code then initializes a cache simulator and reads the requested trace file(s).
*/
int main(int argc, char* argv[]) {
	ConfigInfo config;
	if (argc < 3) {
		cerr << "You need at least two command line arguments. You should provide a configuration file and at least one trace file." << endl;
		return 1;
	}

	// read the configuration file
	cout << "Reading config file: " << argv[1] << endl;
	ifstream infile(argv[1]);
	unsigned int tmp;
	infile >> config.numberSets;
	infile >> config.blockSize;
	infile >> config.associativity;
	infile >> tmp;
	config.rp = static_cast<ReplacementPolicy>(tmp);
	infile >> tmp;
	config.wp = static_cast<WritePolicy>(tmp);
	infile >> config.cacheAccessCycles;
	infile >> config.memoryAccessCycles;
	infile >> tmp;
	config.cp = static_cast<CoherenceProtocol>(tmp);
	infile.close();

	// Examples of how you can access the configuration file information
	cout << config.numberSets << " sets with " << config.blockSize << " bytes in each block. N = " << config.associativity << endl;

	if (config.rp == ReplacementPolicy::Random)
		cout << "Using random replacement protocol" << endl;
	else
		cout << "Using LRU protocol" << endl;

	if (config.wp == WritePolicy::WriteThrough)
		cout << "Using write-through policy" << endl;
	else
		cout << "Using write-back policy" << endl;

	/* extract the config name from the config path */
	string fullConfigPath = string(argv[1]);
	string configName = getLastDelim(fullConfigPath);

	/* extract the dir that this output should go.
		 This is the second to last delimiter */
	int beginConfig = fullConfigPath.length() - configName.length();
	string dir = getLastDelim(fullConfigPath.substr(0, beginConfig-1));

	/* create the ouput name by combining the configuration used and the filename */
	string tracefile = string(argv[2]);
	string filename = getLastDelim(tracefile);
	string outputname = configName + "-" + filename;

	initializeCache(0, config, tracefile, outputname, dir);

	return 0;
}
