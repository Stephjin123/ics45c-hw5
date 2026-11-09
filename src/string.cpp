#include "string.hpp"

#include <istream>
#include <ostream>
#include <utility>

String::String(const char *s) : head(list::from_string(s)) {}

String::String(const String &s) : head(list::copy(s.head)) {}

String::String(String &&s) : head(s.head) {
    s.head = nullptr;
}

String::String(list::Node *head) : head(head) {}

void String::swap(String &s) {
    std::swap(head, s.head);
}

String &String::operator=(const String &s) {
    if (this != &s) {
        list::Node *fresh = list::copy(s.head);
        list::free(head);
        head = fresh;
    }
    return *this;
}

String &String::operator=(String &&s) {
    if (this != &s) {
        list::free(head);
        head = s.head;
        s.head = nullptr;
    }
    return *this;
}

bool String::in_bounds(int index) const {
    return index >= 0 && index < size();
}

char String::operator[](int index) const {
    if (!in_bounds(index)) return '\0';
    return list::nth(head, index)->data;
}

int String::size() const {
    return list::length(head);
}

String String::reverse() const {
    return String(list::reverse(head));
}

int String::indexOf(char c) const {
    list::Node *p = list::find_char(head, c);
    return p ? list::index(head, p) : -1;
}

int String::indexOf(const String &s) const {
    if (s.head == nullptr) return 0;
    list::Node *p = list::find_list(head, s.head);
    return p ? list::index(head, p) : -1;
}

bool String::operator==(const String &s) const {
    return list::compare(head, s.head) == 0;
}

std::strong_ordering String::operator<=>(const String &s) const {
    return list::compare(head, s.head) <=> 0;
}

String String::operator+(const String &s) const {
    return String(list::append(head, s.head));
}

String &String::operator+=(const String &s) {
    list::Node *tail = list::copy(s.head);
    if (head == nullptr)
        head = tail;
    else
        list::last(head)->next = tail;
    return *this;
}

void String::print(std::ostream &out) const {
    for (list::Node *p = head; p != nullptr; p = p->next)
        out << p->data;
}

void String::read(std::istream &in) {
    char buf[1024];
    in >> buf;
    list::Node *fresh = list::from_string(buf);
    list::free(head);
    head = fresh;
}

String::~String() {
    list::free(head);
}

std::ostream &operator<<(std::ostream &out, const String &s) {
    s.print(out);
    return out;
}

std::istream &operator>>(std::istream &in, String &s) {
    s.read(in);
    return in;
}

