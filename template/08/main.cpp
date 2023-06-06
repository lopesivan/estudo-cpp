template <int val, typename next> struct Node {
  static const int value = val;
  typedef next Next;
};

struct NullNode {};

#define LINKED_LIST(...) MAKE_LINKED_LIST(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define MAKE_LINKED_LIST(SEQ) BOOST_PP_SEQ_FOLD_RIGHT(MAKE_NODE, NullNode, SEQ)

#define MAKE_NODE(s, DATA, TAIL) Node<DATA, TAIL>

typedef LINKED_LIST(1, 2, 3, 4, 5) MyLinkedList;

int main() {
  int headValue = MyLinkedList::value;         // headValue é 1
  int secondValue = MyLinkedList::Next::value; // secondValue é 2
  return 0;
}
