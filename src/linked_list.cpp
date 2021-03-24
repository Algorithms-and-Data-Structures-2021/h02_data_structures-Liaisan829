#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        auto node = new Node(e, nullptr);
        if (size_ == 0) {
            head_ = node;
            tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);

        // Tip 1: вставка элементов на позицию size эквивалентно операции добавления в конец
        auto node = new Node(e, nullptr);
        // Tip 2: рассмотрите несколько случаев:
        //        (1) список пустой,
        if (size_ == 0) {
            head_ = node;
            tail_ = node;
            size_++;
        }
        //        (2) добавляем в начало списка,
        if (index == 0) {
            head_ = node;
            size_++;
        }

        //        (3) добавляем в конец списка
        if (index == size_) {
            tail_->next = node;
            tail_ = node;
            size_++;
        }
        //        (4) все остальное
        if (index > 0 && index < size_) {
            node->next = find_node(index);
            find_node(index-1) -> next = node;
            size_++;

        }
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);

        Node *node = find_node(index);
        node->data = e;

    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
        Element data;
        if (index == 0) {
            Node *delete_node = head_;
            head_ = head_->next;
            data = delete_node->data;
            delete delete_node;
            size_--;
        } else {

            Node *node = find_node(index - 1);
            Node *delete_node = node->next;
            node->next = delete_node->next;
            data = delete_node->data;
            delete delete_node;
            size_--;
        }
        return data;
    }

    void LinkedList::Clear() {

        for (Node *curr_node = head_; curr_node != nullptr;) {
            Node *delete_node = curr_node;
            curr_node = curr_node->next;
            delete delete_node;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        Node* node = find_node(index);
        return node->data;
    }

    int LinkedList::IndexOf(Element e) const {

        int count = 0;
        for (Node *node = head_; node != nullptr; node = node->next) {
            if (node->data == e) return count;
            count++;
        }
        return -1;

    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);

        if (index == 0) return head_;
        if (index == size_ - 1) return tail_;
        int counter = 0;

        for (Node *current_node = head_; current_node != nullptr; current_node = current_node->next) {
            if (counter == index) return current_node;
            counter++;
        }
        return {};
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis