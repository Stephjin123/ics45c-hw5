#include "list.hpp"
#include <iostream>

namespace list {

Node* from_string(const char* s) {
    if (s == nullptr || s[0] == '\0') return nullptr;
    Node* head = new Node{s[0], nullptr};
    Node* tail = head;
    for (int i = 1; s[i] != '\0'; ++i) {
        tail->next = new Node{s[i], nullptr};
        tail = tail->next;
    }
    return head;
}

void free(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

void print(std::ostream& out, Node* head) {
    for (Node* p = head; p != nullptr; p = p->next)
        out << p->data;
}

Node* copy(Node* head) {
    if (head == nullptr) return nullptr;
    Node* result = new Node{head->data, nullptr};
    Node* tail = result;
    for (Node* p = head->next; p != nullptr; p = p->next) {
        tail->next = new Node{p->data, nullptr};
        tail = tail->next;
    }
    return result;
}

static int value(Node* p) {
    return p == nullptr ? 0 : static_cast<unsigned char>(p->data);
}

int compare(Node* lhs, Node* rhs) {
    while (lhs != nullptr && rhs != nullptr && lhs->data == rhs->data) {
        lhs = lhs->next;
        rhs = rhs->next;
    }
    return value(lhs) - value(rhs);
}

int compare(Node* lhs, Node* rhs, int n) {
    while (n > 0 && lhs != nullptr && rhs != nullptr && lhs->data == rhs->data) {
        lhs = lhs->next;
        rhs = rhs->next;
        --n;
    }
    if (n <= 0) return 0;
    return value(lhs) - value(rhs);
}

int length(Node* head) {
    int n = 0;
    for (Node* p = head; p != nullptr; p = p->next) ++n;
    return n;
}

Node* reverse(Node* head) {
    Node* result = nullptr;
    for (Node* p = head; p != nullptr; p = p->next)
        result = new Node{p->data, result};
    return result;
}

Node* append(Node* lhs, Node* rhs) {
    if (lhs == nullptr) return copy(rhs);
    Node* result = copy(lhs);
    last(result)->next = copy(rhs);
    return result;
}

int index(Node* head, Node* node) {
    int i = 0;
    for (Node* p = head; p != nullptr; p = p->next, ++i)
        if (p == node) return i;
    return -1;
}

Node* find_char(Node* head, char c) {
    if (c == '\0') return nullptr;
    for (Node* p = head; p != nullptr; p = p->next)
        if (p->data == c) return p;
    return nullptr;
}

Node* find_list(Node* haystack, Node* needle) {
    if (needle == nullptr) return haystack;
    for (Node* start = haystack; start != nullptr; start = start->next) {
        Node* h = start;
        Node* n = needle;
        while (h != nullptr && n != nullptr && h->data == n->data) {
            h = h->next;
            n = n->next;
        }
        if (n == nullptr) return start;
    }
    return nullptr;
}

Node* nth(Node* head, int n) {
    Node* p = head;
    for (int i = 0; i < n && p != nullptr; ++i) p = p->next;
    return p;
}

Node* last(Node* head) {
    Node* p = head;
    while (p->next != nullptr) p = p->next;
    return p;
}

}
