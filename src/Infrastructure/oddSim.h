//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//																			//
//																			//
//							  oddSim Header									//
//																			//
//																			//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifndef ODDSIM_H
#define ODDSIM_H

#include "simStructure.h"

//////////////////////////////////////////////////////////////////////////////
//					  Print help text to command line						//
//////////////////////////////////////////////////////////////////////////////
void helpText()
{
 	std::cout << "Following are the command line arguments for running OddSim." << "\n";
 	std::cout << "Options:" << "\n";
 	std::cout << "    -c --config:    Path to config file." << "\n";
 	std::cout << "    -i --input:     Path to input file." << "\n";
 	std::cout << "    -h --help:      This help page." << "\n\n";
}

//////////////////////////////////////////////////////////////////////////////
//					  		Parse input file								//
//////////////////////////////////////////////////////////////////////////////
void parseInput( simStructure *simStruct )
{
	
}

//////////////////////////////////////////////////////////////////////////////
//					  		Parse config file								//
//////////////////////////////////////////////////////////////////////////////
void parseConfig( simStructure *simStruct )
{
	
}

//////////////////////////////////////////////////////////////////////////////
//						  Parse command line arguments						//
//////////////////////////////////////////////////////////////////////////////
simStructure parseArgs( int argc, char* argv[] )
{
	bool inputInit = false;
	bool configInit = false;
	simStructure simStruct;
	
	for (int itt = 1 ; itt < argc ; itt++)
	{
		if ( argv[itt][0] == '-' )
		{
			switch ( argv[itt][1] ) 
			{
				case('i'):
					itt++;
					// init input file
					simStruct.InputFile = (string) argv[itt];
					std::cout << "Input file: " << simStruct.InputFile << "\n";
					inputInit = true;
					break;
				case('c'):
					itt++;
					// init config file
					simStruct.ConfigFile = (string) argv[itt];
					std::cout << "Config file: " << simStruct.ConfigFile << "\n";
					configInit = true;
					break;
				case('h'):
					helpText();
					exit(0); // Close program
					break;
				default:
					std::cout << "Error parsing command line argument: " << argv[itt] << "\n";
					break;
			}
		}
	}
	
	if ( inputInit && configInit )
	{
		parseConfig( &simStruct );
		parseInput( &simStruct );
	}
	else
	{
		if ( !configInit )
		{
			cout << "Configuration file has not been initialized.\n";
		}
		if ( !inputInit )
		{
			cout << "Input file has not been initialized.\n";
		}
		cout << "Exiting program.\n";
		exit(0); // Close program
	}
	
	return simStruct;
}


#endif

