#include "semantic_rules.h"

/*
* Semantic Rules & Actions
* Prod No.1 --> E -> E+T, Value[top-2] = Value[top-2] + Value[top] & 2 * pop()
* Prod No.2 --> E -> T, /
* Prod No.3 --> T -> T*F, Value[top-2] = Value[top-2] * Value[top] & 2 * pop()
* Prod No.4 --> T -> F, /
* Prod No.5 --> F -> (E), Value[top-2] = Value[top-1] & 2 * pop()
* Prod No.6 --> F -> a, /
*/

void printValue()
{
    vector<string>::iterator it_val;
    cout << "Current Value Vector - ";
    for (it_val = vec_val.begin(); it_val != vec_val.end(); it_val++)
        cout << *it_val << " ";
    cout << endl;
}

void semantic_actions(int reduced_prod)
{
    float temp;
    switch (reduced_prod)
    {
        case 1: // Reduce to Production No.1 (r1)
            cout << "Reduce to Production No.1" << endl;
            temp = stof(vec_val.end()[-3]) + stof(vec_val.back());
            vec_val.end()[-3] = to_string(temp);
            vec_val.pop_back();
            vec_val.pop_back();
            printValue();
            break;
        case 3: // Reduce to Production No.3 (r3)
            cout << "Reduce to Production No.3" << endl;
            temp = stof(vec_val.end()[-3]) * stof(vec_val.back());
            vec_val.end()[-3] = to_string(temp);
            vec_val.pop_back();
            vec_val.pop_back();
            printValue();
            break;
        case 5: // Reduce to Production No.5 (r5)
            cout << "Reduce to Production No.5" << endl;
            vec_val.end()[-3] = vec_val.end()[-2];
            vec_val.pop_back();
            vec_val.pop_back();
            printValue();
            break;
        default: // Reduce to Production No.2 & 4 & 6 (r2 & r4 & r6)
            break;
    }
}
