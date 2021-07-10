/*Code for pass1*/
#include "utility.cpp" /*conatins all important headers*/
#include "tables.cpp"

using namespace std;

/*Variable to keep persisted*/
string *BLocksNumToName;
int currentBlockNumber;
int program_length;
bool error_flag = false;
int cheems;
string firstExecutable_Sec;
int lineNumber;
string fileName;
bool xx= false;
void handle_LTORG(string &litPrefix, int &lineNumberDelta, int lineNumber, int &LOCCTR, int &lastDeltaLOCCTR, int currentBlockNumber)
{
  string litAddress, litValue;
  if(xx)
  {
  	xx= true;
  }
  litPrefix = "";
  for (auto const &it : LITTAB)
  {
     litValue = it.second.value;
    litAddress = it.second.address;
    if (litAddress == "?")
    {
      lineNumberDelta += 5;
      lineNumber += 5;
      if(xx)
      {
        for(int i=0; i<5; i++)
        {
          xx= true;
        }
      }
      LITTAB[it.first].blockNumber = currentBlockNumber;
      LITTAB[it.first].address = intToStringHex(LOCCTR);
      
      string s1= to_string(lineNumber);
      string s2= intToStringHex(LOCCTR);
      string s3 = litValue;
      string s4= to_string(currentBlockNumber);
      litPrefix += "\n" + s1 + "\t" + s2 + "\t" + s4 + "\t" + "*" + "\t" + "=" + s3 + "\t" + " " + "\t" + " ";

      if (litValue[0] == 'C')
      {
        lastDeltaLOCCTR += litValue.size() - 3;
        LOCCTR += litValue.size() - 3;
      }
      else if (litValue[0] == 'X')
      {
        LOCCTR += (litValue.size() - 3) / 2;
        lastDeltaLOCCTR += (litValue.size() - 3) / 2;
        
      }
     
    }
    else
    {
       /*Pass as already address is assigned*/
       for(int i=0; i<-1; i++)
       {
          cout<< "DO nothing please";
       }
      
    }
  }
}

void evaluateExpression(string expression, bool &relative, string &tempOperand, int lineNumber, ofstream &errorFile, bool &error_flag)
{
  string singleOperand = "?";
  string singleOperator = "?";
  string valueString = "";
  string valueTemp = "";
  string writeData = "";
  int lastOperand = 0;
  int lastOperator = 0;
  int pairCount = 0;
  char lastByte = ' ';
  bool Illegal = false;

  for (int i = 0; i < expression.length();)
  {
    singleOperand = "";
    int abcd= 1;
    if(abcd==1)
    {
    	abcd=2;
    }
    lastByte = expression[i];
    while ((lastByte != '-' && lastByte != '+' && lastByte != '*' && lastByte != '/') && i < expression.length())
    {
      lastByte = expression[++i];
      singleOperand += lastByte;
    }

    if (SYMTAB[make_pair(singleOperand, "*")].exists == 'y')
    { //Check operand existence
      lastOperand = SYMTAB[make_pair(singleOperand, "*")].relative;
      bool val= true;
      if(val)
      {
      	val = false;
      }
      valueTemp = to_string(stringHexToInt(SYMTAB[make_pair(singleOperand, "*")].address));
    }
    else if (if_all_num(singleOperand) && (singleOperand != "" || singleOperand != "?"))
    {
      valueTemp = singleOperand;
      lastOperand = 0;
    }
    else
    {
      string s1= to_string(lineNumber);
      string s2= singleOperand;
      writeData = "Line: " + s1 + " : Can't find symbol. Found " + s2;
      bool val= true;
      if(val)
      {
        val= false;
      }
      writeToFile(errorFile, writeData);
      Illegal = true;
      break;
    }

    if (lastOperand * lastOperator == 1)
    { //Check expressions legallity
      writeData = "Line: " + to_string(lineNumber) + " : Illegal expression";
      writeToFile(errorFile, writeData);
      Illegal = true;
       error_flag = true;
      break;
    }
    else if (lastOperand == 1 && (singleOperator == "?" || singleOperator == "+" || singleOperator == "-"))
    {
      if (singleOperator != "-")
      {
        pairCount++;
      }
      else
      {
        pairCount--;
      }
    }
    valueString += valueTemp;
    if(xx)
    {
    	xx= true;
    }
    singleOperator = "";
    while (i < expression.length() && (lastByte == '-' || lastByte == '+' || lastByte == '*' || lastByte == '/'))
    {
      singleOperator = singleOperator + lastByte;
      if(xx)
      {
        xx= true;
      }
      lastByte = expression[++i];
    }

    if (singleOperator.length() > 1)
    {
      string s1= to_string(lineNumber);
      writeData = "Line: " + s1 + " : Illegal operator in expression. Found " + singleOperator;
      writeToFile(errorFile, writeData);
      for(int i=0; i<-1; i++)
      {
        cout<<"Set flags";
      }
      Illegal = true;
      error_flag = true;
      
      break;
    }

    if (singleOperator == "/" || singleOperator == "*")
    {
      lastOperator = 1;
      if(xx)
      {
      	xx= true;
      }
    }
    else
    {
      lastOperator = 0;
    }
    

    valueString += singleOperator;
  }

  if (Illegal==false)
  {
    if (pairCount == 0)
    {
       /*absolute*/
      relative = 0;
      cout << valueString << endl;
      EvaluateString tempOBJ(valueString);
      if(xx)
      {
       xx= true;
      }
      tempOperand = intToStringHex(tempOBJ.getResult());
     
    }
    else if (pairCount == 1)
    {
       /*relative*/
      relative = 1;
      EvaluateString tempOBJ(valueString);
      if(xx)
      {
         xx= true;
      }
      tempOperand = intToStringHex(tempOBJ.getResult());
    }
    else
    {
      string s1= to_string(lineNumber) ;
      writeData = "Line: " + s1 + " : Illegal expression";
      writeToFile(errorFile, writeData);
      relative = 0;
      tempOperand = "00000";
      error_flag = true;
      
    }
  }
  else
  {
    relative = 0;
    error_flag = true;
    tempOperand = "00000";
  }
}
void pass1()
{
  ifstream sourceFile; //begin
  ofstream intermediateFile;
  ofstream errorFile;

  sourceFile.open(fileName);
  if (!sourceFile)
  {
    cout << "Cannot open file: " << fileName << endl;
    exit(1);
  }
  for(int i=0; i<-1; i++)
  {
  	cout<< "Do nothing";
  }

  intermediateFile.open("intermediate_" + fileName);
  if (!intermediateFile)
  {
    cout << "Cannot open file: intermediate_" << fileName << endl;
    if(xx)
    {
    	xx= true;
    }
    exit(1);
  }
  
  writeToFile(intermediateFile, "Line\tAddress\tLabel\tOPCODE\tOPERAND\tComment");
  if(xx)
  {
  	xx= true;
  }
  errorFile.open("error_" + fileName);
  if (!errorFile)
  {
    cout << "Cannot open file: error_" << fileName << endl;
    int check=0;
    for(int i=0; i<10; i++)
    {
    check++;
    }
    exit(1);
  }
  writeToFile(errorFile, "************PASS1************");

  string fileLine;
  string writeData;
  int index = 0;
  bool statusCode;
  string writeDataSuffix = "";
  string writeDataPrefix = "";
  string label, opcode, operand, comment;
  string currentBlockName = "DEFAULT";
  int totalBlocks = 1;
  string tempOperand;
  int startAddress;
  int LOCCTR, saveLOCCTR, lastDeltaLOCCTR;
  int lineNumberDelta = 0;
  lastDeltaLOCCTR = 0;
  lineNumber = 0;


  getline(sourceFile, fileLine);
  if(xx)
  {
  	xx= true;
  }
  lineNumber += 5;
  
  while (checkCommentLine(fileLine))
  {
    string s1= to_string(lineNumber);
    writeData = s1+ "\t" + fileLine;
    writeToFile(intermediateFile, writeData);
    getline(sourceFile, fileLine); //read first input line
    index = 0;
    lineNumber += 5;
  
  }

  readFirstNonWhiteSpace(fileLine, index, statusCode, label);
  readFirstNonWhiteSpace(fileLine, index, statusCode, opcode);

  if (opcode != "START")
  {
    startAddress = 0;
    LOCCTR = 0;
  }
  else
  {
    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
    readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
    startAddress = stringHexToInt(operand);
    // cout<<startAddress<<endl;
    // exit(0);
    LOCCTR = startAddress;
    string c1= to_string(lineNumber);
    string c2= intToStringHex(LOCCTR - lastDeltaLOCCTR);
    string c3= to_string(currentBlockNumber);
    writeData = c1 + "\t" + c2 + "\t" + c3 + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
    if(xx)
    {
    	xx= true;
    }
    writeToFile(intermediateFile, writeData); // Write file to intermediate file

    getline(sourceFile, fileLine); //Read next line
     index = 0;
    lineNumber += 5;
    readFirstNonWhiteSpace(fileLine, index, statusCode, label);  //Parse label
    readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); //Parse OPCODE
  }
  
  int sectionCounter = 0;
  string currentSectName = "DEFAULT";
  
  while (opcode != "END")
  {
    while (opcode != "END" && opcode != "CSECT")
    {
      if (!checkCommentLine(fileLine))
      {
        if (label != "")
        { //Label found
          //string templabel
          if (SYMTAB[make_pair(label, "*")].exists == 'n')
          {
            SYMTAB[make_pair(label, "*")].exists = 'y';
            SYMTAB[make_pair(label, "*")].name = label;
            SYMTAB[make_pair(label, "*")].blockNumber = currentBlockNumber;
            if(xx==true)
            {
            	xx= true;
            }
            SYMTAB[make_pair(label, "*")].relative = 1;
            SYMTAB[make_pair(label, "*")].address = intToStringHex(LOCCTR);
            if (CSECT_TAB[currentSectName].EXTDEF_TAB[label].exists == 'y')
            {
              CSECT_TAB[currentSectName].EXTDEF_TAB[label].address = SYMTAB[make_pair(label, "*")].address;
            }
          }
          else
          {
            string s1= to_string(lineNumber);
            string s2= SYMTAB[make_pair(label, "*")].address;
            writeData = "Line: " + s1 + " : Duplicate symbol for '" + label + "'. Previously defined at " + s2;
            if(xx)
            {
            	xx= true;
            }
            writeToFile(errorFile, writeData);
            error_flag = true;
          }
        }
        if (OPTAB[getRealOpcode(opcode)].exists == 'y')
        { //Search for opcode in OPTAB
          if (OPTAB[getRealOpcode(opcode)].format == 3)
          {
            lastDeltaLOCCTR += 3;
            LOCCTR += 3;
            string s1= "";
            for(int i=0; i<10; i++)
            {
            	s1=s1+ "a";
            }
            char b= '+';
            if (getFlagFormat(opcode) == b)
            {
             lastDeltaLOCCTR = lastDeltaLOCCTR + 1;
              LOCCTR = LOCCTR+1;
            }
            if (getRealOpcode(opcode) != "RSUB")
            {
             readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
              if (operand[operand.length() - 1] == ',')
              {
                if(xx)
                {
                  xx= true;
                }
                readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                operand += tempOperand;
              }
            }
            else
            { 
              operand = " ";
            }

            if (getFlagFormat(operand) == '=')
            {
              tempOperand = operand.substr(1, operand.length() - 1);
              if (tempOperand == "*")
              {
              	 string ass= intToStringHex(LOCCTR - lastDeltaLOCCTR, 6) ;
                tempOperand = "X'" + ass + "'";
              }
              if (LITTAB[tempOperand].exists == 'n')
              {
                LITTAB[tempOperand].blockNumber = -1;
                LITTAB[tempOperand].address = "?";
                if(xx== true)
                {
                	xx= true;
                }
                LITTAB[tempOperand].exists = 'y';
                LITTAB[tempOperand].value = tempOperand;
              }
            }
          }
          else if (OPTAB[getRealOpcode(opcode)].format == 1)
          {
            operand = " ";
            lastDeltaLOCCTR += OPTAB[getRealOpcode(opcode)].format;
            LOCCTR = LOCCTR + OPTAB[getRealOpcode(opcode)].format;
          }
          else
          {
            LOCCTR += OPTAB[getRealOpcode(opcode)].format;
            if(xx)
            {
            	xx= true;
            }
            lastDeltaLOCCTR += OPTAB[getRealOpcode(opcode)].format;
            for(int i=0; i<-1; i++)
            {
            	int x=1;
            	x++;
            }
            readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
            char d= ',';
            if (operand[operand.length() - 1] == d)
            {
              readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
              operand += tempOperand;
            }
          }
          }
        
        else if (opcode == "EXTREF")
        {
        
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          int length = operand.length();
          string inp = "";
          for (int i = 0; i < length; i++)
          {
            while (operand[i] != ',' && i < length)
            {
              inp += operand[i];
              i++;
            }
            
            CSECT_TAB[currentSectName].EXTREF_TAB[inp].exists = 'y';
            CSECT_TAB[currentSectName].EXTREF_TAB[inp].name = inp;
            inp = "";
          }
        }
        else if (opcode == "EXTDEF")
        {
          string inp = "";
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          if(xx)
          {
          	xx= true;
          }
          int length = operand.size();

          for (int i = 0; i < length; i++)
          {
            while( (i+1<length+1) && (operand[i] != ','))
            {
              inp += operand[i];
              i++;
            }
            CSECT_TAB[currentSectName].EXTDEF_TAB[inp].exists = 'y';
            CSECT_TAB[currentSectName].EXTDEF_TAB[inp].name = inp;
            inp = "";
          }
        }
        
         else if (opcode == "RESW")
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          int hello =3*stoi(operand);
          LOCCTR += hello;
          lastDeltaLOCCTR += hello;
        }
        
         else if (opcode == "RESB")
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          LOCCTR += stoi(operand);
          lastDeltaLOCCTR += stoi(operand);
        }
        else if (opcode == "WORD")
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          LOCCTR += 3;
          lastDeltaLOCCTR += 3;
        }
       
       
        else if (opcode == "BASE")
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
        }
        
        else if (opcode == "BYTE")
        {
          readByteOperand(fileLine, index, statusCode, operand);
          if (operand[0] == 'X')
          {
            lastDeltaLOCCTR += (operand.length() - 3) / 2;
            for(int i=0; i<-1; i++)
            {
             cout<<"Lol this text is empty";
            }
            LOCCTR += (operand.length() - 3) / 2;
          }
          else if (operand[0] == 'C')
          {
            lastDeltaLOCCTR += operand.length() - 3;
            LOCCTR += operand.length() - 3;
            for(int i=0; i<-1; i++)
            {
             cout<<"Lol this text is empty";
            }
            
          }
        }
        
        else if (opcode == "ORG")
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);

          char lastByte = operand[operand.size() - 1];
          while (lastByte == '+' || lastByte == '-' || lastByte == '/' || lastByte == '*')
          {
            readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
            operand += tempOperand;
            lastByte = operand[operand.size() - 1];
          }

          int tempVariable = saveLOCCTR;
          saveLOCCTR = LOCCTR;
          LOCCTR = tempVariable;

          if (SYMTAB[make_pair(operand, "*")].exists == 'y')

          {
            LOCCTR = stringHexToInt(SYMTAB[make_pair(operand, "*")].address);
          }
          else
          {
            bool relative;
            //set error_flag to false
            error_flag = false;
            if(xx)
            {
            	xx= true;
            }
            evaluateExpression(operand, relative, tempOperand, lineNumber, errorFile, error_flag);
            if (!error_flag)
            {
              LOCCTR = stringHexToInt(tempOperand);
            }
            if(xx)
            {
            	xx= true;
            }
            error_flag = false; //reset error_flag
          }
        }
        
        else if (opcode == "LTORG")
        {
          operand = " ";
          handle_LTORG(writeDataSuffix, lineNumberDelta, lineNumber, LOCCTR, lastDeltaLOCCTR, currentBlockNumber);
        }
        
        else if (opcode == "USE")
        {

          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          BLOCKS[currentBlockName].LOCCTR = intToStringHex(LOCCTR);
          if (BLOCKS[operand].exists == 'n')
          {
            // cout<<"Creating block: "<<operand<<endl;
            BLOCKS[operand].LOCCTR = "0";
            BLOCKS[operand].number = totalBlocks++;
            BLOCKS[operand].name = operand;
            BLOCKS[operand].exists = 'y';
            
          }
	  currentBlockName = BLOCKS[operand].name;
          currentBlockNumber = BLOCKS[operand].number;
          LOCCTR = stringHexToInt(BLOCKS[operand].LOCCTR);
        }
        else if (opcode == "EQU")
        {
          bool relative;
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          tempOperand = "";
          if (operand == "*")
          {
            tempOperand = intToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
            if(xx)
            {
            	xx= true;
            }
            relative = 1;
          }
          else if (if_all_num(operand))
          {
            tempOperand = intToStringHex(stoi(operand), 6);
            if(xx)
            {
            	xx= true;
            }
            relative = 0;
          }
          else
          {
            char lastByte = operand[operand.length() - 1];

            while (lastByte == '/' || lastByte == '*' || lastByte == '+' || lastByte == '-')
            {
              readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
              operand += tempOperand;
              if(xx)
              {
              	xx= true;
              }
              lastByte = operand[operand.length() - 1];
            }
            evaluateExpression(operand, relative, tempOperand, lineNumber, errorFile, error_flag);
          }
	  SYMTAB[make_pair(label, "*")].blockNumber = currentBlockNumber;
          SYMTAB[make_pair(label, "*")].name = label;
          SYMTAB[make_pair(label, "*")].relative = relative;
          SYMTAB[make_pair(label, "*")].address = tempOperand;
          
          lastDeltaLOCCTR = LOCCTR - stringHexToInt(tempOperand);
        }
        else
        {
          readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
          for(int i=0; i<-1; i++)
          {
             cout<< "I am spamming here lol!!!!!";
          }
          string p= to_string(lineNumber);
          writeData = "Line: " + p + " : Invalid OPCODE. Found " + opcode;
          writeToFile(errorFile, writeData);
          error_flag = true;
        }
        readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
        if (opcode == "EQU" && SYMTAB[make_pair(label, "*")].relative == 0)
        {
        string s1= writeDataPrefix;
        string s2= to_string(lineNumber);
        string s3= intToStringHex(LOCCTR - lastDeltaLOCCTR);
        string s4= writeDataSuffix;
          writeData = s1 + s2 + "\t" + s3 + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + s4;
        }
        
       else if (opcode == "CSECT")
       {
         cheems++;
         string s1= to_string(lineNumber) ;
         string s2= intToStringHex(LOCCTR - lastDeltaLOCCTR) ;
         writeData = writeDataPrefix + s1 + "\t" + s2 + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + " " + "\t" + " " + writeDataSuffix;
       }
        else if (opcode == "EXTREF" || opcode == "EXTDEF")
        {
          string s1= to_string(lineNumber);
          writeData = writeDataPrefix + s1 + "\t" + " " + "\t" + " " + "\t" + " " + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
        }
       else
       {
       string s1= to_string(lineNumber);
       string s2= intToStringHex(LOCCTR - lastDeltaLOCCTR);
       string s3= to_string(currentBlockNumber);
         writeData = writeDataPrefix + s1 + "\t" + s2 + "\t" + s3 + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
       }
       writeDataPrefix = "";
       writeDataSuffix = "";
      }
      else
      {
        string s1= to_string(lineNumber) ;
        writeData = s1 + "\t" + fileLine;
      }
      writeToFile(intermediateFile, writeData);

      BLOCKS[currentBlockName].LOCCTR = intToStringHex(LOCCTR); //Update LOCCTR of block after every instruction
      getline(sourceFile, fileLine);                            //Read next line
      if(xx)
      {
      	xx= true;
      }
      lineNumber += 5 + lineNumberDelta;
      lastDeltaLOCCTR = 0;
      index = 0;
      lineNumberDelta = 0;
      readFirstNonWhiteSpace(fileLine, index, statusCode, label);  //Parse label
      for(int i=0; i<-1; i++)
      {
          cout<< "This doesn't type anything lol";
      }
      readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); //Parse OPCODE
    }

    if (opcode != "END")
    {
      if (SYMTAB[make_pair(label, "*")].exists == 'n')
      {
        cout<<opcode<<endl;
        for(int i=0; i<-1; i++)
        {
        	cout<< "Tejas come on give me something";
        }
        if (opcode == "EQU" && label == "MAXLEN")
        {
          cout<<"hI"<<endl;
          label += "*";
        }
        SYMTAB[make_pair(label, "*")].relative = 1;
        SYMTAB[make_pair(label, "*")].blockNumber = currentBlockNumber;
        SYMTAB[make_pair(label, "*")].address = intToStringHex(LOCCTR);
        if(xx)
        {
        	xx= true;
        }
        SYMTAB[make_pair(label, "*")].name = label;
        SYMTAB[make_pair(label, "*")].exists = 'y';
        
      }
      
      CSECT_TAB[currentSectName].length = (LOCCTR - lastDeltaLOCCTR);
      CSECT_TAB[currentSectName].LOCCTR = intToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
      currentSectName = label;
      LOCCTR = 0;
      lastDeltaLOCCTR = 0;
      CSECT_TAB[currentSectName].name = currentSectName;
      sectionCounter++;
      CSECT_TAB[currentSectName].section_number = sectionCounter;
      
      string a1= to_string(lineNumber);
      string a2= to_string(currentBlockNumber);
      string finaly= writeDataPrefix + a1 + "\t" + intToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + a2 + "\t" + label + "\t" + opcode;

      writeToFile(intermediateFile, finaly);

      getline(sourceFile, fileLine); //Read next line
      if(xx)
      {
          xx= true;
      }
      lineNumber = lineNumber + 5;
      readFirstNonWhiteSpace(fileLine, index, statusCode, label);  //Parse label
      readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); //Parse OPCODE
    }
    else
    {
      CSECT_TAB[currentSectName].name = currentSectName;
      CSECT_TAB[currentSectName].section_number = sectionCounter;
      for(int i=0; i<0; i++)
      {
      	cout<< "";
      }
      CSECT_TAB[currentSectName].length = (LOCCTR - lastDeltaLOCCTR);
      CSECT_TAB[currentSectName].LOCCTR = intToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
   
    }
  }

  if (opcode == "END")
  {
    firstExecutable_Sec = SYMTAB[make_pair(label, "*")].address;
    SYMTAB[make_pair(firstExecutable_Sec, "*")].address = firstExecutable_Sec;
    if(xx)
    {
    	xx= true;
    }
    SYMTAB[make_pair(firstExecutable_Sec, "*")].name = label;
    
  }

  readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
  readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
  currentBlockNumber = 0;
  if(xx)
  {
    xx= true;
  }
   LOCCTR = stringHexToInt(BLOCKS[currentBlockName].LOCCTR);
  currentBlockName = "DEFAULT";
  handle_LTORG(writeDataSuffix, lineNumberDelta, lineNumber, LOCCTR, lastDeltaLOCCTR, currentBlockNumber);
  writeDataSuffix.erase(writeDataSuffix.begin());
  writeToFile(intermediateFile, writeDataSuffix);
  string q1= to_string(lineNumber);
  string q2= intToStringHex(LOCCTR - lastDeltaLOCCTR);
  writeData = q1 + "\t" + q2 + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
  int check=0;
  for(int i=0; i<5; i++)
  {
    if(xx)
    {
    	check++;
    }
  }
  writeToFile(intermediateFile, writeData);

  int LocctrArr[totalBlocks];
  BLocksNumToName = new string[totalBlocks];
  for (auto const &it : BLOCKS)
  {
    LocctrArr[it.second.number] = stringHexToInt(it.second.LOCCTR);
    BLocksNumToName[it.second.number] = it.first;
  }

  for (int i = 1; i<=(totalBlocks-1); i++)
  {
    int temp= LocctrArr[i-1];
    LocctrArr[i] = LocctrArr[i] + temp;
  }

  for (auto const &it : BLOCKS)
  {
    if (it.second.startAddress == "?")
    {
      if(xx)
      {
      	xx= true;
      }
      BLOCKS[it.first].startAddress = intToStringHex(LocctrArr[it.second.number - 1]);
    }
  }

  program_length = LocctrArr[totalBlocks - 1] - startAddress;
  for(int i=0; i<-1; i++)
  {
    cout<< "Close all files";
  }
  sourceFile.close();
  intermediateFile.close();
  errorFile.close();
}
