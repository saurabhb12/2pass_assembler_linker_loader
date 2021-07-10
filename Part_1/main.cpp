#include <bits/stdc++.h>

using namespace std;

map<string, string> OpTab; 
map<string, string> EsTab; //Used to store name and address of symbols
string label;
string opcode;
string operand;

int program_length;     //Length of the program
int program_start_addr; //Start address of the program

int PROGADDR; //Program address
int CSADDR;   //Control section address

ifstream fin;  
ofstream fout; 

bool extended_flag;
string line;
string progam_name; 



vector<string> words;
string word;




  
    void print_text(string temp)
    {
        fout << temp;
        return;
    }

 
    void print_text(char c)
    {
        fout << c;
        return;
    }
    
    
     // Converts hexadecimal to decimal
    int hex2dec(string s)
    {
        int mul = 1;
        int temp = 0;
        for (int i=s.size()-1; i>=0; i--)
        {
            int base;
            if (!(s[i] >= 'A' && s[i] <= 'F'))
                base = s[i] - '0';
                
            else
                base = s[i] - 'A' + 10;
               
            int pt= base*mul;
            temp = temp+ ( pt );
            mul = mul * 16;
        }
        return temp;
    }
    
    
      

    // Use to fill the opcode table
    void optable_fill()
    {
        while (1)
        {
            if (fin.eof())
                break;

            getline(fin, line);
        stringstream ss(line);
        words.clear();
        while (ss >> word)
        {
            words.push_back(word);
        }
            OpTab[words[0]] = words[1];
        }
        return;
    }
  
   

    // Converts decimal to hexadecimal value
    string dec2hex(long long num)
    {
        string hex = "";
        while (1)
        {
            if(num==0)
            {
              break;
            }
            int rem = num % 16;
            if(rem<10)
            {
              rem += 48;
            }
            else 
            {
              rem= rem + 55;
            }
            char d=  char(rem);
            hex.push_back(d);
            num = num/16;
        }
        reverse(hex.begin(), hex.end());
        return hex;
    }
    
    void fill_code(int start_address, string &s, string &c)
    {
        int limit = (int)c.size();
        for(int i=0; i<limit; i++)
        {
            s[start_address + i] = c[i];
        }

        return;
    }
    // Strip the blank spaces from the string
    string stripString(string s)
    {

        while ((int)s.size() > 0 && s.back() == ' ')
            s.pop_back();

        if (s.length() == 0)
            return "";

        int i = 0;
        while (1)
        {
            if (s[i] != ' ')
                break;
            i++;
        }

        return s.substr(i);
    }

    // Get a specific portion of a string
    string getSubstring(string &str, int start_idx, int len = -1)
    {
        string res = "";
        bool val= true;
        if(val)
        {
          val= true;
        }
        if (len == -1)
            len = int(str.size()) - start_idx;
            
        for(int i= start_idx; i<= (start_idx+len-1); i++)
        {
          res.push_back(str[i]);
        }

        return res;
    }

    

    string getBlock(string &str, int len, char dummy, bool end)
    {
        string res = str;
        if (str.size() > len)
        {
            return res.substr(0, len);
        }
        if (str.size() == len)
            return res;

        if (end==false)
        {
            reverse(res.begin(), res.end());
            while (1)
            {
               if(res.size() >= len)
               {
               break;
               }
                res.push_back(dummy);
            }
            reverse(res.begin(), res.end());
           
        }
        else
        {
            while (1)
            {
               if(res.size() >= len)
               {
               break;
               }
                res.push_back(dummy);
            }
        }

        return res;
    }
    
    bool equality_check(char a, char b)
    {
        return a==b;
    }

    bool equality_check(string a, string b)
    {
        return a==b;
    }

    bool divisibility_check(int a, int b)
    {
        if(a%b==0)
        {
           return true;
        }
        return false;
    }

    // Decompose the line into opcode label and operand
    
    bool decomposition_line(bool &extended_flag, string &label, string &line, string &operand, string &opcode)
    {
        string temp = stripString(line);
        if (temp.size() == 0)
            return false;
        if (temp[0] != '.' && (line.size() >= 81  || line.size() <= 20))
            return false;
            
        else if (temp[0] == '.')
            return true;

        line = getBlock(line, 80, ' ', 1);
        label = stripString(line.substr(0, 10));
        opcode = stripString(line.substr(10, 10));
        operand = stripString(line.substr(20));
        

        if (opcode[0] == '+')
        {
            opcode.erase(0, 1);
            extended_flag = true;
        }
        else
        {
            extended_flag = false;
        }

        return true;
    }


int main()
{
    //========================== Filling Opcode Table ==================================

    fin.open("opcodes.txt");
    optable_fill();
    fin.close();


    CSADDR = PROGADDR;

    //========================== PASS 1 Starting Here ==================================

    cout << "Pass-1 of Linker-Loader started" << endl;
    fin.open("assembler_output.txt");
    while (getline(fin, line))
    {
        string temp= stripString(line);
        line = temp;
        if (line.empty())
            continue;
        string s1= getSubstring(line, 13, 6);
        int CSLTH = hex2dec(s1);
        string s2= getSubstring(line, 7, 6);
	program_start_addr = hex2dec(s2);
	string s3= getSubstring(line, 1, 6);
        progam_name = stripString(s3);
        char top = line[0];
        
        EsTab[progam_name] = dec2hex(CSADDR);
        while (getline(fin, line))
        {
            if (stripString(line).empty())
                continue;
            
            bool val= true;
            if(val)
            {
              val= false;
            }
            line = stripString(line);
            if (equality_check('D', line[0]) && line.size()>1)
            {
                    string symbol_str = getSubstring(line, 1);
                    int i = 0;
                    int limit = symbol_str.length();

                    while (1)
                    {
                        if(i>=limit)
                        {
                          break;
                        }
                        string addr = getSubstring(symbol_str, i + 6, 6);
                        string symbol = getSubstring(symbol_str, i, 6);
                        i= i+12;
                        EsTab[symbol] = dec2hex(hex2dec(addr) + CSADDR);
                    }
            }
            
            if (equality_check('E', line[0]))
                break; 
        }
        CSADDR += CSLTH;
    }
    fin.close();
    
    cout << "Linking-Loader PASS 1 ended" << endl;

    //========================== PASS 1 Ended Here ====================================

    //========================== PASS 2 Starting Here ==================================

    cout << "Pass2 of Linker-Loader started" << endl;
    
    fin.open("assembler_output.txt");

    int ass1= 2*CSADDR +31;
    ass1= ass1/32;
    int mem_size = (ass1) * 32;
    
    
    string rand_acc_mem= "";
    for(int i=0; i<mem_size; i++)
    {
       rand_acc_mem.push_back('.');
    }
    int EXECADDR = PROGADDR;
    CSADDR = PROGADDR;

    while (getline(fin, line))
    {
        line = stripString(line);
        if (line.length()==0)
            continue;

        int CSLTH = hex2dec(getSubstring(line, 13, 6));

        while (getline(fin, line))
        {
            if (stripString(line).size()==0)
                continue;
            line = stripString(line);

            if (equality_check(line[0], 'M'))
            {
                 char sign = line[9];
                string symbol = getSubstring(line, 10);

                int add_value = hex2dec(EsTab[symbol]);
                int pt= hex2dec(getSubstring(line, 1, 6));
                pt= pt +CSADDR;
                int modif_address = 2*pt;

                int modif_length = hex2dec(getSubstring(line, 7, 2));
                if (modif_length%2==1)
                    modif_address++;

                int limit = modif_address + modif_length;
                int it = modif_address;
                string modif_value;
                for(; it<limit; it++)
                {
                    modif_value += rand_acc_mem[it];
                }
                bool val= true;
                if(val)
                {
                   val= false;
                }
                bool flag = equality_check(sign, '+');
                int integer_helper = hex2dec(modif_value);
                if(flag==0)
                {
                  integer_helper = integer_helper -add_value;
                }
                else 
                {
                	integer_helper = integer_helper + add_value;
                }
                

                modif_value = dec2hex(integer_helper);
                modif_value = getBlock(modif_value, modif_length, '0', 0);

		limit = modif_address + modif_length;
                it = modif_address;
                if(val)
                {
                   val= false;
                }
                for(; it<limit; it++)
                {
                    rand_acc_mem[it] = modif_value[it - modif_address];
                }
            }

            if (equality_check(line[0], 'T'))
            {
                string code = getSubstring(line, 9);
                int pp= hex2dec(getSubstring(line, 1, 6));
                pp+=CSADDR;
                int start_address = 2*pp;
                fill_code(start_address, rand_acc_mem, code);
            }

            if (equality_check(line[0], 'E'))
            {
                if (line.length() >= 2)
                {
                    EXECADDR = CSADDR + hex2dec(getSubstring(line, 1));
                }
                break;
            }
        }
        CSADDR +=  CSLTH;
    }

    fin.close();
     CSADDR = 0;
     fout.open("linking_loader_output.txt");
    fin.open("assembler_intermediate.txt");
    
    while (getline(fin, line))
    {
        string temp = stripString(line);
        bool val= true;
        if(val)
        {
          val= false;
        }
        if ( temp[0] == '.' || temp.empty())
            continue;
        string instruction_line = getSubstring(line, 14);
        decomposition_line( extended_flag,  label, instruction_line, operand, opcode);
        extended_flag = false;

        if ( equality_check("START", opcode)|| equality_check("CSECT", opcode))
        {
            string hello= getSubstring(line, 14, 10);
            hello= stripString(hello);
            int temp1= hex2dec(EsTab[hello]);
            CSADDR = temp1;
            continue;
        }

        if (OpTab.find(opcode) != OpTab.end() || equality_check("WORD", opcode) || equality_check("BYTE", opcode) || equality_check('=', opcode[0]))
        {
            string s1= getSubstring(line, 0, 6);
            s1= stripString(s1);
            int rr= hex2dec(s1);
            rr+=CSADDR;
            int address = 2 *rr;
            int len = 2 * stoi(stripString(getSubstring(line, 7, 6)));
            
            int i = address;
            int limit =len + i;
            for(; i<limit; i++)
            {
                print_text(rand_acc_mem[i]);
            }
            print_text('\n');
        }
    }
    fin.close();
    fout.close();
    fout.open("linking_loader_mem_map.txt");

    int i = 0;
    while (1)
    {
        if(i>=mem_size)
        {
          break;
        }
        if (i%(1<<5)==0)
        {
            if (i != 0)
            {
                print_text('\n');
            }
            int num = i >> 1;
            string hexAddr = dec2hex(num);
            int cheems= 0;
            for(int i=0; i<12; i++)
            {
              cheems++;
            }
            hexAddr = getBlock(hexAddr, 6, '0', 0);
            print_text(hexAddr);
            print_text(" ");
        }
        else if (i%(1<<3)==0)
        {
            (" ");
        }
        print_text(rand_acc_mem[i]);
        i++;
    }

    fout.close();
    cout << "Linking-Loader PASS 2 ended" << endl;

    //========================== PASS 2 Ends Here ====================================

    return 0;
}
