#include "CodeGenerator.h"
#include <QStringBuilder>
static QString genJudge(const MinDFA& m){ QString s; s += "int Judgechar(char ch){"; if(m.alpha.hasLetter){ s += " if(isalpha((unsigned char)ch)) return 1;"; } if(m.alpha.hasDigit){ s += " if(isdigit((unsigned char)ch)) return 0;"; } s += " return -1; }"; return s; }
static QString genNext(){ return "char GetNext(){ extern std::string buffer; extern size_t pos; if(pos<buffer.size()) return buffer[pos++]; return '\\0'; }"; }
QString CodeGenerator::generate(const MinDFA& mdfa, const QMap<QString,int>& tokenCodes){
    QString code;
    code += "#include <cctype>\n";
    code += "#include <string>\n";
    code += "using namespace std;\n\n";

    code += "int Judgechar(char ch) {\n";
    if (mdfa.alpha.hasLetter) { code += "    if (isalpha((unsigned char)ch)) return 1;\n"; }
    if (mdfa.alpha.hasDigit) { code += "    if (isdigit((unsigned char)ch)) return 0;\n"; }
    code += "    return -1;\n";
    code += "}\n\n";

    code += "bool AcceptState(int s) {\n";
    code += "    switch (s) {\n";
    for(auto it=mdfa.states.begin(); it!=mdfa.states.end(); ++it){ if(it->accept){ code += "        case "+QString::number(it->id)+": return true;\n"; } }
    code += "        default: return false;\n";
    code += "    }\n";
    code += "}\n\n";

    code += "int Step(int state, char ch) {\n";
    code += "    switch (state) {\n";
    for(auto it=mdfa.states.begin(); it!=mdfa.states.end(); ++it){
        code += "        case "+QString::number(it->id)+":\n";
        for(auto a: mdfa.alpha.ordered()){
            int t=it->trans.value(a,-1);
            if(t!=-1){
                if(a.compare("letter",Qt::CaseInsensitive)==0){ code += "            if (isalpha((unsigned char)ch)) return "+QString::number(t)+";\n"; }
                else if(a.compare("digit",Qt::CaseInsensitive)==0){ code += "            if (isdigit((unsigned char)ch)) return "+QString::number(t)+";\n"; }
                else { code += "            if (ch=='"+a+"') return "+QString::number(t)+";\n"; }
            }
        }
        code += "            return -1;\n";
    }
    code += "        default: return -1;\n";
    code += "    }\n";
    code += "}\n";
    return code;
}
