#pragma once

using namespace std;

/*
�������ʽ�ķ�
<�޷�������>�ˣ� <����>{<����>}
<��־��>�ˣ� <��ĸ>{<��ĸ>��<����>}
<���ʽ>�ˣ� [������]<��>{<�ӷ������><��>}
<��>�ˣ� <����>{<�˷������><����>}
<����>�ˣ� <��־��>��<�޷�������>��������<���ʽ>������
<�ӷ������>�ˣ� ������
<�˷������>�ˣ� ������

����ʽ
S��� S
S �� E
E �� E+T|E-T|T
T �� T*F|T/F|F
F �� (E)|+i|-i|i
*/

class Analyzer {
private:
    vector<Word>::size_type index;
    stack<int> state;  //״̬ջ
    stack<Word> symbol;  //����ջ
    Word& readWord();
    int readState();
    SLR& compare(int state, Word& input);
    void shift(int state, Word& input);
    void reduce(int index);
    double calculate(double num1, const string& operate, double num2);
    void error(const string& msg, vector<Word>::size_type index = 0);
    void pop();
public:
    Analyzer();
    void show();
    void analySyntax();
};

/*
 * ���캯��
 *
 */
Analyzer::Analyzer() : state(), symbol() {
    index = 0;
}

/*
 * ��ȡ����
 * @return ��ǰ����
 *
 */
Word& Analyzer::readWord() {
    return words[index];
}

/*
 * ��ȡ״̬
 * @return ��ǰ״̬
 *
 */
int Analyzer::readState() {
    return state.top();
}

/*
 * �﷨����
 *
 */
void Analyzer::analySyntax() {
    state.push(0);
    symbol.push(Word(SRP, "#"));  //��ʼ������ջ��״̬ջ
    while (true) {
        int cur_state = readState();
        Word cur_input = readWord();
        SLR result = compare(cur_state, cur_input);
        switch (result.getType()) {
        case 0:
            error("�﷨�����ڱ��ʽ�С�" + cur_input.getWord() + "��������λ�ã�", index);
            break;
        case 1:
            //�ƽ�
            shift(result.getValue(), cur_input);
            break;
        case 2:
            //��Լ
            reduce(result.getValue());
        case 3:
            //GOTO
            state.push(compare(state.top(), symbol.top()).getValue());
            break;
        case 4:
            //ACCEPT
            cout << symbol.top().getValue() << endl;
            return;
        default:
            break;
        }
    }
}


/*
 * �Ƚ�
 * @param int ״̬
 * @param Word& ����
 * @return �ȽϽ��
 *
 */
SLR& Analyzer::compare(int state, Word& input) {
    return slr_table[state][input.getType()];
}

/*
 * �ƽ�
 * @param int ״̬
 * @param Word& ����
 *
 */
void Analyzer::shift(int state, Word& input) {
    this->state.push(state);
    this->symbol.push(input);
    ++index;
}

/*
 * ��Լ
 * @param int ����ʽ���
 *
 */
void Analyzer::reduce(int index) {
    switch (index) {
    //�Ҳ���Ϊ1
    case 2:
    case 5:
    case 8:
    case 12: {
        double num = symbol.top().getValue();
        this->pop();
        symbol.push(Word(left_part[index], num));
    }
        break;
    //�Ҳ���Ϊ2
    case 10:
    case 11: {
        double num = symbol.top().getValue();
        this->pop();
        string s = symbol.top().getWord();
        if (s == "-") num = -num;
        this->pop();
        symbol.push(Word(left_part[index], num));
    }
        break;
    //����
    case 9: {
        this->pop();
        double num = symbol.top().getValue();
        this->pop();
        this->pop();
        symbol.push(Word(left_part[index], num));
    }
        break;
    //�Ҳ���Ϊ3
    case 3:
    case 4:
    case 6:
    case 7: {
        double num1 = symbol.top().getValue();
        this->pop();
        string s = symbol.top().getWord();
        this->pop();
        double num2 = symbol.top().getValue();
        this->pop();
        symbol.push(Word(left_part[index], calculate(num2, s, num1)));
    }
        break;
    default:
        break;
    }
}

/*
 * ����
 * @param double ������1
 * @param const string& �����
 * @param double ������2
 * @return ������
 *
 */
double Analyzer::calculate(double num1, const string& operate, double num2) {
    switch (operate[0]) {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0) error("��������Ϊ0��");
        return num1 / num2;
    default:
        return 0;
    }
}

/*
 * ��ʾ������Ϣ
 * #param const string& ������Ϣ
 * @param vector<Word>::size_type λ��
 *
 */
void Analyzer::error(const string& msg, vector<Word>::size_type index) {
    if (index == 0) cout << msg << endl;
    else cout << msg << (index + 1) << endl;
    exit(0);
}

/*
 * ��ջ
 *
 */
void Analyzer::pop() {
    symbol.pop();
    state.pop();
}

/*
 * ��ʾ�ķ�
 *
 */
void Analyzer::show() {
    cout << "�ÿγ�����ķ�Ϊ��" << endl;
    cout << "S��� S" << endl;
    cout << "S �� E" << endl;
    cout << "E �� E + T | E - T | T" << endl;
    cout << "T �� T*F | T / F | F" << endl;
    cout << "F ��(E) | +i | -i | i" << endl;
}