/**
 * @file singly_linked_list.cpp
 * @brief Singly Linked List insert, reverse, and search in C++17.
 * Time Complexity : O(n) | Space Complexity: O(1) auxiliary
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int data;
    Node* next;
    explicit Node(int val) : data(val), next(nullptr) {}
};

class SinglyLinkedList {
    Node* head = nullptr;
public:
    ~SinglyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void pushBack(int val) {
        Node* n = new Node(val);
        if (!head) { head = n; return; }
        Node* curr = head;
        while (curr->next) curr = curr->next;
        curr->next = n;
    }
    void reverse() {
        Node* prev = nullptr;
        Node* curr = head;
        while (curr) {
            Node* nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }
        head = prev;
    }
    bool search(int key) const {
        for (Node* curr = head; curr; curr = curr->next) {
            if (curr->data == key) return true;
        }
        return false;
    }
    vector<int> toVector() const {
        vector<int> v;
        for (Node* curr = head; curr; curr = curr->next) v.push_back(curr->data);
        return v;
    }
};

int main() {
    SinglyLinkedList list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    assert(list.search(20));
    assert(!list.search(99));
    list.reverse();
    assert(list.toVector() == vector<int>({30, 20, 10}));
    cout << "[PASS] Linked List OK\n";
    return 0;
}
