#include <iostream>
#include <cstring>
#include <fstream>
#define MAX 1000
using namespace std;

int a[MAX];
int presentCount = 0;
int tokenCount = 0;
string output[MAX]; // 输出内容存入数组
int outCount = 0;   // 输出数组的个数

bool flag = true;
/*
由于文法 <因子> --> id|{<表达式>} 的存在 此处<表达式>规约完成后不需要输出Exp
因此flag用于标志是否为此文法右侧的<表达式>
如果flag为true 则规约完成后的<表达式>不是此处的<表达式> 因此输出Exp
如果flag为false 则规约完成后的<表达式>是此处的<表达式> 不输出Exp 同时置falg为true
*/

bool Program();                // 程序
bool StatementList();          // 语句列表
bool StatementListPrime();     // 语句列表’
bool SingleStatement();        // 单语句
bool DeclarationStatement();   // 说明语句
bool VariableList();           // 变量列表
bool VariableListPrime();      // 变量列表’
bool Type();                   // 类型
bool AssignmentStatement();    // 赋值语句
bool IfStatement();            // if语句
bool IfStatementPrime();       // if语句’
bool WhileStatement();         // while语句
bool Block();                  // 语句块
bool Expression();             // 表达式
bool ExpressionPrime();        // 表达式’
bool Term();                   // 项
bool TermPrime();              // 项’
bool Factor();                 // 因子
bool BooleanExpression();      // 布尔表达式
bool BooleanExpressionPrime(); // 布尔表达式’
bool BooleanTerm();            // 布尔项
bool BooleanTermPrime();       // 布尔项’
bool BooleanFactor();          // 布尔因子
bool RelationExpression();     // 关系式
bool RelationOperator();       // 关系符

bool M_to_EmptyString(); // M
bool N_to_EmptyString(); // N

bool Program()
{
    if (a[presentCount] == 1)
    {
        presentCount++;
        if (a[presentCount] == 26)
        {
            presentCount++;
            if (a[presentCount] == 27)
            {
                presentCount++;
                if (a[presentCount] == 28)
                {
                    presentCount++;
                    if (StatementList())
                    {
                        if (a[presentCount] == 29)
                        {
                            output[outCount] = "Program";
                            outCount++;
                            presentCount++;
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool StatementList()
{
    if (SingleStatement())
    {
        if (StatementListPrime())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool StatementListPrime()
{
    if (StatementList())
    {
        return true;
    }
    else
    {
        return true;
    }
}

bool SingleStatement()
{
    if (DeclarationStatement())
    {
        return true;
    }
    else if (AssignmentStatement())
    {
        return true;
    }
    else if (IfStatement())
    {
        return true;
    }
    else if (WhileStatement())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DeclarationStatement()
{
    if (Type())
    {
        if (VariableList())
        {
            if (a[presentCount] == 24)
            {
                presentCount++;
                output[outCount] = "DecSta";
                outCount++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool VariableList()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        presentCount++;
        if (VariableListPrime())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool VariableListPrime()
{
    if (a[presentCount] == 25)
    {
        presentCount++;
        if (VariableList())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

bool Type()
{
    if (a[presentCount] == 2)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 3)
    {
        presentCount++;
        return true;
    }
    else
    {
        return false;
    }
}

bool AssignmentStatement()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        presentCount++;
        if (a[presentCount] == 13)
        {
            presentCount++;
            if (Expression())
            {
                if (a[presentCount] == 24)
                {
                    presentCount++;
                    output[outCount] = "AssSta";
                    outCount++;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*
<if语句> --> if(<布尔表达式>)<M><语句块><if语句'>
*/
bool IfStatement()
{
    if (a[presentCount] == 4)
    {
        presentCount++;
        if (a[presentCount] == 26)
        {
            presentCount++;
            if (BooleanExpression())
            {
                if (a[presentCount] == 27)
                {
                    presentCount++;
                    if (M_to_EmptyString())
                    {
                        if (Block())
                        {
                            if (IfStatementPrime())
                            {
                                output[outCount] = "IfSta";
                                outCount++;
                                return true;
                            }
                            else
                                return false;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

/*
<if语句'> --> <N>else<M><语句块>|ES
*/
bool IfStatementPrime()
{
    if (N_to_EmptyString())
    {
        if (a[presentCount] == 5)
        {
            presentCount++;
            if (M_to_EmptyString())
            {
                if (Block())
                {
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return true;
    }
    else
        return true;
}

/*
<while语句> --> while(<M1><布尔表达式>)<M2><语句块>
*/
bool WhileStatement()
{
    if (a[presentCount] == 6)
    {
        presentCount++;
        if (a[presentCount] == 26)
        {
            presentCount++;
            if (M_to_EmptyString())
            {
                if (BooleanExpression())
                {
                    if (a[presentCount] == 27)
                    {
                        presentCount++;
                        if (M_to_EmptyString())
                        {
                            if (Block())
                            {
                                output[outCount] = "WhileSta";
                                outCount++;
                                return true;
                            }
                            else
                                return false;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

bool Block()
{
    if (SingleStatement())
    {
        return true;
    }
    else if (a[presentCount] == 28)
    {
        presentCount++;
        if (StatementList())
        {
            if (a[presentCount] == 29)
            {
                presentCount++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Expression()
{
    if (Term())
    {
        if (ExpressionPrime())
        {
            if (flag)
            {
                output[outCount] = "Exp";
                outCount++;
                return true;
            }
            else
            {
                flag = true;
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool ExpressionPrime()
{
    if (a[presentCount] == 14 || a[presentCount] == 15)
    {
        presentCount++;
        if (Term())
        {
            if (ExpressionPrime())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

bool Term()
{
    if (Factor())
    {
        if (TermPrime())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool TermPrime()
{
    if (a[presentCount] == 16 || a[presentCount] == 17)
    {
        presentCount++;
        if (Factor())
        {
            if (TermPrime())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

bool Factor()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 26)
    {
        presentCount++;
        flag = false;
        if (Expression()) // 从这里进入的<表达式>文法，不输出Exp 所以在进入之前置flag为false
        {
            if (a[presentCount] == 27)
            {
                presentCount++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool BooleanExpression()
{
    if (BooleanTerm())
    {
        if (BooleanExpressionPrime())
        {
            output[outCount] = "BoolExp";
            outCount++;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*
<布尔式'> --> ||<M><布尔项><布尔式'>|ES
*/
bool BooleanExpressionPrime()
{
    if (a[presentCount] == 7)
    {
        presentCount++;
        if (M_to_EmptyString())
        {
            if (BooleanTerm())
            {
                if (BooleanExpressionPrime())
                {
                    return true;
                }
                else
                    return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return true;
}

bool BooleanTerm()
{
    if (BooleanFactor())
    {
        if (BooleanTermPrime())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*
<布尔项'> --> &&<M><布尔因子><布尔项'>|ES
*/
bool BooleanTermPrime()
{
    if (a[presentCount] == 8)
    {
        presentCount++;
        if (M_to_EmptyString())
        {
            if (BooleanFactor())
            {
                if (BooleanTermPrime())
                {
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return true;
}

bool BooleanFactor()
{
    if (a[presentCount] == 9)
    {
        presentCount++;
        if (RelationExpression())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (RelationExpression())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool RelationExpression()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        presentCount++;
        if (RelationOperator())
        {
            if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
            {
                presentCount++;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool RelationOperator()
{
    if (a[presentCount] == 18)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 19)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 20)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 21)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 22)
    {
        presentCount++;
        return true;
    }
    else if (a[presentCount] == 23)
    {
        presentCount++;
        return true;
    }
    else
    {
        return false;
    }
}

bool M_to_EmptyString()
{
    return true;
}

bool N_to_EmptyString()
{
    return true;
}

int main()
{
    int first;
    int second;

    ifstream ifile("token.txt");

    while (ifile >> first >> second)
    {
        a[tokenCount] = first;

        tokenCount++;
    }

    ifile.close();

    Program();

    ofstream ofile("syntax.txt");

    for (int i = 0; i < outCount; i++)
    {
        ofile << output[i] << endl;
    }

    ofile.close();
}