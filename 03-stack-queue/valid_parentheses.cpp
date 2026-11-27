/**
 * @file valid_parentheses.cpp
 * @brief Balanced parentheses checker using std::stack in C++17.
 * Time Complexity : O(n) | Space Complexity: O(n)
 */

#include <cassert>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isValidParentheses(const string& s) {
    stack<char> st;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            st.push(ch);
        } else {
            if (st.empty()) return false;
            char top = st.top();
            st.pop();
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
    }
    return st.empty();
}

int main() {
    assert(isValidParentheses("()[]{}"));
    assert(isValidParentheses("{[()]}"));
    assert(!isValidParentheses("(]"));
    assert(!isValidParentheses("((("));
    assert(isValidParentheses(""));
    cout << "[PASS] Valid Parentheses OK\n";
    return 0;
}
