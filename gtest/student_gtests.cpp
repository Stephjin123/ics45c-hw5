#include <gtest/gtest.h>

#include "list.hpp"

using namespace std;
using list::Node;

TEST(ListTests, FromStringBasic) {
    Node* const foo_list_head = list::from_string("foo");
    Node* foo_list = foo_list_head;

    EXPECT_EQ(foo_list->data, 'f');
    // ASSERT instead of EXPECT means: end the test here if this fails, do not try to continue
    // running. This is useful to prevent early crashes.
    ASSERT_NE(foo_list->next, nullptr);

    foo_list = foo_list->next;
    EXPECT_EQ(foo_list->data, 'o');
    ASSERT_NE(foo_list->next, nullptr);

    foo_list = foo_list->next;
    EXPECT_EQ(foo_list->data, 'o');
    ASSERT_EQ(foo_list->next, nullptr);

    list::free(foo_list_head);
}

TEST(ListTests, Length) {
    Node* const head = list::from_string("foo");
    EXPECT_EQ(list::length(head), 3);
    list::free(head);
}

// Add remaining tests below. All tests should follow
// the format of `TEST(ListTests, <TestName>){}`.


#include <string>

namespace {
std::string str(list::Node* h) {
    std::string s;
    for (; h; h = h->next) s += h->data;
    return s;
}
bool shares(list::Node* a, list::Node* b) {
    for (auto p = a; p; p = p->next)
        for (auto q = b; q; q = q->next)
            if (p == q) return true;
    return false;
}
}

TEST(ListTests, FromStringEmptyIsNull) { EXPECT_EQ(list::from_string(""), nullptr); }
TEST(ListTests, FromStringOneNode) {
    auto h = list::from_string("x");
    ASSERT_NE(h, nullptr); EXPECT_EQ(h->data, 'x'); EXPECT_EQ(h->next, nullptr);
    list::free(h);
}
TEST(ListTests, FromStringOrder) {
    auto h = list::from_string("abc");
    EXPECT_EQ(str(h), "abc");
    list::free(h);
}
TEST(ListTests, LengthCases) {
    EXPECT_EQ(list::length(nullptr), 0);
    auto a = list::from_string("a"), b = list::from_string("abc");
    EXPECT_EQ(list::length(a), 1); EXPECT_EQ(list::length(b), 3);
    list::free(a); list::free(b);
}
TEST(ListTests, FreeNullIsSafe) { list::free(nullptr); SUCCEED(); }

TEST(ListTests, CopyNull) { EXPECT_EQ(list::copy(nullptr), nullptr); }
TEST(ListTests, CopyOneNode) {
    auto a = list::from_string("z"); auto b = list::copy(a);
    EXPECT_EQ(str(b), "z"); EXPECT_FALSE(shares(a, b));
    list::free(a); list::free(b);
}
TEST(ListTests, CopyIsDeepAndInputUnchanged) {
    auto a = list::from_string("abc"); auto b = list::copy(a);
    EXPECT_EQ(str(b), "abc"); EXPECT_EQ(str(a), "abc"); EXPECT_FALSE(shares(a, b));
    list::free(a); list::free(b);
}

TEST(ListTests, CompareEmpties) {
    auto ab = list::from_string("ab");
    EXPECT_EQ(list::compare(nullptr, nullptr), 0);
    EXPECT_LT(list::compare(nullptr, ab), 0);
    EXPECT_GT(list::compare(ab, nullptr), 0);
    list::free(ab);
}
TEST(ListTests, CompareEqualAndPrefix) {
    auto ab = list::from_string("ab"), abc = list::from_string("abc"), abc2 = list::from_string("abc");
    EXPECT_EQ(list::compare(abc, abc2), 0);
    EXPECT_LT(list::compare(ab, abc), 0);
    EXPECT_GT(list::compare(abc, ab), 0);
    list::free(ab); list::free(abc); list::free(abc2);
}
TEST(ListTests, CompareDifferAtFirstAndLast) {
    auto abc = list::from_string("abc"), abd = list::from_string("abd"), bbc = list::from_string("bbc");
    EXPECT_LT(list::compare(abc, abd), 0); EXPECT_GT(list::compare(abd, abc), 0);
    EXPECT_LT(list::compare(abc, bbc), 0); EXPECT_GT(list::compare(bbc, abc), 0);
    list::free(abc); list::free(abd); list::free(bbc);
}
TEST(ListTests, CompareNCases) {
    auto abc = list::from_string("abc"), abd = list::from_string("abd"), ab = list::from_string("ab");
    EXPECT_EQ(list::compare(abc, abd, 0), 0);
    EXPECT_EQ(list::compare(abc, abd, 1), 0);
    EXPECT_EQ(list::compare(abc, abd, 2), 0);
    EXPECT_LT(list::compare(abc, abd, 3), 0);
    EXPECT_GT(list::compare(abd, abc, 3), 0);
    EXPECT_LT(list::compare(ab, abc, 3), 0);
    EXPECT_EQ(list::compare(ab, abc, 2), 0);
    EXPECT_EQ(list::compare(abc, abc, 100), 0);
    EXPECT_EQ(list::compare(nullptr, nullptr, 5), 0);
    list::free(abc); list::free(abd); list::free(ab);
}

TEST(ListTests, NthReturnsCorrectNode) {
    auto h = list::from_string("abc");
    EXPECT_EQ(list::nth(h, 0), h);
    EXPECT_EQ(list::nth(h, 1), h->next);
    EXPECT_EQ(list::nth(h, 2), h->next->next);
    list::free(h);
}
TEST(ListTests, LastReturnsLastNode) {
    auto one = list::from_string("a"), two = list::from_string("ab"), three = list::from_string("abc");
    EXPECT_EQ(list::last(one), one);
    EXPECT_EQ(list::last(two), two->next);
    EXPECT_EQ(list::last(three), three->next->next);
    list::free(one); list::free(two); list::free(three);
}
TEST(ListTests, IndexOfNodes) {
    auto h = list::from_string("abc");
    EXPECT_EQ(list::index(h, h), 0);
    EXPECT_EQ(list::index(h, h->next), 1);
    EXPECT_EQ(list::index(h, h->next->next), 2);
    list::free(h);
}
TEST(ListTests, IndexWithDuplicateData) {
    auto h = list::from_string("aaa");
    EXPECT_EQ(list::index(h, h->next->next), 2);
    list::free(h);
}

TEST(ListTests, FindCharCases) {
    auto h = list::from_string("abcb");
    EXPECT_EQ(list::find_char(h, 'a'), h);
    EXPECT_EQ(list::find_char(h, 'b'), h->next);
    EXPECT_EQ(list::find_char(h, 'c'), h->next->next);
    EXPECT_EQ(list::find_char(h, 'z'), nullptr);
    EXPECT_EQ(list::find_char(h, '\0'), nullptr);
    EXPECT_EQ(list::find_char(nullptr, 'a'), nullptr);
    list::free(h);
}
TEST(ListTests, FindCharLastNode) {
    auto h = list::from_string("xyz");
    EXPECT_EQ(list::find_char(h, 'z'), h->next->next);
    list::free(h);
}

TEST(ListTests, FindListNullCases) {
    auto h = list::from_string("abc"), n = list::from_string("a");
    EXPECT_EQ(list::find_list(h, nullptr), h);
    EXPECT_EQ(list::find_list(nullptr, nullptr), nullptr);
    EXPECT_EQ(list::find_list(nullptr, n), nullptr);
    list::free(h); list::free(n);
}
TEST(ListTests, FindListAtStart) {
    auto h = list::from_string("abc"), n = list::from_string("ab");
    EXPECT_EQ(list::find_list(h, n), h);
    list::free(h); list::free(n);
}
TEST(ListTests, FindListPartialMatchThenMatch) {
    auto h = list::from_string("aaab"), n = list::from_string("aab");
    EXPECT_EQ(list::find_list(h, n), h->next);
    list::free(h); list::free(n);
}
TEST(ListTests, FindListRestartAfterFail) {
    auto h = list::from_string("abcabd"), n = list::from_string("abd");
    EXPECT_EQ(list::find_list(h, n), list::nth(h, 3));
    list::free(h); list::free(n);
}
TEST(ListTests, FindListNoMatchAndLongNeedle) {
    auto h = list::from_string("ababc"), n1 = list::from_string("abd"), n2 = list::from_string("ababcx");
    EXPECT_EQ(list::find_list(h, n1), nullptr);
    EXPECT_EQ(list::find_list(h, n2), nullptr);
    list::free(h); list::free(n1); list::free(n2);
}
TEST(ListTests, FindListWholeAndAtEnd) {
    auto h = list::from_string("hello"), w = list::from_string("hello"), e = list::from_string("lo");
    EXPECT_EQ(list::find_list(h, w), h);
    EXPECT_EQ(list::find_list(h, e), list::nth(h, 3));
    list::free(h); list::free(w); list::free(e);
}
TEST(ListTests, FindListPrefixAtEndOnly) {
    auto h = list::from_string("xxab"), n = list::from_string("abc");
    EXPECT_EQ(list::find_list(h, n), nullptr);
    list::free(h); list::free(n);
}
TEST(ListTests, FindListSingleCharNeedle) {
    auto h = list::from_string("xyz"), n = list::from_string("z");
    EXPECT_EQ(list::find_list(h, n), h->next->next);
    list::free(h); list::free(n);
}

TEST(ListTests, ReverseNull) { EXPECT_EQ(list::reverse(nullptr), nullptr); }
TEST(ListTests, ReverseOneTwoThree) {
    auto a = list::from_string("a"), b = list::from_string("ab"), c = list::from_string("abc");
    auto ra = list::reverse(a), rb = list::reverse(b), rc = list::reverse(c);
    EXPECT_EQ(str(ra), "a"); EXPECT_EQ(str(rb), "ba"); EXPECT_EQ(str(rc), "cba");
    EXPECT_EQ(str(a), "a");  EXPECT_EQ(str(b), "ab"); EXPECT_EQ(str(c), "abc");
    EXPECT_FALSE(shares(a, ra)); EXPECT_FALSE(shares(b, rb)); EXPECT_FALSE(shares(c, rc));
    list::free(a); list::free(b); list::free(c);
    list::free(ra); list::free(rb); list::free(rc);
}

TEST(ListTests, AppendWithEmpties) {
    auto x = list::from_string("ab");
    EXPECT_EQ(list::append(nullptr, nullptr), nullptr);
    auto l = list::append(nullptr, x), r = list::append(x, nullptr);
    EXPECT_EQ(str(l), "ab"); EXPECT_EQ(str(r), "ab"); EXPECT_EQ(str(x), "ab");
    EXPECT_FALSE(shares(l, x)); EXPECT_FALSE(shares(r, x));
    list::free(x); list::free(l); list::free(r);
}
TEST(ListTests, AppendTwoListsDeep) {
    auto a = list::from_string("ab"), b = list::from_string("cd");
    auto c = list::append(a, b);
    EXPECT_EQ(str(c), "abcd"); EXPECT_EQ(str(a), "ab"); EXPECT_EQ(str(b), "cd");
    EXPECT_FALSE(shares(a, c)); EXPECT_FALSE(shares(b, c));
    list::free(a); list::free(b); list::free(c);
}
TEST(ListTests, AppendSingleNodes) {
    auto a = list::from_string("a"), b = list::from_string("b");
    auto c = list::append(a, b);
    EXPECT_EQ(str(c), "ab"); EXPECT_EQ(list::length(c), 2);
    list::free(a); list::free(b); list::free(c);
}
