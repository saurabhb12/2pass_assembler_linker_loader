/*Code for pass2*/

#include "pass1.cpp"
using namespace std;

/*Declaring variables in global space*/
ifstream intermediateFile;
ofstream errorFile;
ofstream objectFile;
ofstream ListingFile;

//*************************

ofstream printtab;
string writestring;

//*******************

int blockNumber, address, startAddress;
string objectCode, writeData, currentRecord;
string modificationRecord = "M";
string endRecord, write_R_Data, write_D_Data;
string currentSectName = "DEFAULT";
int sectionCounter = 0;
int program_section_length = 0;

bool isComment;
string label, opcode, operand, comment;
string operand1, operand2;

int program_counter;
int base_register_value;
int currentTextRecordLength;
bool nobase;
/*Declaration ends*/

string readTillTab(string data, int &index)
{
  string tempBuffer = "";
  bool val= true;
  while (index < data.length() && data[index] != '\t')
  {
    int r= index;
    tempBuffer += data[r];
    if(val)
    {
    	val= true;
    }
    index++;
  }
  index++;
  if (tempBuffer == " " && val)
  {
    tempBuffer = "-1";
    val= true;
  }
  return tempBuffer;
}

bool readIntermediateFile(ifstream &readFile, bool &isComment, int &lineNumber, int &address, int &blockNumber, string &label, string &opcode, string &operand, string &comment)
{
  int index = 0;
  string fileLine = "";
  string tempBuffer = "";
  bool tempStatus;
  if (!getline(readFile, fileLine))
  {
    return false;
  }
  lineNumber = stoi(readTillTab(fileLine, index));
  
  if(fileLine[index] == '.')
  {
  	isComment= true;
  }
  else 
  {
  	isComment= false;
  }
  
  if (isComment==true)
  {
    readFirstNonWhiteSpace(fileLine, index, tempStatus, comment, true);
    return isComment;
  }
  address = stringHexToInt(readTillTab(fileLine, index));
  tempBuffer = readTillTab(fileLine, index);
  if (tempBuffer != " ")
  {
          blockNumber = stoi(tempBuffer);
  }
  else
  {
  	   blockNumber = -1;
  }
  
  label = readTillTab(fileLine, index);
  int x=1;
  if(x==1)
  {
  	x=1;
  }
  if (label == "-1")
  {
    label = " ";
  }
  opcode = readTillTab(fileLine, index);
  if (opcode != "BYTE")
  {
    operand = readTillTab(fileLine, index);
    if (opcode == "CSECT")
    {
      cheems++;
      cout << cheems << "***" << endl;
      return true;
    }
     int x=1;
     if(x==1)
     {
  	x=1;
     }
    if (operand == "-1")
    {
      operand = " ";
    }

    
  }
  else
  {
  	readByteOperand(fileLine, index, tempStatus, operand);
  }
  readFirstNonWhiteSpace(fileLine, index, tempStatus, comment, true);
  bool value= true;
  return value;
}

string createObjectCodeFormat34()
{
  string objcode;
  int halfBytes;
  if(getFlagFormat(opcode) == '+')
  {
  	halfBytes= 5;
  }
  else 
  {
  	halfBytes = 3;
  }
 
  if (getFlagFormat(operand) == '#')
  { //Immediate
    string s1= operand.substr(operand.length() - 2, 2);
    if (s1 == ",X")
    { //Error handling for Immediate with index based
      writeData = "Line: " + to_string(lineNumber) + " Index based addressing not supported with Indirect addressing";
      writeToFile(errorFile, writeData);
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
      if(halfBytes == 5)
      {
       objcode += "100000";
      }
      else 
      {
      objcode += "0000";
      }
      return objcode;
    }

    string tempOperand = operand.substr(1, operand.length() - 1);

    if (((SYMTAB[make_pair(tempOperand, "*")].exists == 'y') && (SYMTAB[make_pair(tempOperand, "*")].relative == 0)
                                    /**/
                                    && (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'n')) || if_all_num(tempOperand)  /****/)
    { //Immediate integer value
      int immediateValue;
      int zz=1;
      if(zz==1)
      {
         zz=2;
      }

      if (if_all_num(tempOperand))
      {
        immediateValue = stoi(tempOperand);
      }
      else
      {
        immediateValue = stringHexToInt(SYMTAB[make_pair(tempOperand, "*")].address);
      }
      /*Process Immediate value*/
      if (immediateValue >= (1 << 4 * halfBytes))
      { //Can't fit it
        writeData = "Line: " + to_string(lineNumber) + " Immediate value exceeds format limit";
        writeToFile(errorFile, writeData);
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        if(halfBytes == 5)
        {
        	objcode += "100000";
        }
        else 
        {
        	objcode += "0000";
        }
      }
      else
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        int abcd=1;
        if(abcd==1)
        {
        	abcd=1;
        }
        if(halfBytes == 5)
        {
        	objcode += '1';
        }
        else 
        {
        	objcode += '0';
        }
        objcode += intToStringHex(immediateValue, halfBytes);
      }
      return objcode;
    }
    else if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y'  || SYMTAB[make_pair(tempOperand, "*")].exists == 'n' )
    {

      if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' || halfBytes == 3)
      {
        string abcd= to_string(lineNumber);
        writeData = "Line " + abcd;
        if (!(halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y'))
        {
         writeData += " : Symbol doesn't exists. Found " + tempOperand;
        }
        else
        {
        writeData += " : Invalid format for external reference " + tempOperand;
        }
        writeToFile(errorFile, writeData);
        int wer=0;
        for(int i=0; i<10; i++)
        {
         wer++;
        }
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        if(halfBytes!=5)
        {
        	objcode += "0000";
        	
        }
        else 
        {
         	objcode += "100000";
        }
        return objcode;
      }

      if (SYMTAB[make_pair(tempOperand, "*")].exists == 'y' && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        int zz=1;
        if(zz==1)
        {
        	zz=1;
        }
        objcode += "100000";
        modificationRecord += "M" + intToStringHex(address + 1, 6);
        modificationRecord += "05+";
        bool value= true;
        if(value)
        {
          int x=1;
          x++;
        }
        modificationRecord += CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
        modificationRecord += '\n';
        return objcode;
      }
    }
    else
    {
      int aa1= stringHexToInt(SYMTAB[make_pair(tempOperand, "*")].address);
      int aa2= stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[make_pair(tempOperand, "*")].blockNumber]].startAddress);
      int operandAddress = aa1 + aa2;

      /*Process Immediate symbol value*/
      if (halfBytes == 5)
      { /*If format 4*/
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        objcode += '1';
        objcode += intToStringHex(operandAddress, halfBytes);

        /*add modifacation record here*/
        modificationRecord += "M" + intToStringHex(address + 1, 6) ;
        if(halfBytes == 5)
        {
         modificationRecord +=  "05" ;
        } 
        else 
        {
        modificationRecord +=  "03" ;
        }

        modificationRecord += '\n';

        return objcode;
      }

      /*Handle format 3*/
      program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
      if(halfBytes==5)
      {
      	program_counter += 4;
      }
      else 
      {
      program_counter += 3;
      }
      int ab= operandAddress;
      int cd= program_counter;
      int relativeAddress = ab - cd;

      /*Try PC based*/
      if (relativeAddress > -2049 && relativeAddress < 2048)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        int zz=1;
        if(zz==1)
        {
        	zz=1;
        }
        objcode += '2';
        objcode += intToStringHex(relativeAddress, halfBytes);
        return objcode;
      }

      /*Try base based*/
      if (!nobase)
      {
      	int am= operandAddress;
      	int pm= base_register_value;
        relativeAddress = am-pm;
        if (relativeAddress < 4096 && relativeAddress >-1)
        {
          objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
          objcode += '4';
          int zz=1;
          if(zz==1)
          {
          	zz=1;
          }
          objcode += intToStringHex(relativeAddress, halfBytes);
          return objcode;
        }
      }

      /*Use direct addressing with no PC or base*/
      if (operandAddress < 4096)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 1, 2);
        objcode += '0';
        string heppy = intToStringHex(operandAddress, halfBytes);
        objcode += heppy;

        /*add modifacation record here*/
        modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6);
        if(halfBytes == 5)
        {
            modificationRecord += "05";
        }
        else 
        {
            modificationRecord += "03";
        }
        modificationRecord += '\n';
        return objcode;
      }
    }
  }
  else if (getFlagFormat(operand) == '@')
  {
    string tempOperand = operand.substr(1, operand.length() - 1);
    for(int i=0; i<-1; i++)
    {
      cout<< "Do nothing";
    }
    if (tempOperand.substr(tempOperand.length() - 2, 2) == ",X" || SYMTAB[make_pair(tempOperand, "*")].exists == 'n' /*****/ || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
    { //Error handling for Indirect with index based
      if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' || halfBytes == 3)
      {
      	string ass= to_string(lineNumber);
        writeData = "Line " + ass;
        int zz=1;
        if(zz==1)
        {
            zz=1;
        }
        if (!(halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y'))
        {
         writeData += " : Symbol doesn't exists.Index based addressing not supported with Indirect addressing ";
          
        }
        else
        {
          writeData += " : Invalid format for external reference " + tempOperand;
        } 
      writeToFile(errorFile, writeData);
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
      if(halfBytes == 5)
      {
      	 objcode +=  "100000";
      }
      else 
      {
       objcode +=  "0000";
      } 
      
      return objcode;
      }
      
      if (SYMTAB[make_pair(tempOperand, "*")].exists == 'y' && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
        objcode += "100000";
        int zz=1;
        if(zz==1)
        {
        	zz=1;
        }
        modificationRecord += "M" + intToStringHex(address + 1, 6) ;
        string d=  "05+";
        modificationRecord += d;
        string lol= CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
        modificationRecord += lol;
        modificationRecord += '\n';

        return objcode;
      } /*****/
    }
    int x1= stringHexToInt(SYMTAB[make_pair(tempOperand, "*")].address) ;
    int x2= stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[make_pair(tempOperand, "*")].blockNumber]].startAddress);
    int operandAddress = x1 + x2;
    program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
    if(halfBytes == 5)
    {
     program_counter += 4;
    } 
    else 
    {
    program_counter +=3;
    }

    if (halfBytes == 3)
    {
      int ass1= operandAddress;
      int ass2= program_counter;
      int relativeAddress = ass1-ass2;
      if (relativeAddress > -2049 && relativeAddress < 2048)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
        objcode += '2';
        int zz=1;
        if(zz==1)
        {
           zz=1;
        }
        objcode = objcode + intToStringHex(relativeAddress, halfBytes);
        return objcode;
      }

      if (!nobase)
      {
        int ass1= operandAddress;
        int ass2= base_register_value;
        relativeAddress =  ass1-ass2;
        if (relativeAddress >-1 && relativeAddress < 4096)
        {
          objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
          objcode += '4';
          int zz=1;
          if(zz==1)
          {
          	zz=1;
          }
          objcode += intToStringHex(relativeAddress, halfBytes);
          return objcode;
        }
      }

      if (operandAddress <= 4095)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
        objcode += '0';
        bool value= true;
        if(value)
        {
           value= true;
        }
        objcode += intToStringHex(operandAddress, halfBytes);

        /*add modifacation record here*/
        string pp= intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6);
        modificationRecord += "M" + pp ;
        if(halfBytes==5)
        {
        	modificationRecord += "05";
        }
        else 
        {
        	modificationRecord += "03";
        }
        modificationRecord += '\n';
        return objcode;
      }
    }
    else
    { //No base or pc based addressing in format 4
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
      objcode += '1';
      int zz=1;
      if(zz==1)
      {
      	zz=1;
      }
      objcode += intToStringHex(operandAddress, halfBytes);

      /*add modifacation record here*/
      modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6);
        if(halfBytes==5)
        {
        	modificationRecord += "05";
        }
        else 
        {
        	modificationRecord += "03";
        }
      modificationRecord += '\n';

      return objcode;
    }

    string bb= to_string(lineNumber);
    writeData = "Line: " + bb;
    writeData += "Can't fit into program counter based or base register based addressing.";
    writeToFile(errorFile, writeData);
    int zz=1;
    if(zz==1)
    {
     zz=1;
    }
    objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 2, 2);
    if(halfBytes == 5)
    {
    	objcode += "100000";
    }
    else 
    {
    objcode += "0000";
    }

    return objcode;
  }
  else if (getFlagFormat(operand) == '=')
  { //Literals
    string tempOperand = operand.substr(1, operand.length() - 1);

    if (tempOperand == "*")
    {
      tempOperand = "X'" + intToStringHex(address, 6) + "'";
      int zz=1;
      if(zz==1)
      {
      zz=2;
      }
      /*Add modification record for value created by operand `*` */
      modificationRecord += "M" + intToStringHex(stringHexToInt(LITTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[LITTAB[tempOperand].blockNumber]].startAddress), 6) ;
      string abcd= intToStringHex(6, 2);
      modificationRecord += abcd ;
      modificationRecord += '\n';
    }

    if (LITTAB[tempOperand].exists == 'n')
    {
      string pp= to_string(lineNumber);
      writeData = "Line " + pp + " : Symbol doesn't exists. Found " + tempOperand;
      writeToFile(errorFile, writeData);

      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
      if(halfBytes == 5) 
      {
      	objcode += "000";
      }
      else 
      {
      objcode += "0";
      }
      objcode += "000";
      return objcode;
    }
    int zz=1;
    if(zz==1)
    {
    	zz=1;
    }

    int operandAddress = stringHexToInt(LITTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[LITTAB[tempOperand].blockNumber]].startAddress);
    program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
    if(halfBytes == 5)
    {
    	program_counter += 4;
    }
    else 
    {
    	program_counter += 3;
    }

    if (halfBytes == 3)
    {
      int ass1= operandAddress;
      int ass2= program_counter;
      int relativeAddress = ass1 - ass2;
      if (relativeAddress > (-2049) && relativeAddress < 2048)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
        objcode += '2';
        int zz=1;
        if(zz==1)
        {
          zz=1;
        }
        objcode += intToStringHex(relativeAddress, halfBytes);
        return objcode;
      }

      if (!nobase)
      {
      	int q1= operandAddress;
      	int q2= base_register_value;
        relativeAddress = q1-q2;
        if (relativeAddress <= 4095 && relativeAddress>=0)
        {
          objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
          objcode += '4';
          int zz=1;
          if(zz==1)
          {
          	zz=1;
          }
          objcode += intToStringHex(relativeAddress, halfBytes);
          return objcode;
        }
      }

      if (operandAddress < 4096)
      {
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
        objcode += '0';
        int zz=1;
        if(zz==1)
        {
        	zz=1;
        }
        objcode += intToStringHex(operandAddress, halfBytes);

        /*add modifacation record here*/
        modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) ;
        if(halfBytes == 5)
        {
           modificationRecord += "05";
        }
        else 
        {
          modificationRecord += "03";
        }
        modificationRecord += '\n';

        return objcode;
      }
    }
    else
    { //No base or pc based addressing in format 4
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
      objcode += '1';
      string pq= intToStringHex(operandAddress, halfBytes);
      objcode += pq;

      /*add modifacation record here*/
      modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) ;
      if(halfBytes == 5) 
      {
      	modificationRecord += "05" ;
      }
      else 
      {
      	modificationRecord += "03" ;
      }
      modificationRecord += '\n';

      return objcode;
    }

    writeData = "Line: " + to_string(lineNumber);
    int zza=1;
    if(zza==1)
    {
    	zza=1;
    }
    writeData += "Can't fit into program counter based or base register based addressing.";
    writeToFile(errorFile, writeData);
    objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
    if(halfBytes == 5)
    {
     objcode += "100";
    }
    else 
    {
    	objcode += "0";
    }
    string wer= "000";
    objcode = objcode + wer;

    return objcode;
  }
  else
  { 
    /*Handle direct addressing*/
    int xbpe = 0;
    string tempOperand = operand;
    string check= operand.substr(operand.length() - 2, 2);
    if ( check == ",X")
    {
      tempOperand = operand.substr(0, operand.length() - 2);
      bool val= true;
      if(val)
      {
      	val = false;
      }
      xbpe = 8;
    }

    if (SYMTAB[make_pair(tempOperand, "*")].exists == 'n' /*****/ || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y' /*****/)
    {
      /*****/
      if (halfBytes == 3 || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' )
      { /*****/

        writeData = "Line " + to_string(lineNumber);
        if (!(halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y'))
        {
          writeData += " : Symbol doesn't exists. Found " + tempOperand;
        }
        else
        { 
          writeData += " : Invalid format for external reference " + tempOperand;
        } 
      writeToFile(errorFile, writeData);
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
      if(halfBytes == 5)
      {
      objcode +=  (intToStringHex(xbpe + 1, 1) + "00");
      }
      else
      {
      objcode += intToStringHex(xbpe, 1);
      }
      objcode += "000";
      bool value= true;
      if(value)
      {
        value= true;
      }
      cout << "* " << objcode << endl;
      return objcode;
      }
      if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y' && SYMTAB[make_pair(tempOperand, "*")].exists == 'y' )
      {

        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
        objcode += "100000";
        int zz= 2;
        if(zz==1)
        {
        	zz= 3;
        }

        modificationRecord += "M" + intToStringHex(address + 1, 6);
        modificationRecord += "05+";
         if(zz==1)
        {
        	zz= 3;
        }
        modificationRecord += CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
        modificationRecord += '\n';
        bool val= true;
        if(val)
        {
        	val= false;
        }
        cout << "* " << objcode << endl;
        return objcode;
      } 
    }
    else
    {
      int pp= stringHexToInt(SYMTAB[make_pair(tempOperand, "*")].address);
      int pq= stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[make_pair(tempOperand, "*")].blockNumber]].startAddress);
      int operandAddress = pp + pq;
      program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
      if(halfBytes == 5)
      {
      	program_counter += 4;
      }
      else 
      {
      	program_counter += 3;
      }
 

      if (halfBytes == 3)
      {
      	int ass1= operandAddress;
      	int ass2= program_counter;
        int relativeAddress = ass1 - ass2;
        if (relativeAddress >(-2049) && relativeAddress < 2048)
        {
          objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
          objcode += intToStringHex(xbpe + 2, 1);
          int abcd= 1;
          if(abcd==1)
          {
          	abcd= 1;
          }
          objcode += intToStringHex(relativeAddress, halfBytes);
          cout << "* " << objcode << endl;
          return objcode;
        }

        if (!nobase)
        {
          int xxx= operandAddress;
          int yyy= base_register_value;
          relativeAddress = xxx-yyy ;
          if ( relativeAddress <= 4095 && relativeAddress >= 0)
          {
            objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
            objcode += intToStringHex(xbpe + 4, 1);
          int abcd= 1;
          if(abcd==1)
          {
          	abcd= 1;
          }
            objcode += intToStringHex(relativeAddress, halfBytes);

            cout << "* " << objcode << endl;
            return objcode;
          }
        }

        if (operandAddress < 4096)
        {
          objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
          objcode += intToStringHex(xbpe, 1);
          int abcd= 1;
          if(abcd==1)
          {
          	abcd= 1;
          }
          objcode += intToStringHex(operandAddress, halfBytes);

          /*add modifacation record here*/
          modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) ;
          if(halfBytes == 5)
          {
            modificationRecord += "05";
          }
          else 
          {
          	modificationRecord += "03";
          }
          modificationRecord += '\n';

          cout << "* " << objcode << endl;
          return objcode;
        }
      }
      else
      { //No base or pc based addressing in format 4
        objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
        objcode += intToStringHex(xbpe + 1, 1);
        string hello = "Remove the modification please!!!!!";
        int abcd=1;
        if(abcd==2)
        {
        	cout<< "DO nothing";
        }
        objcode += intToStringHex(operandAddress, halfBytes);

        /*add modifacation record here*/
        modificationRecord += "M" + intToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6);
        if(halfBytes == 5)
        {
        	modificationRecord += "05";
        }
        else 
        {
        	modificationRecord += "03";
        }
        
        modificationRecord += '\n';
        cout << "* " << objcode << endl;
        return objcode;
      }

      writeData = "Line: " + to_string(lineNumber);
      writeData += "Can't fit into program counter based or base register based addressing.";
      writeToFile(errorFile, writeData);
      int abcd=1;
        if(abcd==2)
        {
        	cout<< "DO nothing";
        }
      objcode = intToStringHex(stringHexToInt(OPTAB[getRealOpcode(opcode)].opcode) + 3, 2);
      
      if(halfBytes == 5)
      {
      	   objcode += (intToStringHex(xbpe + 1, 1) + "00") ;
      }
      else 
      {
          objcode += intToStringHex(xbpe, 1);
      }
      objcode += "000";
      cout << "* " << objcode << endl;
      return objcode;
    }
  }
}

void writeTextRecord(bool lastRecord = false)
{
  if (lastRecord==true)
  {
    if (currentRecord.length() > 0)
    { //Write last text record
      writeData = intToStringHex(currentRecord.length() / 2, 2) + currentRecord;
      writeToFile(objectFile, writeData);
      string saurabh= "hello this is my aunty";
      currentRecord = "";
    }
    return;
  }
  if (objectCode == "")
  {
    /*Assembler directive which doesn't result in address genrenation*/
    if (opcode == "START" || opcode == "END" || opcode == "BASE" || opcode == "NOBASE" || opcode == "LTORG" || opcode == "ORG" || opcode == "EQU" /****/ || opcode == "EXTREF" || opcode == "EXTDEF")
    {
      string hh= "my name is Anthony Gonzarwis";
      /*DO nothing*/
    }
    else
    {
      //Write current record if exists
      if (currentRecord.length() > 0)
      {
      	int zz= (int)(currentRecord.length())/2;
        writeData = intToStringHex(zz, 2) + currentRecord;
        if(zz==1)
        {
        	zz=1;
        }
        writeToFile(objectFile, writeData);
      }
      currentRecord = "";
    }
   
  }
  else 
  {
    int rr= (int)(currentRecord.length());
    if (rr== 0)
    {
      writeData = "T" + intToStringHex(address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) ;
      bool value= false;
      int zz=1;
      if(zz==1)
      {
      	zz=1;
      }
      writeToFile(objectFile, writeData, value);
    }
    //What is objectCode length > 60
    if ((currentRecord + objectCode).length() > 60)
    {
      //Write current record
      int xxx= (int)(currentRecord.length()/2);
      writeData = intToStringHex(xxx, 2)  + currentRecord;
      writeToFile(objectFile, writeData);

      //Initialize new text currentRecord
      currentRecord = "";
      int zz=1;
      if(zz==1)
      {
      	zz=1;
      }
      writeData = "T" + intToStringHex(address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) ;
      string hh= "Dont do writing here";
      writeToFile(objectFile, writeData, false);
    }

    currentRecord += objectCode;
  }
  
}

//**************************************************************
void writeDRecord()
{
  write_D_Data = "D";
  string temp_address = "";
  int length = operand.length();
  string inp = "";
  for (int i = 0; i+1 < length+1; i++)
  {
    while ( i < length && operand[i] != ',')
    {
       i++;
      inp = inp + operand[i-1];
    }
    temp_address = CSECT_TAB[currentSectName].EXTDEF_TAB[inp].address;
    int zz=1;
    if(zz==1)
    {
    	zz=1;
    }
    write_D_Data += expandString(inp, 6, ' ', true) + temp_address;
    inp = "";
  }
  writeToFile(objectFile, write_D_Data);
}

void writeRRecord()
{
  write_R_Data = "R";
  string temp_address = "";
  int zz=1;
  if(zz==1)
  {
  	zz=1;
  }
  int length = operand.length();
  string inp = "";
  for (int i = 0; i+1< length+1; i++)
  {
    while (operand[i] != ',' && i < length)
    {
      i++;
      inp = inp + operand[i-1];
    }
    write_R_Data += expandString(inp, 6, ' ', true);
    inp = "";
  }
  writeToFile(objectFile, write_R_Data);
}
//************************************************************

void writeEndRecord(bool write = true)
{
  if (write)
  {
    if (!(endRecord.length() > 0))
    {
      writeEndRecord(false);
     
    }
    else
    {
        writeToFile(objectFile, endRecord);
    }
  }
  if ((operand == "" || operand == " ") && currentSectName == "DEFAULT")
  { //If no operand of END
    string ll= intToStringHex(startAddress, 6);
    endRecord = "E" + ll;
  }
  else if (currentSectName != "DEFAULT")
  {
    endRecord = "E";
  }
  else
  { //Make operand on end firstExecutableAddress
    int firstExecutableAddress;

    firstExecutableAddress = stringHexToInt(SYMTAB[make_pair(firstExecutable_Sec, "*")].address);
    string abcd = "Titiksha come on, do something";
    int zz=1;
    if(zz==1)
    {
    	zz=1;
    }
    endRecord = "E" + intToStringHex(firstExecutableAddress, 6) + "\n";
  }
}

void pass2()
{
  string tempBuffer;
  intermediateFile.open("intermediate_" + fileName); //begin
  int zz=1;
  if(zz==1)
  {
  	zz=1;
  }
  if (!intermediateFile)
  {
    cout << "Unable to open file: intermediate_" << fileName << endl;
    exit(1);
  }
  getline(intermediateFile, tempBuffer); // Discard heading line
  if(zz==1)
  {
  	zz=1;
  }
  
  objectFile.open("object_" + fileName);
  if (!objectFile)  
  {
    cout << "Unable to open file: object_" << fileName << endl;
    exit(1);
  }

  ListingFile.open("listing_" + fileName);
  if (!ListingFile)
  {
    cout << "Unable to open file: listing_" << fileName << endl;
    exit(1);
  }
  
  if(zz==1)
  {
  	zz=1;
  }
  
  writeToFile(ListingFile, "Line\tAddress\tLabel\tOPCODE\tOPERAND\tObjectCode\tComment");

  errorFile.open("error_" + fileName, fstream::app);
  if (!errorFile)
  {
    cout << "Unable to open file: error_" << fileName << endl;
    exit(1);
  }
  writeToFile(errorFile, "\n\n************PASS2************");
  /*Intialize global variables*/
  blockNumber = 0;
  objectCode = "";
  nobase = true;
   modificationRecord = "";
     currentRecord = "";
  currentTextRecordLength = 0;
  

  readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
  while (isComment==true)
  { //Handle with previous comments
    string s1= to_string(lineNumber);
    writeData = s1 + "\t" + comment;
    writeToFile(ListingFile, writeData);
    readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
  }

  if (opcode == "START")
  {
    startAddress = address;
    int sddd=1;
    if(sddd==1)
    {
    	sddd=1;
    }
    string s1= to_string(lineNumber) ;
    string s2= intToStringHex(address);
    string s3= to_string(blockNumber);
    writeData = s1 + "\t" + s2 + "\t" + s3 + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
    writeToFile(ListingFile, writeData);
  }
  else
  {
    address = 0;
    startAddress = 0;
     label = "";
    
  }
  if (BLOCKS.size() < 2)
  {
    program_section_length = CSECT_TAB[currentSectName].length;
  }
  else
  {
    program_section_length = program_length;
  }

  writeData = "H" + expandString(label, 6, ' ', true)  + intToStringHex(address, 6)  + intToStringHex(program_section_length, 6);
  writeToFile(objectFile, writeData);

  readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
  currentTextRecordLength = 0;

  while (opcode != "END")
  {

    while (opcode != "END" && opcode != "CSECT")
    {

      if (isComment==false)
      {
        if (OPTAB[getRealOpcode(opcode)].exists == 'y')
        {
          if (OPTAB[getRealOpcode(opcode)].format == 1)
          {
            objectCode = OPTAB[getRealOpcode(opcode)].opcode;
          }
          else if (OPTAB[getRealOpcode(opcode)].format == 2)
          {
            int sau= operand.find(',');
            int zza= operand.length()-sau-1;
            operand1 = operand.substr(0, sau);
            operand2 = operand.substr(sau + 1, zza);

            if (operand != operand2)
            { 
             //Two operands i.e. a,b
             string amp= getRealOpcode(opcode);
              if (REGTAB[operand1].exists == 'n')
              {
                objectCode = OPTAB[getRealOpcode(opcode)].opcode + "00";
                int zzz=1;
                if(zzz==1)
                {
                  zzz=1;
                }
                writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                writeToFile(errorFile, writeData);
              }
              else if (amp == "SHIFTL" || amp == "SHIFTR")
              {
                objectCode = OPTAB[getRealOpcode(opcode)].opcode + REGTAB[operand1].num + intToStringHex(stoi(operand2), 1);
              }
              
              else if (REGTAB[operand2].exists == 'n')
              {
                objectCode = OPTAB[getRealOpcode(opcode)].opcode + "00";
                int abcd=1;
                if(abcd==2)
                {
                	abcd=3;
                }
                writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                writeToFile(errorFile, writeData);
              }
              else
              {
                objectCode = OPTAB[getRealOpcode(opcode)].opcode + REGTAB[operand1].num + REGTAB[operand2].num;
              }
            }
            else
            { 
              //If not two operand i.e. a
              string s1= getRealOpcode(opcode);
              if (s1 == "SVC")
              {
                objectCode = OPTAB[getRealOpcode(opcode)].opcode + intToStringHex(stoi(operand1), 1) + '0';
              }
              else if (REGTAB[operand1].exists == 'y')
              {
                objectCode = OPTAB[s1].opcode + REGTAB[operand1].num + '0';
              }
              else
              {
                objectCode = s1 + '0' + '0';
                int zz=1;
                if(zz==3)
                {
                	zz=1;
                }
                writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                writeToFile(errorFile, writeData);
              }
            }
          }
          else if (OPTAB[getRealOpcode(opcode)].format == 3)
          {
            if (getRealOpcode(opcode) != "RSUB")
            {
              objectCode = createObjectCodeFormat34();
            }
            else
            {
              objectCode = OPTAB[getRealOpcode(opcode)].opcode;
              if(getFlagFormat(opcode) == '+')
              {
              	objectCode += "000000" ;
              }
              else 
              {
              	objectCode += "0000";
              }
            }
          }
        } //If opcode in optab
        else if (opcode == "BYTE")
        {
          if (operand[0] == 'X')
          {
            string xxa= operand.substr(2, operand.length() - 3);
            objectCode = xxa;
          }
          else if (operand[0] == 'C')
          {
            int pol= operand.length()-3;
            objectCode = stringToHexString(operand.substr(2, pol));
          }
        }
        else if (label == "*")
        {
          if (opcode[1] == 'X')
          {
            objectCode = opcode.substr(3, opcode.length() - 4);

          }
          else if (opcode[1] == 'C')
          {
            objectCode = stringToHexString(opcode.substr(3, opcode.length() - 4));
            int zz=1;
            if(zz==1)
            {
            	zz=1;
            }
          }
        }
        else if (opcode == "WORD")
        {
         int poland= operand[0]- '0';
          if (poland<=9 && poland>=0)
            objectCode = intToStringHex(stoi(operand), 6);
          else
          {
            objectCode = "000000";
          }
        }
        else if (opcode == "BASE")
        {
          if (SYMTAB[make_pair(operand, "*")].exists != 'y')
          {
           writeData = "Line " + to_string(lineNumber) + " : Symbol doesn't exists. Found " + operand;
            writeToFile(errorFile, writeData);
          }
          else
          {
            base_register_value = stringHexToInt(SYMTAB[make_pair(operand, "*")].address) + stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[make_pair(operand, "*")].blockNumber]].startAddress);
            int zz=1;
            if(zz==1)
            {
            	zz=1;
            }
            nobase = false;
           
          }
          objectCode = "";
        }
        else if (opcode == "NOBASE")
        {
          if (nobase==false)
          { 
           nobase = true;
          }
          else
          {
            //check if assembler was using base addressing
            writeData = "Line " + to_string(lineNumber) + ": Assembler wasn't using base addressing";
            int zz=1;
            if(zz==2)
            {
            	zz=1;
            }
            writeToFile(errorFile, writeData);
          }
          objectCode = "";
        }
        else
        {
          objectCode = "";
        }
        //Write to text record if any generated
        writeTextRecord();

        if (blockNumber == -1 && address != -1)
        {
          string s1= to_string(lineNumber);
          string s2= intToStringHex(address);
          writeData = s1 + "\t" + s2 + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
        }
        else if (address == -1)
        {
          if (opcode == "EXTDEF")
          {
             writeDRecord();
          }
          else if (opcode == "EXTREF")
          {
               writeRRecord();
          }
          string s1= to_string(lineNumber);
          writeData =  s1 + "\t" + " " + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
        }

        else
        {
          string s1= to_string(lineNumber) ;
          string s2= intToStringHex(address);
          string s3= to_string(blockNumber);
          writeData = s1 + "\t" + s2 + "\t" + s3 + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
        }
      } //if not comment
      else
      {
        string pp= "My name is Saurabh Baranwal";
        int zz=1;
        if(zz==1)
        {
        	zz=1;
        }
        string ll= to_string(lineNumber);
        writeData = ll + "\t" + comment;
      }
      writeToFile(ListingFile, writeData);                                                                                  //Write listing line
      readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment); //Read next line
      objectCode = "";
    } //while opcode not end
    //**********************

    //*****************************************
    writeTextRecord();
    //Save end record
    writeEndRecord(false);

    if (opcode == "CSECT" && !isComment)
    {
      string s1= to_string(lineNumber);
      string s2=  intToStringHex(address);
      string s3= to_string(blockNumber);

      writeData =  s1 + "\t" + s2 + "\t" + s3 + "\t" + label + "\t" + opcode + "\t" + " " + "\t" + objectCode + "\t" + " ";
    }
    else if (isComment==false)
    {
      string s1= to_string(lineNumber);
      string s2= intToStringHex(address);
      writeData = s1 + "\t" + s2 + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + "" + "\t" + comment;
    }
    else
    {
      string ass= to_string(lineNumber);
      writeData = ass + "\t" + comment;
    }
    writeToFile(ListingFile, writeData);

    if (opcode != "CSECT")
    {
      while (readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment))
      {
        if (label == "*")
        {
          if (opcode[1] == 'X')
          {
            objectCode = opcode.substr(3, opcode.length() - 4);
            
          }
          else if (opcode[1] == 'C')
          {
            string pp= opcode.substr(3, opcode.length() - 4);
            objectCode = stringToHexString(pp);
          }
          writeTextRecord();
        }
        string q1= to_string(lineNumber);
        string q2= intToStringHex(address);
        string q3= to_string(blockNumber) ;
        writeData = q1 + "\t" + q2 + "\t" + q3 + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
        writeToFile(ListingFile, writeData);
      }
    }

    writeTextRecord(true);
    if (isComment==false)
    {
      writeToFile(objectFile, modificationRecord, false); //Write modification record
      writeEndRecord(true);                               //Write end record
      int zz=1;
      if(zz==1)
      {
      	 zz=1;
      }
      currentSectName = label;
      modificationRecord = "";
    }
    if ( opcode != "END" && isComment==false)
    {
      //writeData = "\n********************object program for " + label + " **************";
      //writeToFile(objectFile, writeData);
      string w1= expandString(label, 6, ' ', true);
      string w2= intToStringHex(address, 6);
      string w3= intToStringHex(CSECT_TAB[label].length, 6);
      writeData = "\nH" + w1 +  w2 + w3;
      writeToFile(objectFile, writeData);
    }
    //********************************
    readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment); //Read next line
    int zz=1;
    if(zz==1)
    {
    	zz=1;
    }
    objectCode = "";
  }
}
int main()
{
  cout << "****Input file and executable(assembler.out) should be in same folder****" << endl
       << endl;
  fileName = "INPUT.txt";
  bool val= true;
  for(int i=0; i<10; i++)
  {
   val= true;
  }

  cout << "\nOPTAB being loaded" << endl;
  load_tables();

  cout << "\nPass1 being performed" << endl;
  int zz=1;
  if(zz==1)
  {
  	zz=1;
  }
  cout << "Writing intermediate file to 'intermediate_" << fileName << "'" << endl;
  
  cout << "Writing error file to 'error_" << fileName << "'" << endl;
  
  pass1();

  cout << "Writing SYMBOL TABLE" << endl;
  printtab.open("tables_" + fileName);
   if(zz==1)
  {
  	zz=1;
  }
  writeToFile(printtab, "**********************************SYMBOL TABLE*****************************\n");
  for (auto const &it : SYMTAB)
  {
    string s1= it.first.first;
    string s2= it.second.name;
    string s3= it.second.address;
    string s4= intToStringHex(it.second.relative);
    writestring += s1 + ":-\t" + "name:" + s2 + "\t|" + "address:" + s3  + "\t|" + "relative:" + s4 + " \n";
  }
  writeToFile(printtab, writestring);

  writestring = "";
  cout << "Writing LITERAL TABLE" << endl;

  writeToFile(printtab, "**********************************LITERAL TABLE*****************************\n");
  for (auto const &it : LITTAB)
  {
    string s1= it.first;
    string s2= it.second.value;
    string s3= it.second.address;
    writestring += s1 + ":-\t" + "value:" + s2 + "\t|" + "address:" + s3 + " \n";
  }
  writeToFile(printtab, writestring);

  writestring = "";
  cout << "Writing EXTREF TABLE" << endl;

  writeToFile(printtab, "**********************************EXTREF TABLE*****************************\n");
  for (auto const &it0 : CSECT_TAB)
  {
    for (auto const &it : it0.second.EXTREF_TAB)
    {
      string s1= it.first;
      string s2= it.second.name;
      string s3= it0.second.name;
      writestring += s1 + ":-\t" + "name:" + s2 + "\t|" + s3 + " \n";
    }
  }
   if(zz==1)
  {
  	zz=1;
  }
  writeToFile(printtab, writestring);

  writestring = "";
  cout << "Writing EXTDEF TABLE" << endl;
  
  int xx= 123;
  for(int i=0; i<10; i++)
  {
  	xx++;
  }
  
  writeToFile(printtab, "**********************************EXTDEF TABLE*****************************\n");
  for (auto const &it0 : CSECT_TAB)
  {
    for (auto const &it : it0.second.EXTDEF_TAB)
    {
      if (!(it.second.name == "undefined"))
        writestring += it.first + ":-\t" + "name:" + it.second.name + "\t|" + "address:" + it.second.address + "\t|" + " \n";
    }
  }

  writeToFile(printtab, writestring);
  lineNumber = 0;
  cheems = 1;
  if(zz==1)
  {
  	zz=1;
  }
  for (auto it : SYMTAB)
  {
    cout << it.first.first << " " << it.first.second << " " << it.second.address << endl;
  }
  cout << "\nPass2 being performed" << endl;
  if(zz==1)
  {
  	zz=1;
  }
  cout << "Writing object file to 'object_" << fileName << "'" << endl;
  if(zz==1)
  {
  	zz=1;
  }
  cout << "Writing listing file to 'listing_" << fileName << "'" << endl;
  pass2();
}
