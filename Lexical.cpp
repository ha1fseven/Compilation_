#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

#define MAX 300
#define integer_code 11
#define real_code 12

typedef struct WordToken
{
    char name[30];        // 名字
    int index;            //  编号
    int number_in_symbol; // 在符号表中的序号
} WT;

typedef struct WordSymbol
{
    int number;     // 在符号表中的序号
    char name[30];  // 名字
    int index = 10; // 标识符编号固定为10
} WS;

WT keyword[6] = {"main", 1, 0, "int", 2, 0, "float", 3, 0, "if", 4, 0, "else", 5, 0, "while", 6, 0};
WT delimiter[6] = {";", 24, 0, ",", 25, 0, "(", 26, 0, ")", 27, 0, "{", 28, 0, "}", 29, 0};
WT operatorr[14] = {"||", 7, 0, "&&", 8, 0, "!", 9, 0, "=", 13, 0, "+", 14, 0, "-", 15, 0, "*", 16, 0, "/", 17, 0, "<", 18, 0, "<=", 19, 0, ">", 20, 0, ">=", 21, 0, "==", 22, 0, "!", 23, 0};
WT token_table[MAX];
WS symbol[MAX];

int token_table_length = 0;       // token表的长度
int symbol_length = 0;            // symbol表的长度
int symbol_identifier_length = 0; // symbol表中符号的数量
int symbol_digital_length = 0;    // symbol表中常数的数量
bool integer_real = 0;            // 用于判断常数是整数还是实数 0为整数，1为实数

int iskeyword(string str)
{
    if (str == "main")
        return 1;
    else if (str == "int")
        return 2;
    else if (str == "float")
        return 3;
    else if (str == "if")
        return 4;
    else if (str == "else")
        return 5;
    else if (str == "while")
        return 6;
    else
        return 0;
}
/*用来判断是否为关键字 若是则返回编码 若不是则返回0*/

int is_alpha(char *str)
{
    if (*str >= 'a' && *str <= 'z')
        return 1;
    if (*str >= 'A' && *str <= 'Z')
        return 1;
    return 0;
}
/*如果是字母 则返回1 否则返回0*/
int is_digit(char *str)
{
    if (*str >= '0' && *str <= '9')
        return 1;
    return 0;
}
/*如果是数字 则返回1 否则返回0*/

int is_delimiter(char *str)
{
    int i = 0;
    while (i < 6)
    {
        if (*str == delimiter[i].name[0]) // 由于分隔符都是单字符，所以只用比较第一个字符即可
        {
            return delimiter[i].index;
        }

        else
        {
            i++;
        }
    }
    return 0;
}
/*如果是界符 ，；（）{} 则返回对应的编码 否则返回0*/

int is_operatorr(char *str)
{
    int i = 0;
    while (i < 14)
    { // 只需判断第一个符号是否是运算符
        // 第二个符号是否是运算符，在recognize中判断即可
        if (*str == operatorr[i].name[0])
        {
            return operatorr[i].index;
        }
        i++;
    }
    return 0;
}
/*如果是运算符 ||，&&等 则返回1 否则返回0*/

void insert_token(WT token)
{
    int i = token_table_length;
    strcpy(token_table[i].name, token.name);
    token_table[i].index = token.index;
    token_table[i].number_in_symbol = token.number_in_symbol;
    token_table_length++;
}

int isexist_sym(char *str)
{
    int i = 0;
    while (i < symbol_length)
    {
        if (strcmp(str, symbol[i].name) == 0)
        {
            return i + 1;
        }
        i++;
    }
    return 0;
}
/*symbol是符号表 当在符号表中查到单词时 返回对应的位置 否则返回0*/

int insert_sym(char *str)
{
    int i = 0;
    if (isexist_sym(str))
    {
        return isexist_sym(str);
    }
    else
    {
        if (is_alpha(str))
        {
            i = symbol_identifier_length; // 从当前长度插入
            symbol[i].number = i + 1;
            strcpy(symbol[i].name, str); // 复制字符串到 name
            symbol[i].index = 10;

            symbol_length++;            // 增加符号表长度
            symbol_identifier_length++; // 标识符个数加一
        }
        if (is_digit(str))
        {
            i = symbol_digital_length + 100; // 从100开始标号
            symbol[i].number = i + 1;
            strcpy(symbol[i].name, str);

            if (integer_real)
            {
                symbol[i].index = 12;
            }
            else
            {
                symbol[i].index = 11;
            }

            symbol_length++;         // 增加符号表长度
            symbol_digital_length++; // 常数个数加一
        }
        return symbol[i].number; // 返回插入的序号
    }
}
/*在symbol表中插入 同时长度+1*/

void write_token_to_txt(char *name)
{
    int i = 1;
    ofstream file(name);
    if (!file)
    {
        return;
    }
    for (int i = 0; i < token_table_length; i++)
    {
        file << token_table[i].index << " " << token_table[i].number_in_symbol;
        file << endl;
    }
    file.close();
}

void write_symbol_to_txt(char *name)
{
    int i = 1;
    ofstream file(name);
    if (!file)
    {
        return;
    }
    for (int i = 0; i < symbol_identifier_length; i++)
    {
        file << symbol[i].number << " " << symbol[i].name;
        file << endl;
    }
    for (int i = 0; i < symbol_digital_length; i++)
    {
        file << symbol[i + 100].number << " " << symbol[i + 100].name;
        file << endl;
    }
    file.close();
}
/*将symbol表中的内容输出到txt文件中*/

WT recognize_identifier(char *ch)
{
    WT tmp;
    int code = 0;
    int value = 0;
    char str[MAX] = "";

    // 根据状态图将标识符读入str中
    while (is_alpha(ch) || is_digit(ch))
    {
        strncat(str, ch, 1);
        ch++;
    }

    code = iskeyword(str); // 判断是否为关键字
    if (code != 0)
    {
        strcpy(tmp.name, str);
        tmp.index = code;
        tmp.number_in_symbol = 0;
        return tmp;
        // 是关键字 则返回token表中对应的信息
    }
    else
    {
        value = insert_sym(str); // 不是关键字，则插入字符表中
        strcpy(tmp.name, str);
        tmp.index = 10;
        tmp.number_in_symbol = value;
        return tmp; // 返回标识符的对应信息
    }
}
/*判断是关键字还是标识符，返回对应的信息*/

WT recognize_delimer(char *ch)
{
    WT tmp;
    char str[MAX] = "";
    strncat(str, ch, 1);
    string word(str);

    tmp.index = is_delimiter(ch);
    tmp.number_in_symbol = 0;
    strcpy(tmp.name, str);

    return tmp;
}
/*判断是哪个分界符，返回对应的信息*/

WT recognize_operatorr(char *ch)
{
    WT tmp;
    int code = 0;
    char str[MAX] = "";

    while (is_operatorr(ch)) // 挨个读取运算符
    {
        strncat(str, ch, 1);
        ch++;
    }
    if (strcmp(str, "||") == 0)
    {
        code = 7;
    }
    else if (strcmp(str, "&&") == 0)
    {
        code = 8;
    }
    else if (strcmp(str, "<=") == 0)
    {
        code = 19;
    }
    else if (strcmp(str, ">=") == 0)
    {
        code = 21;
    }
    else if (strcmp(str, "==") == 0)
    {
        code = 22;
    }
    else if (strcmp(str, "!=") == 0)
    {
        code = 23;
    }
    else
    {
        code = is_operatorr(str); // 注意是str不是ch 因为取完整的符号时，ch已经不包括符号
    }
    /*上述写法的前提条件：单符号单词须出现在双符号单词前*/
    /*例如：>号编码为x，则>=号编码必须大于x*/

    strcpy(tmp.name, str);
    tmp.index = code;
    tmp.number_in_symbol = 0;
    return tmp;
}
/*判断是哪个运算符 其中用到大量if else结构 返回token表中信息*/

WT recognize_digital(char *ch)
{
    WT tmp;
    int state = 0;
    char str[MAX] = "";
    int value = 0;

    while (state != 7)
    {
        switch (state)
        {
        case 0:
            if (is_digit(ch))
                state = 1;
            else if (*ch == '.')
                state = 6;
            strncat(str, ch, 1);
            ch++; // 移动到下一个字符
            break;
        case 1:
            if (is_digit(ch))
                state = 1;
            else if (*ch == '.')
                state = 2;
            else if (*ch == 'e' || *ch == 'E')
                state = 4;
            else
            {
                // 整数识别成功
                integer_real = 0;             // 整数
                value = insert_sym(str);      // value保存编号
                strcpy(tmp.name, str);        // 保存name
                tmp.index = integer_code;     // 保存编号
                tmp.number_in_symbol = value; // value赋值归位
                return tmp;
            }
            strncat(str, ch, 1);
            ch++;
            break;
        case 2:
            if (is_digit(ch))
                state = 2;
            else if (*ch == 'e' || *ch == 'E')
                state = 3;
            else
            {
                // 实型常数识别成功
                integer_real = 1;             // 实数
                value = insert_sym(str);      // value保存编号
                strcpy(tmp.name, str);        // 保存name
                tmp.index = real_code;        // 保存编号
                tmp.number_in_symbol = value; // value赋值归位
                return tmp;
            }
            strncat(str, ch, 1);
            ch++;
            break;
        case 3:
            if (is_digit(ch))
                state = 5;
            else if (*ch == '+' || *ch == '-')
                state = 4;
            strncat(str, ch, 1);
            ch++;
            break;
        case 4:
            if (is_digit(ch))
                state = 5;
            strncat(str, ch, 1);
            ch++;
            break;
        case 5:
            if (is_digit(ch))
                state = 5;
            else
            {
                // 实型常数识别成功
                value = insert_sym(str);      // value保存编号
                strcpy(tmp.name, str);        // 保存name
                tmp.index = real_code;        // 保存编号
                tmp.number_in_symbol = value; // value赋值归位
                integer_real = 1;             // 实数
                return tmp;
            }
            strncat(str, ch, 1);
            ch++;
            break;
        case 6:
            if (is_digit(ch))
                state = 2;
            strncat(str, ch, 1);
            ch++;
            break;
        }
    }
}

void scanner(ifstream &file)
{
    int index = 0;
    string line;
    // char *line_char;
    /*line_char  -->  line_char+index*/
    /*line_char++  -->  index++*/
    char line_char[MAX];
    while (getline(file, line))
    {
        index = 0;
        *line_char = '\0';
        int i = 0; // i是line的索引
        int j = 0; // j是line_char的索引

        // 跳过开头的空格
        while (i < line.length() && line[i] == ' ')
        {
            i++;
        }

        // 从第一个非空格字符开始复制
        for (; i < line.length(); i++)
        {
            line_char[j] = line[i];
            j++;
        }
        line_char[j] = '\0'; // 在末尾手动添加 '\0' 终止符

        while (line_char[index] != '\0') // 如果字符数组没结束
        {
            if (is_alpha(line_char + index)) // 读取到的第一个是字母 说明是标识符或关键字
            {
                WT token = recognize_identifier(line_char + index); // 判断是标识符还是关键字//已经插入到sym表中了
                insert_token(token);                                // 插入到token表中
                while (is_alpha(line_char + index) || is_digit(line_char + index))
                {
                    index++; // 将这个单词跳过
                }
                continue;
            }
            if (is_delimiter(line_char + index)) // 如果扫描到的是分隔符
            {
                WT token = recognize_delimer(line_char + index);
                insert_token(token);
                // 由于分隔符都是单字符，因此不需要跳过 只需要读取一个字符
                index++;
                continue;
            }
            if (is_operatorr(line_char + index))
            {
                WT token = recognize_operatorr(line_char + index);
                insert_token(token);
                while (is_operatorr(line_char + index)) // 跳过双符号运算符的第二个符号
                {
                    index++;
                }
                continue;
            }
            if (is_digit(line_char + index))
            {
                WT token = recognize_digital(line_char + index);
                insert_token(token);
                while (is_digit(line_char + index) || line_char[index] == '.')
                {
                    index++;
                }
                continue;
            }
            else
            {
                index++;
            }
        }
    }
}

int main()
{
    ifstream file("sourceCode.txt");
    scanner(file);
    write_token_to_txt("token.txt");
    write_symbol_to_txt("symbol.txt");
    return 0;
}