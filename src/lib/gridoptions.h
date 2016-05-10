#ifndef GRIDDER_OPTIONS_H
#define GRIDDER_OPTIONS_H

#include <string>
#include <time.h>

using namespace std;

/* options that can be set on the command line */
struct gridoptions
{
	string receptorfile;
	string ligandfile;
	string outname;
	string recmap;
	string ligmap;
	double dim;
	double res;
	fl randtranslate;
	int verbosity;
	int seed;
	bool randrotate;
	bool help;
	bool version;
	bool outmap;
	bool binary;
	gridoptions() :
			dim(24), res(0.5), verbosity(1), seed((int)time(NULL)),
			randrotate(false), randtranslate(0.0),
			help(false), version(false), outmap(false), binary(false)
	{
	}
};

#endif