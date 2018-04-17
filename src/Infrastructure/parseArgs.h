//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//																			//
//																			//
//					  Input argument parsing functions						//
//																			//
//																			//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifndef PARSEARGS_H
#define PARSEARGS_H

#include <iostream>
#include <fstream>

#include "simStructure.h"
#include "printEvent.h"

//////////////////////////////////////////////////////////////////////////////
//					  Print help text to command line						//
//////////////////////////////////////////////////////////////////////////////
void helpText()
{
 	printToTerminal("Following are the command line arguments for running OddSim.\n");
 	printToTerminal("Options:\n");
 	printToTerminal("    -c --config:    Path to config file.\n");
 	printToTerminal("    -i --input:     Path to input file.\n");
 	printToTerminal("    -h --help:      This help page.\n\n");
}


//////////////////////////////////////////////////////////////////////////////
//				  		Check for parsing errors							//
//////////////////////////////////////////////////////////////////////////////
void checkForParsingError( int lineNum, int itt )
{
    if ( itt >= 5 )
    {
        printToTerminal( "\n\nParsing error on line " );
        printToTerminal( lineNum );
        printToTerminal( "\nExiting simulation.\n\n" );
        exit(0);   
    }
}


//////////////////////////////////////////////////////////////////////////////
//					  		Parse config file								//
//////////////////////////////////////////////////////////////////////////////
void parseConfig( simStructure *simStruct )
{
    bool rateTrue;
    bool orderTrue;
    bool typeTrue;
    bool connectionsTrue;
    bool objDone = false;
	bool fileCorrupted = false;
	bool continueReading = true;
    int noAction;
    int charPos1;
    int charPos2;
    int lastCharUsed;
    int lineCount = 0;
	string STR;
    string objName;
    string varName;
    string tempSTR;
	ifstream configFile;
    objects OBJ;
    
	configFile.open((*simStruct).ConfigFile);
	
	if ( !configFile )
	{
		printToTerminal("Config file ");
		printToTerminal((*simStruct).ConfigFile);
		printToTerminal(" does not exist.\n\nExiting program.\n");
		exit(0);
	}
	printToTerminal("Parsing config file.\n");
	
    noAction = 0;
	while( continueReading ) 
	{
		getline(configFile,STR); lineCount++;
        lastCharUsed = 0;
		if ( STR.length() != 0 )
		{
            if ( STR.find("startObj") != string::npos )
            {
                objDone = false;
                rateTrue = false;
                orderTrue = false;
                typeTrue = false;
                connectionsTrue = false;
                
                charPos1 = STR.find(':')+1;
                charPos2 = STR.find('>');
                lastCharUsed = charPos2;
                objName = STR.substr(charPos1,charPos2-charPos1);
                OBJ.ClassName = objName;
                
                noAction = 0;
                while (( continueReading == true ) && ( objDone == false )) 
                {
		            if ( STR.find('<', lastCharUsed) == string::npos )
                    {
                        getline(configFile,STR); lineCount++;
                        lastCharUsed = 0;
                    }
                    
                    tempSTR = STR.substr(lastCharUsed, 9);
                    if ( tempSTR.compare("<endObj>") == 0 )
                    {
                        objDone = true;
                        lastCharUsed = STR.find('>',lastCharUsed);
                    }
                    else
                    {
                        charPos1 = STR.find('<',lastCharUsed)+1;
                        charPos2 = STR.find(':',charPos1);
                        lastCharUsed = charPos2;
                        tempSTR = STR.substr(charPos1,charPos2-charPos1);
                        
                        charPos1 = charPos2+1;
                        charPos2 = STR.find('>',charPos1);
                        lastCharUsed = charPos2;

                        if ( tempSTR.compare("rate") == 0 )
                        {
                            rateTrue = true;
                            OBJ.rate = stoi(STR.substr(charPos1,charPos2-charPos1));
                        }          
                        else if( tempSTR.compare("order") == 0 )
                        {
                            orderTrue = true;
                            OBJ.order = stoi(STR.substr(charPos1,charPos2-charPos1));
                        }        
                        else if( tempSTR.compare("classType") == 0 )
                        {
                            typeTrue = true;
                            OBJ.ClassType = STR.substr(charPos1,charPos2-charPos1);
                        }           
                        else if( tempSTR.compare("connections") == 0 )
                        {
                            connectionsTrue = true;
                            OBJ.Connections.push_back( STR.substr(charPos1,charPos2-charPos1));
                        }   
                        else
                        {
		                    printToTerminal("Unrecognized config file option field ");                                            
		                    printToTerminal( tempSTR );
		                    printToTerminal(" on line ");
		                    printToTerminal(lineCount); newLine();
                        }
                    }
                    
                    if ( configFile.eof() ){ continueReading = false; }
                }
                if ( !connectionsTrue && !typeTrue && !orderTrue && !rateTrue )
                {
                    printToTerminal( objName );                                            
                    printToTerminal(" not fully initialized in config file." );
                    printToTerminal("\n\nExiting simulation.\n\n");
                    exit(0);
                }
                else
                {
	                (*simStruct).Objects.push_back(OBJ);   
                }
            }
            else
            {
                noAction++;  
                checkForParsingError( lineCount, noAction );
            }
		}
        
        if ( configFile.eof() ){ continueReading = false; }
	}
	
	if ( fileCorrupted )
	{
		printToTerminal("Config file is corrupted. Exiting program.\n");
		exit(0);
	}
	configFile.close();
}


//////////////////////////////////////////////////////////////////////////////
//					  		Parse input file								//
//////////////////////////////////////////////////////////////////////////////
void parseInput( simStructure *simStruct )
{
    bool typeTrue;
    bool valueTrue;
    bool objDone = false;
    bool varDone = false;
    bool ignoreObject = false;
	bool fileCorrupted = false;
	bool continueReading = true;
    int objItt;
    int charPos1;
    int charPos2;
    int lastCharUsed;
    int noAction = 0;
    int lineCount = 0;
	string STR;
    string objName;
    string varName;
    string tempSTR;
	ifstream inputFile;
    inputs inpStruct;
    
	inputFile.open((*simStruct).InputFile);
	
	if ( !inputFile )
	{
		printToTerminal("Input file ");
		printToTerminal((*simStruct).InputFile);
		printToTerminal(" does not exist.\n\nExiting program.\n");
		exit(0);
	}
	printToTerminal("Parsing input file.\n");
	
    noAction = 0;
	while( continueReading ) 
	{
		getline(inputFile,STR); lineCount++;
        lastCharUsed = 0;
		if ( STR.length() != 0 )
		{
            if ( STR.find("startObj") != string::npos )
            {
                objDone = false;
                ignoreObject = true;
                charPos1 = STR.find(':')+1;
                charPos2 = STR.find('>');
                lastCharUsed = charPos2;
                objName = STR.substr(charPos1,charPos2-charPos1);
                for (int itt = 0 ; itt < (int) (*simStruct).Objects.size() ; itt++)
                {
                    if ( (*simStruct).Objects[itt].ClassName.compare(objName) == 0)
                    {
                        ignoreObject = false;
                        objItt = itt;
                        break;
                    }
                }   
                if ( ignoreObject )
                {
	                printToTerminal( objName );
                    printToTerminal( " is not included in the config file.\n" );
                }
                
                noAction = 0;
                while (( continueReading == true ) && ( objDone == false )) 
                {
		            if ( STR.find('<', lastCharUsed) == string::npos )
                    {
                        getline(inputFile,STR); lineCount++;
                        lastCharUsed = 0;
                    }
                    
                    tempSTR = STR.substr(lastCharUsed, 9);
                    if ( tempSTR.compare("<endObj>") == 0 )
                    {
                        objDone = true;
                        lastCharUsed = STR.find('>',lastCharUsed);
                    }
                    else
                    {
                        if ( STR.find("startVar", lastCharUsed) != string::npos )
                        {     
                            typeTrue = false;
                            valueTrue = false;
                            varDone = false;
                            lastCharUsed = STR.find("startVar");
                            charPos1 = STR.find(':',lastCharUsed)+1;
                            charPos2 = STR.find('>',charPos1);
                            lastCharUsed = charPos2;
                            inpStruct.varName = STR.substr(charPos1,charPos2-charPos1);
                       
                            while (( continueReading == true ) && ( varDone == false )) 
                            {
                                if ( STR.find('<', lastCharUsed) == string::npos )
                                {
                                    getline(inputFile,STR); lineCount++;
                                    lastCharUsed = 0;
                                }

                                lastCharUsed = STR.find('<',lastCharUsed);
                                tempSTR = STR.substr(lastCharUsed, 9);
                                if ( tempSTR.compare("<endVar>") == 0 )
                                {
                                    varDone = true;
                                    lastCharUsed = STR.find('>',lastCharUsed);
                                }  
                                else
                                {
                                    charPos1 = STR.find('<',lastCharUsed)+1;
                                    charPos2 = STR.find(':',charPos1);
                                    lastCharUsed = charPos2;
                                    tempSTR = STR.substr(charPos1,charPos2-charPos1);

                                    charPos1 = charPos2+1;
                                    charPos2 = STR.find('>',charPos1);
                                    lastCharUsed = charPos2;

                                    if ( tempSTR.compare("type") == 0 )
                                    {
                                        tempSTR = STR.substr(charPos1,charPos2-charPos1);
                                        string tempSTR2 = "boolcharstringintfloatdoublevectormatrix";
                                        if ( tempSTR2.find(tempSTR) >= 0)
                                        {
                                            valueTrue = true;
                                            inpStruct.varType = tempSTR;
                                        }
                                        else
                                        {
		                                    printToTerminal("Unrecognized data type ");                                       
		                                    printToTerminal( tempSTR );   
                                            printToTerminal(" on line ");
                                            printToTerminal(lineCount); newLine();
                                        }
                                    }
                                    else if ( tempSTR.compare("value") == 0 )
                                    {
                                        valueTrue = true;
                                        inpStruct.varValue = STR.substr(charPos1,charPos2-charPos1);
                                    }          
                                    else
                                    {
		                                printToTerminal("Unrecognized variable field ");                                            
		                                printToTerminal( tempSTR );
		                                printToTerminal(" on line ");
		                                printToTerminal(lineCount); newLine();
                                    }
                                }
                                if ( inputFile.eof() ){ continueReading = false; }
                            }
                            if ( !valueTrue && !typeTrue )
                            {
		                        printToTerminal( inpStruct.varName );                                            
		                        printToTerminal(" not fully initialized in input file." );
		                        printToTerminal("\n\nExiting simulation.\n\n");
                                exit(0);
                            }
                            else
                            {
                                (*simStruct).Objects[objItt].Inputs.push_back(inpStruct);  
                            }
                        }
                        else
                        {
                            noAction++;  
                            checkForParsingError( lineCount, noAction );
                        }
                    }
                    
                    if ( inputFile.eof() ){ continueReading = false; }
                }
            }
            else
            {
                noAction++;  
                checkForParsingError( lineCount, noAction );
            }
		}
        
        if ( inputFile.eof() ){ continueReading = false; }
	}
	
	if ( fileCorrupted )
	{
		printToTerminal("Input file is corrupted. Exiting program.\n");
		exit(0);
	}
	
	inputFile.close();
}

//////////////////////////////////////////////////////////////////////////////
//						  Parse command line arguments						//
//////////////////////////////////////////////////////////////////////////////
simStructure parseArgs( int argc, char* argv[] )
{
	bool inputInit = false;
	bool configInit = false;
	simStructure simStruct;
	newLine();
	
	for (int itt = 1 ; itt < argc ; itt++)
	{
		if ( argv[itt][0] == '-' )
		{
			switch ( argv[itt][1] ) 
			{
				case('h'):
					helpText();
					exit(0); // Close program
					break;
				case('i'):
					itt++;
					// init input file
					simStruct.InputFile = (string) argv[itt];
					printToTerminal( "Input file: " );
					printToTerminal( simStruct.InputFile );
					newLine();
					inputInit = true;
					break;
				case('c'):
					itt++;
					// init config file
					simStruct.ConfigFile = (string) argv[itt];
					printToTerminal("Config file: ");
					printToTerminal(simStruct.ConfigFile);
					newLine();
					configInit = true;
					break;
				default:
                    newLine();
					printToTerminal("Error parsing command line argument: ");
					printToTerminal(argv[itt]);
					newLine();
					break;
			}
		}
	}
	
	if ( inputInit && configInit )
	{
		newLine();
		parseConfig( &simStruct );
		parseInput( &simStruct );
        newline();
	}
	else
	{
		if ( !configInit )
		{
			printToTerminal("Configuration file has not been initialized.\n");
		}
		if ( !inputInit )
		{
			printToTerminal("Input file has not been initialized.\n");
		}
		printToTerminal("Exiting program.\n");
		exit(0); // Close program
	}
	
	return simStruct;
}


#endif

