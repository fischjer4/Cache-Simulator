/*
	Cache Simulator Implementation by Justin Goins
	Oregon State University
	Fall Term 2018
*/

#include "CacheController.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <cmath>
#include <sstream>

using namespace std;

CacheController::CacheController(ConfigInfo ci, char* tracefile) {
	// store the configuration info
	this->ci = ci;
	this->inputFile = string(tracefile);
	this->outputFile = this->inputFile + ".out";
	// compute the other cache parameters
	this->numByteOffsetBits = log2(ci.blockSize);
	this->numSetIndexBits = log2(ci.numberSets);
	// initialize the counters
	this->globalCycles = 0;
	this->globalHits = 0;
	this->globalMisses = 0;
	this->globalEvictions = 0;

	// cache has Sets, which have Blocks
	for(unsigned int i = 0; i < this->ci.numberSets; i++) {
		this->sets.push_back( new CacheSet(i, this->ci) );
	}

	// manual test code to see if the cache is behaving properly
	// will need to be changed slightly to match the function prototype
	/*
	cacheAccess(false, 0);
	cacheAccess(false, 128);
	cacheAccess(false, 256);

	cacheAccess(false, 0);
	cacheAccess(false, 128);
	cacheAccess(false, 256);
	*/
}

/*
	Starts reading the tracefile and processing memory operations.
*/
void CacheController::runTracefile() {
	cout << "Input tracefile: " << inputFile << endl;
	cout << "Output file name: " << outputFile << endl;

	// process each input line
	string line;
	// define regular expressions that are used to locate commands
	regex commentPattern("==.*");
	regex instructionPattern("I .*");
	regex loadPattern(" (L )(.*)(,[[:digit:]]+)$");
	regex storePattern(" (S )(.*)(,[[:digit:]]+)$");
	regex modifyPattern(" (M )(.*)(,[[:digit:]]+)$");

	// open the output file
	ofstream outfile(outputFile);
	// open the output file
	ifstream infile(inputFile);
	// parse each line of the file and look for commands
	while (getline(infile, line)) {
		// these strings will be used in the file output
		string opString, activityString;
		smatch match; // will eventually hold the hexadecimal address string
		/* NOTE:
				* Address will hold the decimal representation of the hexadecimal number,
				  however when you print to the screen the hex string will be printed.
				* For printing you need to cout << std::dec << address to see the decimal
				  representation
		*/
		unsigned long int address;
		// create a struct to track cache responses
		CacheResponse response;

		// ignore comments
		if (std::regex_match(line, commentPattern) || std::regex_match(line, instructionPattern)) {
			// skip over comments and CPU instructions
			continue;
		} else if (std::regex_match(line, match, loadPattern)) {

			cout << "Found a load op!" << endl;
			istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			cacheAccess(&response, false, address);
			outfile << " " << response.cycles << (response.hit ? " hit" : " miss") << (response.eviction ? " eviction" : "");

		} else if (std::regex_match(line, match, storePattern)) {

			cout << "Found a store op!" << endl;
			istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			cacheAccess(&response, true, address);
			outfile << " " << response.cycles << (response.hit ? " hit" : " miss") << (response.eviction ? " eviction" : "");

		} else if (std::regex_match(line, match, modifyPattern)) {

			cout << "Found a modify op!" << endl;
			istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3);
			// first process the read operation
			cacheAccess(&response, false, address);
			string tmpString; // will be used during the file output
			tmpString.append(response.hit ? " hit" : " miss");
			tmpString.append(response.eviction ? " eviction" : "");
			unsigned long int totalCycles = response.cycles; // track the number of cycles used for both stages of the modify operation
			// now process the write operation
			cacheAccess(&response, true, address);
			tmpString.append(response.hit ? " hit" : " miss");
			tmpString.append(response.eviction ? " eviction" : "");
			totalCycles += response.cycles;
			outfile << " " << totalCycles << tmpString;

		} else {
			throw runtime_error("Encountered unknown line format in tracefile.");
		}
		outfile << endl;
	}

	// add the final cache statistics
	outfile << "Hits: " << this->globalHits << " Misses: " << this->globalMisses
					<< " Evictions: " << this->globalEvictions << endl;
	outfile << "Cycles: " << this->globalCycles << endl;

	cout << "Hits: " << this->globalHits << " Misses: " << this->globalMisses
				  << " Evictions: " << this->globalEvictions << endl;
	cout << "Cycles: " << this->globalCycles << endl;


	infile.close();
	outfile.close();
}

/*
	Calculate the block index and tag for a specified address.
*/
CacheController::AddressInfo CacheController::getAddressInfo(unsigned long int address) {
	AddressInfo ai;
	// 0 based tag and index
	ai.tag = (address / this->ci.blockSize);  //floor of decimal address / blocksize
	ai.setIndex = (ai.tag % this->ci.numberSets);   //index is the (tag mod blocksize)

	return ai;
}

/*
	This function allows us to read or write to the cache.
	The read or write is indicated by isWrite.
*/
void CacheController::cacheAccess(CacheResponse *response, bool isWrite, unsigned long int address) {
	// determine the index and tag
	AddressInfo ai = getAddressInfo(address);

	cout << "\tSet index: " << ai.setIndex << ", tag: " << ai.tag << endl;

	if (isWrite) {
		/* Store Operation */
		this->sets[ai.setIndex]->storeBlockFromCPU(response, ai.tag);
	} else {
		/* Load Operation */
		this->sets[ai.setIndex]->loadBlockIntoCPU(response, ai.tag);
	}

	if (response->hit)
		cout << "Address " << std::hex << address << " was a hit." << endl;
	else
		cout << "Address " << std::hex << address << " was a miss." << endl;

	cout << "-----------------------------------------" << endl;

	return;
}

/*
	* Compute the number of cycles used by a particular memory operation.
	* This will depend on the cache write policy.
	1. Load and In cache
		a. Cache access
		b. Hit = true
	2. Store and in cache
		a. Cache access
		b. If write-through, mem access
		c. Hit = true
	3. Load and not in cache
		a. Cache access
		b. Mem access, 1
		c. If dirty = true, mem access 2
		d. Hit = false
	4. Store and not in cache
		a. Cache access
		b. Mem access, 1
		c. If dirty = true, mem access 2
		d. Write-through, mem acccess 2
Hit = false
*/
void CacheController::updateCycles(CacheResponse* response, bool isWrite) {
	response->cycles = 0;
	/* add cycles to get to cache */
	response->cycles += this->ci.cacheAccessCycles;
	/* if it was a hit */
	if (response->hit) {
		/* if it was a store and write-through mode */
		if (isWrite && this->ci.wp == WritePolicy::WriteThrough) {
			/* memory access 1 */
			response->cycles += this->ci.memoryAccessCycles;
		}
	}
  /* if it was a miss */
	else {
		/* memory access 1 to go get our block */
		response->cycles += this->ci.memoryAccessCycles;
		/* if write-back mode and a dirty eviction took place */
		if (this->ci.wp == WritePolicy::WriteBack && response->dirtyEviction) {
			/* memory access 2 to write old block back to memory */
			response->cycles += this->ci.memoryAccessCycles;
		} else if (this->ci.wp == WritePolicy::WriteThrough) {
			/* memory access 2 to write our data back to memory for write-through mode */
			response->cycles += this->ci.memoryAccessCycles;
		}
	}
}
