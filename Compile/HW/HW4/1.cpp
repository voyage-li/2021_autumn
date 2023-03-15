#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

/********************************************************
 *                                                      *
 *                第一部分：词法分析                       *
 *                                                      *
 *******************************************************/
// 关键字表置初始值
string Cppkeyword[100] = {"#", "标识符(变量名)", "整数", "实数", "字符常量", "+", "-", "*", "/", "<",
                          "<=", "==", "!=", ">=", ">", "&", "&&", "||", "=", "(",
                          ")", "[", "]", "{", "}", ":", ";", ",", "@", "!",
                          "void", "int", "float", "char", "if", "else", "while", "do", "for", "true",
                          "false", "using", "namespace", "std", "main", "return", "null"};
class word
{
public:
    int syn{};
    string token;
};
//存储词法分析结果
word lexicalTable[1000];
int lexicalTableLen = 0;
//定位二元式在哪源代码的哪一行
vector<int> whichLine;

// 处理关键词和变量的函数
word letterAnalysis(const string &subCode)
{
    word item;
    int isKeyword = 0;
    for (int i = 30; i <= 46; i++)
    {
        if (subCode.substr(0, Cppkeyword[i].length()) == Cppkeyword[i])
        {
            item.syn = i;
            isKeyword = 1;
        }
    }
    /* 不用上述for循环的话就得如下一一列举，如果关键词数目较多，就要写很多低级代码
    if (subCode.substr(0, 4) == "void")
    {
        item.syn = 28;
    }
    else if (subCode.substr(0, 3) == "int")
    {
        item.syn = 29;
    }
    else if (subCode.substr(0, 5) == "float")
*/
    if (isKeyword == 0)
    {
        // 如果不是上述关键词，一律视为变量名
        for (int i = 0; i <= subCode.length(); ++i)
        { //找到第一个不是数字、字母、下划线的位置
            if (!(subCode[i] >= 'a' && subCode[i] <= 'z' || subCode[i] >= 'A' && subCode[i] <= 'Z' || subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '_'))
            {
                item.syn = 1; // 1号位存储变量名
                Cppkeyword[item.syn] = subCode.substr(0, i);
                break;
            }
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理数字的函数
word numberAnalysis(string subCode)
{
    word item;
    for (int i = 0; i <= subCode.length(); ++i)
    {
        // 截取到第一个非数字和小数点字符
        if (!(subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '.'))
        {
            string curNum = subCode.substr(0, i);
            if (curNum.find('.') == string::npos) //没读到'.'返回值为很大的数，若读到返回值是第一次出现的下标
                item.syn = 2;                     // 2号位存整数
            else
                item.syn = 3; // 3号位存实数

            Cppkeyword[item.syn] = curNum;
            break;
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理字符常量的函数
word strAnalysis(string subCode)
{ //"hello"<<endl;
    word item;
    int nextindex = subCode.find_first_of('"', 1); //找到第二个"出现的位置下标
    item.syn = 4;                                  //字符常量置为4
    Cppkeyword[item.syn] = subCode.substr(0, nextindex + 1);

    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理字符的函数
word charAnalysis(string subCode)
{
    word item;
    switch (subCode[0])
    {
    case '#':
        item.syn = 0;
        break;
    case '+':
        item.syn = 5;
        break;
    case '-':
        item.syn = 6;
        break;
    case '*':
        item.syn = 7;
        break;
    case '/':
        item.syn = 8;
        break;
    case '<':
        if (subCode[1] == '=')
        {
            item.syn = 10;
        }
        else
        {
            item.syn = 9;
        }
        break;
    case '=':
        if (subCode[1] == '=')
        {
            item.syn = 11;
        }
        else
        {
            item.syn = 18;
        }
        break;
    case '!':
        if (subCode[1] == '=')
        {
            item.syn = 12;
        }
        else
            item.syn = 29;
        break;

    case '>':
        if (subCode[1] == '=')
        {
            item.syn = 13;
        }
        else
        {
            item.syn = 14;
        }
        break;
    case '&':
        if (subCode[1] == '&')
        {
            item.syn = 16;
        }
        else
        {
            item.syn = 15;
        }
        break;
    case '|':
        if (subCode[1] == '|')
        {
            item.syn = 17;
        }
        break;

    case '(':
        item.syn = 19;
        break;
    case ')':
        item.syn = 20;
        break;
    case '[':
        item.syn = 21;
        break;
    case ']':
        item.syn = 22;
        break;
    case '{':
        item.syn = 23;
        break;
    case '}':
        item.syn = 24;
        break;
    case ':':
        item.syn = 25;
        break;
    case ';':
        item.syn = 26;
        break;
    case ',':
        item.syn = 27;
        break;
    case '@':
        item.syn = 28;
        break;
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 词法分析
void scanner(const string &code)
{ // if a=1;

    for (int i = 0; i < code.length(); ++i)
    {
        word item;
        if (code[i] >= 'a' && code[i] <= 'z' || code[i] >= 'A' && code[i] <= 'Z')
        { // 处理单词,假设句子是 if a=1;进行单词分析后返回“if”,i后移了两位，这点在该函数最后有做处理
            item = letterAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] >= '0' and code[i] <= '9')
        { // 处理数字
            item = numberAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] == ' ')
        { // 如果是空格，直接跳过
            continue;
        }
        else
        { // 处理特殊符号
            item = charAnalysis(code.substr(i, code.length() - i + 1));
        }
        i += int(item.token.length()) - 1;
        lexicalTable[lexicalTableLen++] = item; //词法处理完的结果存入lexicalTable中
        cout << "(" << item.syn << "," << item.token << ")" << endl;
    }
}
/****************************************************************
 *                                                              *
 *        第二部分、语法分析前的一些准备工作,主要包括:                 *
 *      (1) 扫描文法，获取文法中出现的所有的Vn和Vt                    *
 *      (2) 将文法中的产生式拆分为左部、右部,其中右部采用vector进行保存  *
 *      (3) 构造所有Vn和Vt的first集和follow集                      *
 *                                                              *
 ****************************************************************/
vector<string> grammar;          //存储文法
map<string, int> VN2int, VT2int; // VN、VT映射为下标索引
int symbolNum = 0;
map<string, bool> nullable; //各终结符或非终结符是否可空
set<string> first[50];      //存储各Vn和Vt的first集，没错，Vt也构造first集，就是其自身
set<string> follow[50];     //存储各Vn和Vt的follow集，Vt的follow都是空，240~263行取消注释可查看Vt的first和follow集

string getVn(string grammar)
{ //获取文法中的非终结符

    if (grammar.substr(0, 2) == "<<")
    { //处理形如"<<exp>>"格式的非终结符
        int bracketsDelimiter = grammar.find(">>");
        string Vn = grammar.substr(0, bracketsDelimiter + 2);
        return Vn;
    }
    if (grammar[1] == '\'')
    { //带'的非终结符,如 E',T'
        return grammar.substr(0, 2);
    }
    else
    { //不带'的正常非终结符，如E，T
        return grammar.substr(0, 1);
    }
}

string getVt(string grammar)
{ //获取文法中的终结符
    for (int k = 0; k <= 29; k++)
    { //这里应该注意：使用最长匹配。举例：遇到>=会优先匹配>，这并非所要的
        if (grammar.substr(0, 2) == Cppkeyword[k])
            return grammar.substr(0, 2);
    }
    for (int k = 0; k <= 29; k++)
    { //使用最长匹配
        if (grammar.substr(0, 1) == Cppkeyword[k])
            return grammar.substr(0, 1);
    }
    for (int k = 30; k <= 46; k++)
    { //使用最长匹配
        string Vt = grammar.substr(0, Cppkeyword[k].length());
        if (Vt == Cppkeyword[k])
        {
            return Vt;
        }
    }
    //如果运行到这里，说明这个终结符不是关键词表里的，认为小写字母也属于终结符
    if (grammar[0] >= 'a' && grammar[0] <= 'z')
    {
        return grammar.substr(0, 1);
    }
    return "";
}

void readVnAndVt()
{
    //扫描一个产生式，识别所有的非终结符和终结符
    for (int i = 0; i < grammar.size(); i++)
    {
        for (int j = 0; j < grammar[i].length(); j++)
        {
            if (grammar[i][j] == ' ')
            {
                continue;
            }
            else if (grammar[i].substr(j, 2) == "<<")
            { //处理形如"<<exp>>"格式的非终结符
                int bracketsDelimiter = grammar[i].substr(j, grammar[i].length() - j).find(">>");
                string Vn = grammar[i].substr(j, bracketsDelimiter + 2);
                if (VN2int[Vn] == 0)
                    VN2int[Vn] = ++symbolNum;
                j = j + Vn.length() - 1;
            }
            else if (grammar[i][j] >= 'A' && grammar[i][j] <= 'Z')
            { //非终结符一般大写
                string Vn = getVn(grammar[i].substr(j, 2));
                if (VN2int[Vn] == 0)
                    VN2int[Vn] = ++symbolNum;
                j = j + Vn.length() - 1;
            }
            else if (grammar[i].substr(j, 2) == "->")
            {
                j = j + 2 - 1;
            }
            else
            { //扫描产生式右部的可能的终结符(关键词表)
                string Vt = getVt(grammar[i].substr(j, grammar[i].length() - j));
                if (VT2int[Vt] == 0)
                    //该终结符第一次出现,将该终结符映射为下标索引
                    VT2int[Vt] = ++symbolNum;
                j = j + Vt.length() - 1;
            }
        }
    }

    cout << "非终结符VN:" << endl;
    for (auto it = VN2int.begin(); it != VN2int.end(); it++)
    {
        cout << "索引下标:" << it->second << "\t名称：" << it->first << endl;
    }
    cout << "终结符VT:" << endl;
    for (auto it = VT2int.begin(); it != VT2int.end(); it++)
    {
        cout << "索引下标:" << it->second << "\t名称：" << it->first << endl;
    }
}

vector<string> splitGrammarIntoYi(string rightGrama)
{ //将产生式的右部(左部->右部拆分)：X->Y1Y2...Yk
    vector<string> Y;

    for (int j = 0; j < rightGrama.length(); j++)
    {
        if (rightGrama[j] == ' ')
        {
            continue;
        }
        if (rightGrama[j] >= 'A' && rightGrama[j] <= 'Z' || rightGrama.substr(j, 2) == "<<")
        { //非终结符
            string Vn = getVn(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vn);
            j = j + Vn.length() - 1;
        }
        else
        { //终结符
            string Vt = getVt(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vt);
            j = j + Vt.length() - 1;
        }
    }
    return Y;
}

void split(string grama, string &X, vector<string> &Y)
{
    int delimiterIndex = grama.find("->");
    X = grama.substr(0, delimiterIndex);
    // trim()功能，C++不带，只能自己实现，剪除首尾的空格符号
    X.erase(0, X.find_first_not_of(" "));
    X.erase(X.find_last_not_of(" ") + 1);
    string rightGrama = grama.substr(delimiterIndex + 2, grama.length() - delimiterIndex - 2);
    Y = splitGrammarIntoYi(rightGrama);
}

bool allNullable(vector<string> Y, int left, int right)
{ //判断 Y[left]...Y[right]是否全可空
    if (left >= Y.size() || left > right || right < 0)
        return true;
    for (int i = left; i <= right; i++)
    {
        if (nullable[Y[i]] == false)
            return false;
    }
    return true;
}

void getFirstFollowSet()
{
    /*计算FIRST、FOLLOW、nullable的算法*/
    for (auto it = VT2int.begin(); it != VT2int.end(); it++)
    { //对每一个终结符Z，first[Z]={Z}
        string Vt = it->first;
        int Vt_index = it->second;
        first[Vt_index].insert(Vt);
    }
    for (int grammarIndex = 0; grammarIndex < grammar.size(); grammarIndex++)
    {
        //对于每个产生式：X->Y1Y2...Yk
        string X;
        vector<string> Y;
        int delimiterIndex = grammar[grammarIndex].find("->");
        X = grammar[grammarIndex].substr(0, delimiterIndex);
        X.erase(0, X.find_first_not_of(" "));
        X.erase(X.find_last_not_of(" ") + 1);                                                                                      //以"->"为界，分隔产生式
        string rightGrama = grammar[grammarIndex].substr(delimiterIndex + 2, grammar[grammarIndex].length() - delimiterIndex - 2); //提取左部产生式
        Y = splitGrammarIntoYi(rightGrama);

        int k = Y.size();
        nullable["null"] = true;
        //如果所有Yi都是可空的，则nullable[X]=true
        if (allNullable(Y, 0, k - 1))
        {
            nullable[X] = true;
        }

        for (int i = 0; i < k; i++)
        {
            //如果Y0...Y(i-1)都是可空的(言外之意Yi不空),则first[X] = first[X]∪first[Yi] (1)
            if (nullable[Y[i]] == false && allNullable(Y, 0, i - 1))
            {
                if (i <= k - 1)
                {
                    set<string> setX = first[VN2int[X]];
                    //判断Yi是终结符还是非终结符
                    set<string> setY = VT2int.count(Y[i]) != 0 ? first[VT2int[Y[i]]] : first[VN2int[Y[i]]];
                    set_union(setX.begin(), setX.end(), setY.begin(), setY.end(), inserter(setX, setX.begin())); //(1)
                    first[VN2int[X]] = setX;
                }
            }
            //如果Y(i+1)...Yk都是可空的(言外之意Y0..Y(i-1)都不空)，则follow[Yi] = follow[Yi]∪follow[X] (2)
            if (allNullable(Y, i + 1, k - 1))
            {
                set<string> setX = follow[VN2int[X]];
                //判断Yi是终结符还是非终结符
                set<string> setY = VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]];
                set_union(setX.begin(), setX.end(), setY.begin(), setY.end(), inserter(setY, setY.begin()));
                VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]] = setY;
            }

            for (int j = i + 1; j < k; j++)
            {
                //如果Y(i+1)...Y(j-1)都是可空的(言外之意Yj不空),则follow[Yi] = follow[Yi]∪first[Yj] (3)
                if (nullable[Y[j]] == false && allNullable(Y, i + 1, j - 1))
                {
                    if (j <= k - 1)
                    {
                        set<string> setYi = VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]];
                        set<string> setYj = VT2int.count(Y[j]) ? first[VT2int[Y[j]]] : first[VN2int[Y[j]]];
                        set_union(setYi.begin(), setYi.end(), setYj.begin(), setYj.end(), inserter(setYi, setYi.begin()));
                        VT2int.count(Y[i]) ? follow[VT2int[Y[i]]] : follow[VN2int[Y[i]]] = setYi;
                    }
                }
            }
        }
    }
}

void converge()
{ //迭代计算first和follow集，直到收敛
    set<string> oldFirst[50];
    set<string> oldFollow[50];
    int isConverge = 1;
    string _vn = getVn(grammar[0]);
    //这是一个可以手动修改的地方，很多教材的终止符不一样，这里统一一下，都用#作为终止符
    follow[VN2int[_vn]].insert("#");
    int times = 1; //经过多少轮才收敛
    do
    { //非终结符的first、follow不再变化则收敛
        isConverge = 1;
        getFirstFollowSet();
        // VN的状态
        for (auto it = VN2int.begin(); it != VN2int.end(); it++)
        {
            int vnindex = it->second;
            if (oldFirst[vnindex].size() != first[vnindex].size() || oldFollow[vnindex].size() != follow[vnindex].size())
                isConverge = 0;
            //保存旧状态，以便之后和新状态比较是否变化判断收敛与否
            oldFirst[vnindex] = first[vnindex];
            oldFollow[vnindex] = follow[vnindex];
        }
    } while (isConverge != 1);
    //输出最终结果
    cout << endl;
    for (auto it = VN2int.begin(); it != VN2int.end(); it++)
    {
        int vnindex = it->second;
        if (oldFirst[vnindex].size() != first[vnindex].size() || oldFollow[vnindex].size() != follow[vnindex].size())
        {
            isConverge = 0;
        }
        //输出状态
        cout << it->first << "的first集：\t";
        for (auto first_it = first[vnindex].begin(); first_it != first[vnindex].end(); first_it++)
        {
            cout << *first_it << " ";
        }
        cout << endl;
        cout << it->first << "的follow集：\t";
        for (auto follow_it = follow[vnindex].begin(); follow_it != follow[vnindex].end(); follow_it++)
        {
            cout << *follow_it << " ";
        }
        cout << endl;
    }
}

/****************************************************************
 *                                                              *
 *                  第三部分：构造文法的LR1分析表                   *
 *          关键函数：(1) closure():计算项集的LR1闭包               *
 *                  (2) GOTO():由项集I读入Vn或Vt转向项集J          *
 *                                                              *
 ****************************************************************/
struct term
{                 // LR1项集内部的项
    int termType; //移进项目、待约项目、规约项目
    string leftPart;
    vector<string> rightPart;
    int dotPos{-1};             //活前缀在右部的位置,初始化为-1
    vector<string> subsequence; // LR1文法才用到的后继符,随便初始化一个字符串
    bool operator==(const term &b) const
    {
        if (leftPart == b.leftPart && rightPart == b.rightPart && dotPos == b.dotPos)
        {
            if (subsequence == b.subsequence)
                return true;
        }
        return false;
    }
};
const int maxN = 4000;        //预定最大有maxN个状态，不够再加，但最好不要超过10000，不然可能会发生未知错误
vector<term> statusSet[maxN]; //项集
int globalStatusNum = 1;      //项集编号
int actionTable[maxN][50];    // action表，行表示状态，列表示终结符。这里预定最多50个终结符，应该够用了
int gotoTable[maxN][50];      // goto表，行表示状态，列表示非终结符

void initGrammar()
{
    /*
    grammar.push_back("P-><<statement>>"); //0 P->S
    //if控制语句
    grammar.push_back("<<statement>>->C<<statement>>"); //S->C S
    grammar.push_back("C->if(<<BE>>)");                 //C->if(BE)
    grammar.push_back("<<statement>>->T<<statement>>"); //S->T S
    grammar.push_back("T->C<<statement>>else");         //T->C S else
    //grammar.push_back("<<statement>>->{<<statement>>}"); //S->{ S }，在复合语句里面完成了
    //do-while循环控制语句
    grammar.push_back("D->do");                    //D->do
    grammar.push_back("U->D<<statement>>while");   //U->D S while
    grammar.push_back("<<statement>>->U(<<BE>>)"); //S->U(BE)
    //while循环控制语句
    grammar.push_back("W->while");                           //W->while
    grammar.push_back("<<Wd>>->W(<<BE>>)");                  //Wd->W(E)
    grammar.push_back("<<statement>>-><<Wd>><<statement>>"); //S->Wd S
    //复合语句
    grammar.push_back("L-><<statement>>");        //L->S
    grammar.push_back("<<Ls>>->L;");              //Ls ->L;    每一个单独的结构都要以;结尾，不然就不要使用顺序结构,且要使用顺序结构，还要在一个块{}中
    grammar.push_back("L-><<Ls>><<statement>>");  //L->Ls S
    grammar.push_back("<<statement>>->{<<Ls>>}"); //S->{ Ls }  这里算是一个勘误。用ppt上的S->{L}不可行，遇到"L;}"就无法规约"L;"。改为Ls就可以了
    //布尔表达式
    grammar.push_back("<<statement>>-><<BE>>");        //11 S->B
    grammar.push_back("<<BE>>-><<BEor>><<BT>>");       //12
    grammar.push_back("<<BEor>>-><<BE>>||");           //13
    grammar.push_back("<<BE>>-><<BT>>");               //14
    grammar.push_back("<<BT>>-><<BTand>><<BF>>");      //15
    grammar.push_back("<<BTand>>-><<BT>>&&");          //16
    grammar.push_back("<<BT>>-><<BF>>");               //17
    grammar.push_back("<<BF>>->(<<BE>>)");             //18
    grammar.push_back("<<BF>>->!<<BF>>");              //19
    grammar.push_back("<<BF>>-><<AEXPR>><<<AEXPR>>");  //20 关系运算符优先级高于布尔运算，低于算术运算
    grammar.push_back("<<BF>>-><<AEXPR>><=<<AEXPR>>"); //21
    grammar.push_back("<<BF>>-><<AEXPR>>==<<AEXPR>>"); //22
    grammar.push_back("<<BF>>-><<AEXPR>>!=<<AEXPR>>"); //23
    grammar.push_back("<<BF>>-><<AEXPR>>><<AEXPR>>");  //24
    grammar.push_back("<<BF>>-><<AEXPR>>>=<<AEXPR>>"); //25
    grammar.push_back("<<BF>>->i<i");                  //26
    grammar.push_back("<<BF>>->i<=i");                 //27
    grammar.push_back("<<BF>>->i==i");                 //28
    grammar.push_back("<<BF>>->i!=i");                 //29
    grammar.push_back("<<BF>>->i>i");                  //30
    grammar.push_back("<<BF>>->i>=i");                 //31
    grammar.push_back("<<BF>>->i");                    //32
    //赋值表达式
    grammar.push_back("<<statement>>->i=<<AEXPR>>");    //1 S->i=E
    grammar.push_back("<<AEXPR>>->@<<AEXPR>>");         //2,E->@E
    grammar.push_back("<<AEXPR>>-><<AEXPR>>+<<TERM>>"); //3 E->E+T
    grammar.push_back("<<AEXPR>>-><<AEXPR>>-<<TERM>>"); //4 E->E-T
    grammar.push_back("<<AEXPR>>-><<TERM>>");           //5 E->T
    grammar.push_back("<<TERM>>-><<TERM>>*<<FACTOR>>"); //6 T->T*F
    grammar.push_back("<<TERM>>-><<TERM>>/<<FACTOR>>"); //7 T->T/F
    grammar.push_back("<<TERM>>-><<FACTOR>>");          //8 T->F
    grammar.push_back("<<FACTOR>>->i");                 //9 F->i
    grammar.push_back("<<FACTOR>>->(<<AEXPR>>)");       //10 F->(E)
*/
    grammar.push_back("E'->E");
    grammar.push_back("E->E+T");
    grammar.push_back("E->T");
    grammar.push_back("T->TF");
    grammar.push_back("T->F");
    grammar.push_back("F->F*");
    grammar.push_back("F->a");
    grammar.push_back("F->b");
}

//该函数作用:项集I读入Vn或Vt可能会生成新的项集J，但也有可能指向已有项集，该函数就是判断是否指向已有项集
int mergeSet()
{
    int flag = -1; //假定不可以进行合并
    for (int i = 0; i < globalStatusNum - 1; i++)
    {
        if (statusSet[globalStatusNum - 1].size() != statusSet[i].size())
            continue;
        flag = i; //可以和状态集i合并
        for (int j = 0; j < statusSet[globalStatusNum - 1].size(); j++)
        {
            if (!(statusSet[i][j] == statusSet[globalStatusNum - 1][j]))
            {
                flag = -1;
            }
        }
        if (flag != -1) //第一次遇到可以合并的集合就结束，不然等到后面会遇到项目数相等但状态集不同的情况
            return flag;
    }
    return -1;
}

void initI0()
{
    term firstTerm;
    string X;
    vector<string> Y;
    split(grammar[0], X, Y);
    if (firstTerm.dotPos == -1) //如果没有活前缀"·"
        firstTerm.dotPos = 0;   //就添加活前缀
    firstTerm.leftPart = X;
    firstTerm.rightPart = Y;
    firstTerm.subsequence.push_back("#");
    statusSet[0].push_back(firstTerm);
}

//相对于LR0和SLR，LR1在closure这个函数中有大改动，几乎是重写
void closure(int statusNum)
{ //计算LR1文法的项集闭包
    queue<term> termQueue;
    for (int i = 0; i < statusSet[statusNum].size(); i++)
        termQueue.push(statusSet[statusNum][i]);
    while (!termQueue.empty())
    { //对项集I中每个项[A->a·B beta,alpha]
        term curTerm = termQueue.front();
        if (curTerm.dotPos == curTerm.rightPart.size()) //如果是规约项，跳过并弹出该项
        {
            termQueue.pop();
            continue;
        }
        string B = curTerm.rightPart[curTerm.dotPos];
        for (int j = 0; j < grammar.size(); j++)
        { //对增广文法G'中的每个产生式B->gamma
            if (B != getVn(grammar[j].substr(0, grammar[j].length())))
                continue;
            //将[B->·gamma,b]加入集合I中，其中b是FIRST[beta alpha]中的终结符
            term newTerm;
            newTerm.dotPos = 0;
            split(grammar[j], newTerm.leftPart, newTerm.rightPart);
            // warning:好像没考虑B->null的情况，还是说已经考虑了(null看成终结符)
            if (VT2int.count(newTerm.rightPart[0]) != 0) // B->·bA，移进项目
                newTerm.termType = 2;
            else if (VN2int.count(newTerm.rightPart[0]) != 0) // B->b·A,待约项目
                newTerm.termType = 3;
            //找b
            string beta;
            vector<string> b;
            if (curTerm.dotPos == curTerm.rightPart.size() - 1)
            { //如果beta不存在，b即alpha
                b = curTerm.subsequence;
            }
            else if (VT2int.count(curTerm.rightPart[curTerm.dotPos + 1]) != 0)
            { //如果beta存在，b为first(beta)。特例：beta为终结符时，b就是beta
                b.push_back(curTerm.rightPart[curTerm.dotPos + 1]);
            }
            else
            { //遍历first(beta)中的终结符b
                beta = curTerm.rightPart[curTerm.dotPos + 1];
                for (auto it = first[VN2int[beta]].begin(); it != first[VN2int[beta]].end(); it++)
                {
                    b.push_back(*it);
                }
            }
            newTerm.subsequence = b;
            //只有闭包生成的新项B->gamma,不在集合I中才加入；在集合I中的只要相应的增加后继符
            int newTermFlag = -1; //先假设不在集合I中
            for (int k = 0; k < statusSet[statusNum].size(); k++)
            {
                if (newTerm.leftPart == statusSet[statusNum][k].leftPart && newTerm.rightPart == statusSet[statusNum][k].rightPart && newTerm.dotPos == statusSet[statusNum][k].dotPos)
                    newTermFlag = k;
            }
            if (newTermFlag == -1)
            { //不在集合I中就加入
                termQueue.push(newTerm);
                statusSet[statusNum].push_back(newTerm);
            }
            else
            { //如果新项B->gamma在集合I中，就新增其后继符(如果有新后继符的话)
                map<string, int> subsequenceMap;
                for (int m = 0; m < statusSet[statusNum][newTermFlag].subsequence.size(); m++)
                {
                    subsequenceMap[statusSet[statusNum][newTermFlag].subsequence[m]]++;
                }
                for (int m = 0; m < newTerm.subsequence.size(); m++)
                {
                    if (subsequenceMap[newTerm.subsequence[m]] == 0)
                        statusSet[statusNum][newTermFlag].subsequence.push_back(newTerm.subsequence[m]);
                }
            }
        }
        termQueue.pop();
    }
}

// GOTO函数无须变动，不管是LR0,SLR,LR1分析，都是从集合I读入一个符号经过闭包运算得到集合J。goto函数内部调用了closure()。
int GOTO(int statusNum, string symbol)
{ //由状态集statusNum读入一个符号symbol(vn或vt)，返回转移后的项集编号
    int size = statusSet[statusNum].size();
    for (int i = 0; i < size; i++)
    {
        vector<string> right = statusSet[statusNum][i].rightPart;
        int dotPos = statusSet[statusNum][i].dotPos;
        // symbol是vn或者vt都可
        if (dotPos < right.size() && symbol == right[dotPos])
        {
            term tmpTerm = statusSet[statusNum][i];
            tmpTerm.dotPos = tmpTerm.dotPos + 1; //活前缀后移一位
            dotPos = tmpTerm.dotPos;
            //如果后移一位之后发现成为了规约项目,则加入新项目集
            if (tmpTerm.dotPos == tmpTerm.rightPart.size())
            {
                tmpTerm.termType = 4;
                statusSet[globalStatusNum].push_back(tmpTerm);
            }
            else if (VT2int.count(tmpTerm.rightPart[dotPos]) != 0)
            { //活前缀不在最后，且紧随着一个终结符(移进项目)，加入新项目
                tmpTerm.termType = 2;
                statusSet[globalStatusNum].push_back(tmpTerm);
            }
            else if (VN2int.count(tmpTerm.rightPart[dotPos]) != 0)
            { //活前缀不在最后，且紧随着一个非终结符(待约项目)，将上一个状态集中的该非终结符产生式加入
                //先加入 S->B·B
                tmpTerm.termType = 3;
                statusSet[globalStatusNum].push_back(tmpTerm);
                //再进行闭包计算
                closure(globalStatusNum);
            }
        }
    }
    globalStatusNum++;
    int flag = mergeSet();
    if (flag != -1) //可合并
    {
        statusSet[globalStatusNum - 1].clear(); //清空，以防万一
        globalStatusNum--;
        return flag;
    }
    else
        return globalStatusNum - 1;
}

void printStatus()
{
    //输出状态集
    for (int i = 0; i < globalStatusNum; i++)
    {
        if (statusSet[i].size() == 0)
            continue;
        cout << "┌───────────────────────┐" << endl;
        cout << "│      I" << i << "\t\t│" << endl;
        cout << "├───────────────────────┤" << endl;
        for (auto it = statusSet[i].begin(); it != statusSet[i].end(); it++)
        {
            cout << " \t";
            cout << it->leftPart << "->";
            for (int j = 0; j < it->rightPart.size(); j++)
            {
                if (j == it->dotPos)
                    cout << "·";
                cout << it->rightPart[j];
            }
            if (it->rightPart.size() == it->dotPos)
                cout << "·";
            for (int j = 0; j < it->subsequence.size(); j++)
            {
                if (j == 0)
                    cout << "," << it->subsequence[j];
                else
                    cout << "_" << it->subsequence[j];
            }
            cout << "     \t" << endl;
        }

        cout << "└───────────────────────┘" << endl;
    }
}

void printTable()
{
    //输出分析表，表头
    cout << " \t";
    for (auto it = VT2int.begin(); it != VT2int.end(); it++)
        cout << it->first << "  \t";
    for (auto it = VN2int.begin(); it != VN2int.end(); it++)
    { // goto表跳过增广文法的左部
        if (it->first == getVn(grammar[0].substr(0, grammar[0].length())))
            continue;
        cout << it->first << "  \t";
    }
    cout << endl;
    //输出表内容
    for (int i = 0; i < globalStatusNum; i++)
    {
        if (statusSet[i].size() == 0)
            continue;
        cout << i << "\t";
        for (auto it = VT2int.begin(); it != VT2int.end(); it++)
        { // action，移进(大20000)、规约(大30000)、接受项为10000
            if (actionTable[i][it->second] >= 20000 && actionTable[i][it->second] < 30000)
                cout << "s" << actionTable[i][it->second] - 20000 << "\t";
            else if (actionTable[i][it->second] >= 30000 && actionTable[i][it->second] < 40000)
                cout << "r" << actionTable[i][it->second] - 30000 << "\t";
            else if (actionTable[i][it->second] == 10000)
                cout << "acc\t";
            else
                cout << actionTable[i][it->second] << "  \t";
        }
        for (auto it = VN2int.begin(); it != VN2int.end(); it++)
        { // goto表跳过增广文法的左部，goto项数字就是项集编号
            if (it->first == getVn(grammar[0].substr(0, grammar[0].length())))
                continue;
            cout << gotoTable[i][it->second] << "  \t";
        }
        cout << endl;
    }
}

//相比LR0和SLR，LR1分析表只对规约项进行改动(项种类：移进项(不变)、接受项(?)、GOTO项(不变))
//删了十几行，加了1行就ok了
void constructStatusSet(int choice = 0)
{ //同步构造项集和分析表
    initI0();
    closure(0);
    queue<string> symbolToRead;
    map<string, int> symbolMap;
    int curStatus = 0; //队列中当前项的状态

    for (int i = 0; i < statusSet[0].size(); i++)
    {
        string symbolStr = statusSet[0][i].rightPart[statusSet[0][i].dotPos];
        if (symbolMap[symbolStr] == 0)
        {
            symbolToRead.push(symbolStr);
            symbolMap[symbolStr]++;
        }
    }
    symbolToRead.push("sep"); //加入分隔项

    while (!symbolToRead.empty())
    {
        if (symbolToRead.front() == "sep")
        { //一个项集的移进项和goto项都处理完毕之后，开始处理规约项
            for (int ii = 0; ii < statusSet[curStatus].size(); ii++)
            {
                /****action表规约项构造****/
                if (statusSet[curStatus][ii].dotPos == statusSet[curStatus][ii].rightPart.size())
                { //判断该规约项是用哪个产生式规约的
                    term tmpTerm = statusSet[curStatus][ii];
                    string reduceTerm = tmpTerm.leftPart + "->";
                    for (int ii = 0; ii < tmpTerm.rightPart.size(); ii++)
                        reduceTerm = reduceTerm + tmpTerm.rightPart[ii];
                    int genNum = -1;
                    for (int ii = 0; ii < grammar.size(); ii++)
                        if (reduceTerm == grammar[ii])
                            genNum = ii;
                    //接受状态
                    if (genNum == 0)
                        actionTable[curStatus][VT2int["#"]] = 10000;
                    else
                    {
                        // LR(1)分析中，只有规约项的后继符才进行规约
                        for (int _i = 0; _i < tmpTerm.subsequence.size(); _i++)
                        {
                            if (actionTable[curStatus][VT2int[tmpTerm.subsequence[_i]]] != 0)
                                cout << "(状态" << curStatus << "存在规约-规约冲突)" << endl;
                            actionTable[curStatus][VT2int[tmpTerm.subsequence[_i]]] = 30000 + genNum; //同样为避免编号冲突，规约项全体加30000
                        }
                    }
                }
                continue;
            }
            /****action表规约项填充完毕*****/
            curStatus++;
            symbolToRead.pop();
            continue;
        }
        int nextStatus = GOTO(curStatus, symbolToRead.front());
        cout << "I" << curStatus << "--" << symbolToRead.front() << "-->"
             << "I" << nextStatus;
        // action表移入项填充
        if (VT2int.count(symbolToRead.front()) != 0)
        {
            if (actionTable[curStatus][VT2int[symbolToRead.front()]] != 0)
                cout << "(状态" << curStatus << "移进" << symbolToRead.front() << "存在冲突)";
            actionTable[curStatus][VT2int[symbolToRead.front()]] = 20000 + nextStatus;
        }
        else // goto表填充
            gotoTable[curStatus][VN2int[symbolToRead.front()]] = nextStatus;
        cout << endl;
        //新状态集的活前缀后面的符号(包括vn和vt)入队列,若产生的状态集是已有的状态集的就不用了
        if (nextStatus == globalStatusNum - 1)
        {
            symbolMap.clear();
            for (int ii = 0; ii < statusSet[nextStatus].size(); ii++)
            {
                //规约项就跳过
                if (statusSet[nextStatus][ii].dotPos == statusSet[nextStatus][ii].rightPart.size())
                    continue;
                string symbolStr = statusSet[nextStatus][ii].rightPart[statusSet[nextStatus][ii].dotPos];
                if (symbolMap[symbolStr] == 0)
                {
                    symbolToRead.push(symbolStr);
                    symbolMap[symbolStr]++;
                }
            }
            symbolToRead.push("sep"); //引入分隔项
        }
        symbolToRead.pop();
    }
    printStatus(); //输出状态项集
    printTable();  //输出分析表
}

void LALR()
{
    cout << "———————————构造LALR分析表———————————————————" << endl;
    int merged[maxN];               //记录已经被合并的状态集
    fill(merged, merged + maxN, 0); //全0初始化
    map<int, int> mergedTo;         //记录那些被合并的状态集都被并入哪些状态集了
    int flag = -1;                  //首先假定状态集I的同心项目集和状态集J不一样
    for (int i = 0; i < globalStatusNum; i++)
    {
        if (merged[i] == 1)
            continue;
        for (int j = i + 1; j < globalStatusNum; j++)
        {
            if (merged[j] == 1)
                continue;
            if (statusSet[i].size() != statusSet[j].size())
                continue;
            flag = j; //状态集IJ此时的大小一样，假设可以相同，进一步一项项判断
            for (int k = 0; k < statusSet[j].size(); k++)
            {
                if (!(statusSet[i][k].leftPart == statusSet[j][k].leftPart && statusSet[i][k].rightPart == statusSet[j][k].rightPart && statusSet[i][k].dotPos == statusSet[j][k].dotPos))
                    flag = -1;
            }
            if (flag != -1)
            { //状态集I和状态集J是同心状态集，合并，也就是合并后继符
                for (int m = 0; m < statusSet[i].size(); m++)
                { //状态集I的第m项的第n个后继符
                    map<string, int> subsequenceMap;
                    for (int n = 0; n < statusSet[i][m].subsequence.size(); n++)
                        subsequenceMap[statusSet[i][m].subsequence[n]]++;
                    for (int n = 0; n < statusSet[j][m].subsequence.size(); n++)
                    {
                        if (subsequenceMap.count(statusSet[j][m].subsequence[n]) == 0)
                            statusSet[i][m].subsequence.push_back(statusSet[j][m].subsequence[n]);
                    }
                    subsequenceMap.clear();
                }
                cout << "同心状态集:i=" << i << " j=" << j << endl;
                mergedTo[j] = i;
                //将状态集J的分析表也并入状态集I，GOTO不会有冲突，也不会有规约-移进冲突，可能存在规约-规约冲突(如果存在则不是LALR文法)
                for (auto it = VT2int.begin(); it != VT2int.end(); it++)
                { //只要把状态集J可能的规约项填入状态集I的action表中相应位置就可以了
                    if (actionTable[j][it->second] >= 30000 && actionTable[j][it->second] < 40000)
                    {
                        if (actionTable[i][it->second] == 0)
                            actionTable[i][it->second] = actionTable[j][it->second];
                        else if (actionTable[i][it->second] >= 30000 && actionTable[i][it->second] < 40000 && actionTable[i][it->second] != actionTable[j][it->second])
                        { //规约-规约冲突得是两个不同的规约，同一规约不算冲突
                            cout << "产生规约-规约冲突，不是LALR文法" << endl;
                        }
                    }
                }
                //因为状态集J要被合并了，所以之后分析表中如果有项目是向状态集J移进的，改为向状态集I移进；GOTO也是
                // done in other place:指向转移的功能在双重循环外部完成，不然时间复杂度高
                //已经将状态集J并入状态集I,该删除状态集J了
                statusSet[j].clear();
                merged[j] = 1;
            }
        }
    }

    for (int j = 0; j < globalStatusNum; j++)
    {
        if (merged[j] == 1)
            continue;
        for (auto it = VT2int.begin(); it != VT2int.end(); it++)
        {
            if (actionTable[j][it->second] >= 20000 && actionTable[j][it->second] < 30000 && merged[actionTable[j][it->second] - 20000] == 1)
            { //是移进项，且该移进项指向的是被合并的状态集
                actionTable[j][it->second] = mergedTo[actionTable[j][it->second] - 20000] + 20000;
            }
        }
        for (auto it = VN2int.begin(); it != VN2int.end(); it++)
        {
            if (merged[gotoTable[j][it->second]] == 1)
            {
                gotoTable[j][it->second] = mergedTo[gotoTable[j][it->second]];
            }
        }
    }

    printStatus(); //输出状态项集
    printTable();  //输出分析表
}

int main()
{
    initGrammar();        //初始化文法
    readVnAndVt();        //读取文法中所有的VN和VT
    converge();           //构造first和follow集
    constructStatusSet(); //构造LR(1)分析表
    LALR();               //合并LR(1)的同心项目集，构造LALR分析表
    return 0;
}
