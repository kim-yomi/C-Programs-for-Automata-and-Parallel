#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include <cstring>

using namespace std;

string word;

const unordered_set<string> keywords = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
    "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit",
    "export", "extern", "false", "final", "float", "for", "friend", "goto", "if",
    "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not",
    "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
    "public", "register", "reinterpret_cast", "requires", "return", "short",
    "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
    "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef",
    "typeid", "typename", "union", "unsigned", "using", "virtual", "void",
    "volatile", "wchar_t", "while", "xor", "xor_eq"
};

const unordered_set<string> operators = {
    "=", "+", "-", "*", "/", "%", "+=", "-=", "*=", "/=", "%=", ">>=", "<<=",
    "&=", "^=", "|=", "++", "--", "==", "!=", ">", "<", ">=", "<=", "!", "&&",
    "||", "?", "&", "|", "^", "~", "<<", ">>", "?:", "."
};

char* turnStringToArr(const string& str);
void isNumber(int &i, const char* array, int arrayLength);
void isKeyword(int& i, const char* array, int arrayLength);
void isString(int& i, const char* array, int arrayLength);
void isTwoCharOperator(int& i, const char* array, int arrayLength, const string& op);
void isOperator(int& i, const string& op);
void isOther(int& i, char* array);

int main()
{
    string userCode;
    string fullCode;
    cout << "================================" << endl;
    cout << "CS0023 TN26 MIDTERM PROJECT KIM" << endl;
    cout << "Lexical Analyzer of C++" << endl;
    cout << "================================" << endl;
    cout << "        Instructions" << endl;
    cout << "================================" << endl;
    cout << "1. Use C++ syntax in writing a\n   program. " << endl;
    cout << "2. After typing 'return 0; },\n   press enter again to input your \n   program." << endl;
    cout << "================================" << endl;
    cout << "        *SAMPLE CODE*" << endl;
    cout << "int main(){\n int sum = 10 + 5; \n print(sum);\n return 0;\n}"<< endl;

    cout << "================================" << endl;
    cout << "Start Input Code:" << endl;
    while (true)
    {
        getline(cin, userCode);
        fullCode += userCode + "\n";
        if (fullCode.find("return 0;") != string::npos &&
            fullCode.find('}') != string::npos &&
            fullCode.find("return 0;") < userCode.find('}'))
        {
            break;
        }
    }
    cout << userCode << endl;
    cout << "================================" << endl;
    cout << "End Input Code:" << endl;
    cout << "================================" << endl;
    cout << "TOKENS" << endl;

    const char* result = turnStringToArr(fullCode);
    const int resultLength = strlen(result);

    for (int i = 0; i < resultLength;)
    {
        if (isdigit(result[i]))
        {
            isNumber(i, result, resultLength);
        }

        else if (isalnum(result[i]) || result[i] == '_')
        {
            isKeyword(i, result, resultLength);
        }

        else if (ispunct(result[i]))
        {
            string op;
            op += result[i];

            if (result[i] == '"' || result[i] == '\'')
            {
                isString(i, result, resultLength);
            }
            else if (i + 1 < resultLength && operators.contains(op + result[i + 1]))
            {
                isTwoCharOperator(i, result, resultLength, op);
            }
            else
            {
                isOperator(i, op);
            }
        }
        else
        {
            if (!isspace(result[i]))
            {
                cout << result[i] << " is categorized as other." << endl;
            }
            i++;
        }
    }
    cout << "================================" << endl;

    delete[] result;
    return 0;
}

char* turnStringToArr(const string& str)
{
    const int len = str.length();
    char* array = new char[len + 1];
    for (int i = 0; i < len; i++)
    {
        array[i] = str[i];
    }
    array[len] = '\0';
    return array;
}

void isNumber(int& i, const char* array, int arrayLength)
{
    word.clear();
    while (i < arrayLength && isdigit(array[i]))
    {
        word += array[i++];
    }

    if (i < arrayLength && array[i] == '.' && (i + 1 < arrayLength) && isdigit(array[i + 1]))
    {
        word += array[i++]; // Add dot
        while (i < arrayLength && isdigit(array[i]))
        {
            word += array[i++];
        }
        cout << word << " is a decimal." << endl;
    }
    else
    {
        cout << word << " is an integer." << endl;
    }
}

void isKeyword(int& i, const char* array, const int arrayLength)
{
    word.clear();
    while (i < arrayLength && (isalnum(array[i]) || array[i] == '_'))
    {
        word += array[i++];
    }

    if (keywords.contains(word))
    {
        cout << word << " is a keyword." << endl;
    }
    else
    {
        cout << word << " is an identifier." << endl;
    }
}

void isString(int& i, const char* array, const int arrayLength)
{
    char quote = array[i];
    word.clear();
    i++;

    while (i < arrayLength && array[i] != quote)
    {
        word += array[i++];
    }

    cout << word << " is a string" << endl;

    if (i < arrayLength && array[i] == quote) i++;
}

void isTwoCharOperator(int& i, const char* array, const int arrayLength, const string& op)
{
    string twoCharOp = op + array[i + 1];
    if (operators.contains(twoCharOp))
    {
        cout << twoCharOp << " is an operator." << endl;
        i += 2;
    }
}

void isOperator(int& i, const string& op)
{
    if (operators.contains(op))
    {
        cout << op << " is an operator." << endl;
    }
    else
    {
        cout << op << " is a punctuation character." << endl;
    }
    i++;
}

void isOther(int& i, const char* array)
{
    if (!isspace(array[i]))
    {
        cout << array[i] << " is categorized as other." << endl;
    }
    i++;
}
