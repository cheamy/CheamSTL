#include <iostream>

class Counter {
public:
    Counter(int val = 0) : count_(val) {}

    // 后置递增操作符重载返回引用
    Counter operator++(int) {
        Counter tmp(*this);
        ++count_;
        return tmp; // 返回临时对象的引用
    }

    int getCount() const {
        return count_;
    }

private:
    int count_;
};

int main() {
    Counter a(5);
    Counter b = a++;
    // Counter b = a++; // 使用后置递增操作符

    std::cout << "a: " << a.getCount() << std::endl; // 输出：a: 6
    std::cout << "b: " << b.getCount() << std::endl; // 输出：b: 5

    return 0;
}
