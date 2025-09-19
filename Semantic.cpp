#include <iostream>
#include <cstring>
#include <fstream>
#define MAX 1000
using namespace std;

int a[MAX];
int b[MAX];
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

bool flag_else = false;
/*
由于if语句产生式的语法规则和if else语句产生式的语法规则有不同
因此设置此标志位来判断有无else
如果flag为true 则有else 处理较为复杂
如果flag为false 则没有else  处理较为简单
*/

int true_false_ExpCount = 0;   // 布尔表达式数量                拥有属性true,false
int M_quad_ExpCount = 0;       // 控制语句数量 即M->ES的数量     拥有属性quad
int N_next_ExpCount = 0;       // 控制语句数量 即N->ES的数量     拥有属性next
int place_ExpCount = 0;        // 赋值语句数量                  拥有属性place
int next_ExpCount = 0;         // 语句数量                       拥有属性next
int quaternaryFormula[MAX][5]; // 用于存储四元式
int nextquad = 1;
int newtemp_number = 0; // 临时变量标号

int find_E_tf_index[MAX] = {0};
int find_E_tf_index_count = 0;
/*用于查找下标 由于定义的文法是ll1文法，因此有些二元运算的语义规则无法直接找到前一个元素的下标
因此定义此标志栈，用于寻找元素
每当读取到产生式 下标入栈
用下标回填时 下标出栈
具有true false属性*/

int find_E_place_index[MAX] = {0};
int find_E_place_index_count = 0;
/*用于查找下标 由于定义的文法是ll1文法，因此有些二元运算的语义规则无法直接找到前一个元素的下标
因此定义此标志栈，用于寻找元素
每当读取到产生式 下标入栈
用下标回填时 下标出栈
具有place属性*/

int find_M_index[MAX] = {0};
int find_M_index_count = 0;
/*同样用于查找下标 只不过用来查找M->ES(空串)的下标
每当读取到产生式 下标入栈
用下标回填时 下标出栈
具有quad属性*/

int find_N_index[MAX] = {0};
int find_N_index_count = 0;
/*同样用于查找下标 只不过用来查找N->ES(空串)的下标
每当读取到产生式 下标入栈
用下标回填时 下标出栈
具有next属性*/

int find_S_index[MAX] = {0};
int find_S_index_count = 0;
/*用于查找下标 由于定义的文法是ll1文法，因此有些二元运算的语义规则无法直接找到前一个元素的下标
因此定义此标志栈，用于寻找元素
每当读取到产生式 下标入栈
用下标回填时 下标出栈
具有next属性*/

// 入栈
void push_E_tf_stack(int a)
{
    find_E_tf_index[find_E_tf_index_count] = a;
    find_E_tf_index_count++;
}

// 出栈 并返回出栈的值
int pop_E_tf_stack()
{
    int tmp = find_E_tf_index[find_E_tf_index_count - 1];
    find_E_tf_index_count--;
    return tmp;
}

// 入栈
void push_E_place_stack(int a)
{
    find_E_place_index[find_E_place_index_count] = a;
    find_E_place_index_count++;
}

// 出栈 并返回出栈的值
int pop_E_place_stack()
{
    int tmp = find_E_place_index[find_E_place_index_count - 1];
    find_E_place_index_count--;
    return tmp;
}

// 入栈
void push_M_stack(int a)
{
    find_M_index[find_M_index_count] = a;
    find_M_index_count++;
}

// 出栈 并返回出栈的值
int pop_M_stack()
{
    int tmp = find_M_index[find_M_index_count - 1];
    find_M_index_count--;
    return tmp;
}

// 入栈
void push_N_stack(int a)
{
    find_N_index[find_N_index_count] = a;
    find_N_index_count++;
}

// 出栈 并返回出栈的值
int pop_N_stack()
{
    int tmp = find_N_index[find_N_index_count - 1];
    find_N_index_count--;
    return tmp;
}

void push_S_stack(int a)
{
    find_S_index[find_S_index_count] = a;
    find_S_index_count++;
}

// 出栈 并返回出栈的值
int pop_S_stack()
{
    int tmp = find_S_index[find_S_index_count - 1];
    find_S_index_count--;
    return tmp;
}

struct Expr
{
    int true_number[MAX];  // 存储真跳转的地址
    int false_number[MAX]; // 存储假跳转的地址
    int true_count = 0;    // 真跳转地址的数量
    int false_count = 0;   // 假跳转地址的数量

    int quad;

    int place;

} E_true_false[MAX], S_next[MAX], M_quad[MAX], N_next[MAX], E_place[MAX];
/*
用同一个结构体 方便合并地址代码
其中next用true数组代替
next的数量用true_count代替
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

bool M_to_EmptyString();
bool N_to_EmptyString();

// backpatch函数：将quad值填入E.true_number中的四元式跳转地址
void backfill(int list[], int count, int quad)
{
    for (int i = 0; i < count; i++)
    {
        int index = list[i];
        quaternaryFormula[index][4] = quad; // 将quad值填入四元式的result位置
    }
}

// backpatch(E.true, M.quad);
void backpatch_true(Expr &E, int M_quad)
{
    // 将 M_quad 值填入 E.true_number 中记录的所有跳转地址
    backfill(E.true_number, E.true_count, M_quad);
}

// backpatch(E.false, M.quad);
void backpatch_false(Expr &E, int M_quad)
{
    // 将 M_quad 值填入 E.true_number 中记录的所有跳转地址
    backfill(E.false_number, E.false_count, M_quad);
}

void emit(int a, int b, int c, int d)
{
    quaternaryFormula[nextquad][1] = a;
    quaternaryFormula[nextquad][2] = b;
    quaternaryFormula[nextquad][3] = c;
    quaternaryFormula[nextquad][4] = d;
    nextquad++;
}

// 合并两个布尔表达式的真跳转或假跳转集合
void merge(int *dest, int &dest_count, int *src, int src_count)
{
    for (int i = 0; i < src_count; i++)
    {
        dest[dest_count] = src[i];
        dest_count++;
    }
}

// 对true属性合并
Expr mergeBooleanExpr_true(Expr &exp1, Expr &exp2)
{
    Expr result;
    result.true_count = 0;
    result.false_count = 0;

    merge(result.true_number, result.true_count, exp1.true_number, exp1.true_count);

    merge(result.true_number, result.true_count, exp2.true_number, exp2.true_count);

    return result;
}

// 对false属性合并
Expr mergeBooleanExpr_false(Expr &exp1, Expr &exp2)
{
    Expr result;
    result.true_count = 0;
    result.false_count = 0;
    result.place = 0;
    result.quad = 0;

    merge(result.false_number, result.false_count, exp1.false_number, exp1.false_count);

    merge(result.false_number, result.false_count, exp2.false_number, exp2.false_count);

    return result;
}

// newtemp函数
int newtemp()
{
    newtemp_number++;
    return 200 + newtemp_number;
}

/*
<程序> --> main(){<语句列表>}
*/
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

/*
<语句列表> --> <单语句><语句列表’>
*/
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

/*
<语句列表’> --> <M><语句列表>|ES
*/
bool StatementListPrime()
{
    // 如果开始读取，读取到M时，则不可避免地执行M_to_EmptyString函数，导致每次进入都会改变quad属性
    // 准确说不是改变quad的值，而是在M堆栈中新加入索引，只需要将没用的出栈即可
    // 因此if语句外边 即选择第二个候选式时 执行出栈操作
    if (M_to_EmptyString())
    {
        if (StatementList()) // 如果不是第一个候选式 则跳出if语句 置标志位为false
        {
            int Stemp2 = pop_S_stack();
            int Stemp1 = pop_S_stack();
            int Mtemp = pop_M_stack();

            backpatch_true(S_next[Stemp1], M_quad[Mtemp].quad);

            S_next[next_ExpCount].true_count = S_next[Stemp2].true_count;
            for (int i = 0; i < S_next[Stemp2].true_count; i++)
            {
                S_next[next_ExpCount].true_number[i] = S_next[Stemp2].true_number[i];
            }

            push_S_stack(next_ExpCount);

            next_ExpCount++;

            return true;
        }
    }
    pop_M_stack();
    return true;
}

/*
<单语句> --> <说明语句>|<赋值语句>|<if语句>|<while语句>
*/
bool SingleStatement()
{
    if (DeclarationStatement())
    {
        return true;
    }
    else if (AssignmentStatement())
    {
        S_next[next_ExpCount].true_number[MAX] = {0};
        S_next[next_ExpCount].true_count = 0;
        push_S_stack(next_ExpCount);
        next_ExpCount++;
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

/*
<说明语句> --> <类型><变量列表>
*/
bool DeclarationStatement()
{
    if (Type())
    {
        if (VariableList())
        {
            if (a[presentCount] == 24)
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

/*
<变量列表> --> id<变量列表’>
*/
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

/*
<变量列表’> --> ,<变量列表’>|ES
*/
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

/*
<类型> --> int|float
*/
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

/*
<赋值语句> --> id=<表达式>;
*/
bool AssignmentStatement()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        int p = b[presentCount];
        presentCount++;
        if (a[presentCount] == 13)
        {
            presentCount++;
            if (Expression())
            {
                if (a[presentCount] == 24)
                {
                    presentCount++;
                    emit(13, E_place[place_ExpCount - 1].place, 0, p);
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
                                if (flag_else)
                                {
                                    flag_else = false;
                                    int Etemp = pop_E_tf_stack();
                                    int Stemp2 = pop_S_stack();
                                    int Stemp1 = pop_S_stack();
                                    int Mtemp2 = pop_M_stack();
                                    int Mtemp1 = pop_M_stack();
                                    int Ntemp = pop_N_stack();

                                    backpatch_true(E_true_false[Etemp], M_quad[Mtemp1].quad);

                                    backpatch_false(E_true_false[Etemp], M_quad[Mtemp2].quad);
                                    Expr tmp;
                                    tmp = mergeBooleanExpr_true(S_next[Stemp1], S_next[Stemp2]);

                                    S_next[next_ExpCount] = mergeBooleanExpr_true(tmp, N_next[Ntemp]);
                                    push_S_stack(next_ExpCount);
                                    next_ExpCount++;
                                    return true;
                                }
                                else
                                {
                                    int Etemp1 = pop_E_tf_stack();
                                    int Stemp = pop_S_stack();
                                    int Mtemp = pop_M_stack();

                                    backpatch_true(E_true_false[Etemp1], M_quad[Mtemp].quad);

                                    S_next[next_ExpCount] = mergeBooleanExpr_true(E_true_false[true_false_ExpCount], S_next[Stemp]);
                                    push_S_stack(next_ExpCount);
                                    next_ExpCount++;
                                    return true;
                                }
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
    if (a[presentCount] == 5)
    {
        flag_else = true;
        if (N_to_EmptyString())
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
            return false;
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
                                int Etemp = pop_E_tf_stack();
                                int Mtemp2 = pop_M_stack();
                                int Mtemp1 = pop_M_stack();
                                int Stemp = pop_S_stack();

                                backpatch_true(S_next[Stemp], M_quad[Mtemp1].quad);

                                backpatch_true(E_true_false[Etemp], M_quad[Mtemp2].quad);

                                S_next[next_ExpCount].true_count = E_true_false[Etemp].false_count;
                                for (int i = 0; i < E_true_false[Etemp].false_count; i++)
                                {
                                    S_next[next_ExpCount].true_number[i] = E_true_false[true_false_ExpCount].false_number[i];
                                }
                                push_S_stack(next_ExpCount);
                                emit(30, 0, 0, M_quad[Mtemp1].quad);
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
<语句块> --> <单语句>|{<语句列表>}
*/
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

/*
<表达式> --> <项><表达式’>
*/
bool Expression()
{
    if (Term())
    {
        if (ExpressionPrime())
        {
            if (flag)
            {
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

/*
<表达式‘> --> +/-<项><表达式>|ES
*/
bool ExpressionPrime()
{
    if (a[presentCount] == 14 || a[presentCount] == 15)
    {
        int symbol_number = a[presentCount];
        presentCount++;
        if (Term())
        {
            if (ExpressionPrime())
            {
                int Etemp2 = pop_E_place_stack();
                int Etemp1 = pop_E_place_stack();
                E_place[place_ExpCount].place = newtemp();
                push_E_place_stack(place_ExpCount);
                emit(symbol_number, E_place[Etemp1].place, E_place[Etemp2].place, E_place[place_ExpCount].place);
                place_ExpCount++;
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

/*
<项> --> <因子><项’>
*/
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

/*
<项'> --> *<因子><项'>|/<因子><项'>|ES
*/
bool TermPrime()
{
    if (a[presentCount] == 16 || a[presentCount] == 17)
    {
        int symbol_number = a[presentCount];
        presentCount++;
        if (Factor())
        {
            if (TermPrime())
            {
                int Etemp2 = pop_E_place_stack();
                int Etemp1 = pop_E_place_stack();
                E_place[place_ExpCount].place = newtemp();
                push_E_place_stack(place_ExpCount);
                emit(symbol_number, E_place[Etemp1].place, E_place[Etemp2].place, E_place[place_ExpCount].place);
                place_ExpCount++;
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

/*
<因子> --> id|(<表达式>)
*/
bool Factor()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        E_place[place_ExpCount].place = b[presentCount];
        push_E_place_stack(place_ExpCount);

        place_ExpCount++;

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

/*
<布尔表达式> --> <布尔项><布尔表达式’>
*/
bool BooleanExpression()
{
    if (BooleanTerm())
    {
        if (BooleanExpressionPrime())
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
                    int Etemp2 = pop_E_tf_stack();
                    int Etemp1 = pop_E_tf_stack();
                    int Mtemp = pop_M_stack();

                    backpatch_false(E_true_false[Etemp1], M_quad[Mtemp].quad);

                    E_true_false[true_false_ExpCount] = mergeBooleanExpr_true(E_true_false[Etemp1], E_true_false[Etemp2]);

                    E_true_false[true_false_ExpCount].false_count = E_true_false[Etemp2].false_count;
                    for (int i = 0; i < E_true_false[Etemp2].false_count; i++)
                    {
                        E_true_false[true_false_ExpCount].false_number[i] = E_true_false[Etemp2].false_number[i];
                    }

                    push_E_tf_stack(true_false_ExpCount);

                    true_false_ExpCount++;

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

/*
<布尔项> --> <布尔因子><布尔项’>
*/
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
                    int Etemp2 = pop_E_tf_stack();
                    int Etemp1 = pop_E_tf_stack();
                    int Mtemp = pop_M_stack();

                    backpatch_true(E_true_false[Etemp1], M_quad[Mtemp].quad);

                    E_true_false[true_false_ExpCount] = mergeBooleanExpr_false(E_true_false[Etemp1], E_true_false[Etemp2]);

                    E_true_false[true_false_ExpCount].true_count = E_true_false[Etemp2].true_count;
                    for (int i = 0; i < E_true_false[Etemp2].false_count; i++)
                    {
                        E_true_false[true_false_ExpCount].true_number[i] = E_true_false[Etemp2].true_number[i];
                    }

                    push_E_tf_stack(true_false_ExpCount);

                    true_false_ExpCount++;

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

/*
<布尔因子> --> !<关系式>|<关系式>
*/
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

/*
<关系式> --> id<关系符>id
*/
bool RelationExpression()
{
    if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
    {
        presentCount++;
        if (RelationOperator())
        {
            if (a[presentCount] == 10 || a[presentCount] == 11 || a[presentCount] == 12)
            {
                E_true_false[true_false_ExpCount].true_number[E_true_false[true_false_ExpCount].true_count] = nextquad;
                E_true_false[true_false_ExpCount].true_count++;
                E_true_false[true_false_ExpCount].false_number[E_true_false[true_false_ExpCount].false_count] = nextquad + 1;
                E_true_false[true_false_ExpCount].false_count++;
                push_E_tf_stack(true_false_ExpCount);

                true_false_ExpCount++;
                emit(a[presentCount - 1], b[presentCount - 2], b[presentCount], 0);
                emit(30, 0, 0, 0);

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

/*
<关系符> --> <|<=|>|>=|==|!=
*/
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

/*
M --> ES
*/
bool M_to_EmptyString()
{
    M_quad[M_quad_ExpCount].quad = nextquad;
    push_M_stack(M_quad_ExpCount);
    M_quad_ExpCount++;
    return true;
}

/*
N --> ES
*/
bool N_to_EmptyString()
{
    N_next[N_next_ExpCount].true_number[N_next_ExpCount] = nextquad;
    N_next[N_next_ExpCount].true_count++;
    push_N_stack(N_next_ExpCount);
    N_next_ExpCount++;
    emit(30, 0, 0, 0);

    return true;
}

int main()
{
    int first;
    int second;

    ifstream ifile("token.txt");

    // a装第一元信息，即在编码表中的标号
    // b装第二元信息
    while (ifile >> first >> second)
    {
        a[tokenCount] = first;
        b[tokenCount] = second;

        tokenCount++;
    }

    ifile.close();

    Program();

    ofstream ofile("quad.txt");

    for (int i = 1; i < nextquad; i++)
    {
        if (quaternaryFormula[i][4] == 0)
        {
            quaternaryFormula[i][4] = nextquad;
        }
    }
    quaternaryFormula[nextquad][1] = 88;
    quaternaryFormula[nextquad][2] = 0;
    quaternaryFormula[nextquad][3] = 0;
    quaternaryFormula[nextquad][4] = 0;
    for (int i = 1; i <= nextquad; i++)
    {
        ofile << i << " " << quaternaryFormula[i][1] << " " << quaternaryFormula[i][2] << " " << quaternaryFormula[i][3] << " " << quaternaryFormula[i][4] << endl;
    }

    ofile.close();
}