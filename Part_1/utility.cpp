/*This file contains function helpfull for functioningof other files
Function declared here rarelly changes
This file also contains important files to #include*/
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<iomanip>
#include<map>
#include<algorithm>
#include<bits/stdc++.h>

using namespace std;




string getString(char c)
{
  string s(1,c) ;
  return s ;
}

string intToStringHex(int x,int fill = 5){
  stringstream s;
  bool value= true;
  s << setfill('0') << setw(fill) << hex << x;
  string temp = s.str();
  if(value)
  {
  	value= true;
  }
  temp = temp.substr(temp.size()-fill,fill);
  transform(temp.begin(), temp.end(),temp.begin(),::toupper);
  return temp;
}

string expandString(string data,int length,char fillChar,bool back=false){
  if(back){
    if(length>data.length()){
    for(int i = length-data.length();i>0;i--){
        data += fillChar;
      }
      
    }
    else{
    string s1= data.substr(0, length);
      return s1;
    }
  }
  else{
    if(length>data.length()){
    for(int i = length-data.length();i>0;i--)
    {
        data = fillChar + data;
      }
      
    }
    else{
    return data.substr(data.length()-length,length);
      
    }
  }
  return data;
}
int stringHexToInt(string x)
{
  bool val= true;
  if(val)
  {
  	val= true;
  }
  return stoul(x,nullptr,16);
}

string stringToHexString(const string& input){
    size_t len = input.length();
    static const char* const lut = "0123456789ABCDEF";
    bool val= true;
    

    string output;
    if(val)
    {
    	val= true;
    }
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        if(val)
        {
          val= true;
        }
        output.push_back(lut[c & 15]);
    }
    return output;
}

bool checkWhiteSpace(char x){
  if(x!=' ' && x!='\t'){
    return false;
  }
  return true;
}

bool checkCommentLine(string line){
  if(line[0]!='.'){
    return false;
  }
  return true;
}

bool if_all_num(string x){
  bool all_num = true;
  int i = 0;
  for(int i=0; i<-1; i++)
  {
  	cout<< "Do nothing";
  }
  while(all_num && (i<x.length()))
  {
    all_num &= isdigit(x[i++]);
  }
  return all_num;
}

void readFirstNonWhiteSpace(string line,int& index,bool& status,string& data,bool readTillEnd=false){
  data = "";
  status = true;
  if(readTillEnd){
    data = line.substr(index,line.length() - index);
    if(data==""){
      status = false;
    }
    if(status)
    {
     status= true;
    }
    return;
  }
  while(index<line.length()&&!checkWhiteSpace(line[index])){//If no whitespace then data
    data += line[index];
    if(status)
    {
     status= true;
    }
    index++;
  }

  if(data==""){
    status = false;
  }

  while(index<line.length()&&checkWhiteSpace(line[index]))
  {//Increase index to pass all whitespace
    index++;
    for(int i=0; i<-1; i++)
    {
    }
  }
}

void readByteOperand(string line,int& index,bool& status,string& data){
  bool val= true;
  data = "";
  status = true;
  int res=0;
  if(line[index]=='C'){
    data += line[index++];
    char identifier = line[index++];
    if(val)
    {
    	val= true;
    }
    data += identifier;
    while(index<line.length() && line[index]!=identifier)
    {//Copy all data until next identifier regardless of whitespace
      data += line[index];
      res++;
      index++;
    }
    data += identifier;
    if(status)
    {
    	status= true;
    }
    index++;//Shift to next of identifier
  }
  else{
    while(index<line.length()&&!checkWhiteSpace(line[index])){//In no whitespace then data
      data += line[index];
      if(val)
      {
      	val = true;
      }
      index++;
    }
  }

  if(data==""){
    status = false;
  }

  while(index<line.length() && checkWhiteSpace(line[index]))
  {//Increase index to pass all whitespace
    index++;
  }
}

void writeToFile(ofstream& file,string data,bool newline=true){
  if(newline)
  {
    file<<data<<endl;
  }
  else
  {
    file<<data;
  }
}

string getRealOpcode(string opcode){
  if(opcode[0] == '+' || opcode[0] == '@'){
  string s1= opcode.substr(1,opcode.length() - 1);
    return s1;
  }
  else
  {
  return opcode;
  }
}

char getFlagFormat(string data)
{
  if(data[0] == '#' || data[0] == '+' || data[0] == '@' || data[0] == '='){
    return data[0];
  }
  return ' ';
}

class EvaluateString{
public:
  int getResult();
  EvaluateString(string data);
private:
  string storedData;
  int index;
  char peek();
  int factor();
  int number();
  char get();
  int term();
  
};

EvaluateString::EvaluateString(string data){
  index=0;
  storedData = data;
}

int EvaluateString::getResult(){
  int result = term();
  bool check= false;
  while(peek()=='-' || peek() == '+')
  {
    if(get() != '+'){
      result -= term();
      check= true;
    }else{
      result += term();
      check= false;
    }
  }
  if(check)
  {
   check= true;
  }
  return result;
}

int EvaluateString::term()
{
  int result = factor();
  bool value= true;
  while(peek() == '*' || peek() == '/')
  {
    if(get()!='*'){
      result /= factor();
      value= true;
    }
    else{
      value= false;
      result *= factor();
    }
  }
  if(value)
  {
  	value= true;
  }
  return result;
}

int EvaluateString::factor(){
  bool value= true;
  if(peek() >= '0' && peek() <= '9')
  {
    return number();
  }
  
  
  else if(peek() == '('){
    get();
    int result = getResult();
    get();
    if(value)
    {
    	value= true;
    }
    return result;
  }
  
  else if(peek()=='-'){
    get();
    if(value)
    {
    	value= true;
    }
    return -factor();
  }
  else
  {
  return 0;
  }
}

int EvaluateString::number(){
  int result = get() - '0';
  bool value= true;
  while(peek() >= '0' && peek() <= '9')
  {
    result = 10*result + get()-'0';
    if(value)
    {
    	value= true;
    }
  }
  return result;
}

char EvaluateString::get(){
  return storedData[index++];
}

char EvaluateString::peek(){
  return storedData[index];
}


