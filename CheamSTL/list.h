#ifndef CHEAMSTL_LIST_H_
#define CHEAMSTL_LIST_H_

namespace cheamstl
{

    /* 前置声明， 告诉编译器在后面会有一个模板结构体 list_node_base和 list_node
     * 的定义，但是暂时不需要知道其具体细节, 因为 list_node_base
     * list_node与node_traits互相使用，需要有一个入口*/
    template <class T>
    struct list_node_base;
    template <class T>
    struct list_node;

    /* 使用模板，是因为需要用到一个代表所有类的class T或typename T,
    需要在模版中使用有两个成员类型定义： base_ptr：这是一个指向 list_node_base<T>
    类型的指针，表示基类节点的指针类型。 node_ptr：这是一个指向 list_node<T>
    类型的指针，表示派生类节点的指针类型。 node_traits
    结构体的作用是提供了一种通用的方式来定义节点类型的指针类型，方便在双向链表中使用。通过
    node_traits，可以轻松地将 list_node_base 和 list_node
    的指针类型统一起来，而无需关心具体的节点类型。 就是类型萃取，提高通用性
    base_ptr 和 node_ptr都是指针类型别名,因为这里只需要萃取出指针，所以只是类型别名
    */
    template <class T>
    struct node_traits
    {
        typedef list_node_base<T> *base_ptr;
        typedef list_node<T> *node_ptr;
    };

    /* 在 C++ 中，使用 typedef 定义类型别名时，不需要使用 typename
    关键字，而只有在访问模板中的嵌套类型时才需要使用 typename。
    这里使用嵌套类型的原因是，list_node_base 结构体内部需要使用
    node_traits<T>::base_ptr 和 node_traits<T>::node_ptr 这两个类型，
    而这两个类型是由 node_traits 结构体定义的。通过使用嵌套类型，我们可以在
    list_node_base 结构体内部直接使用 base_ptr 和 node_ptr，而不需要每次都写上
    node_traits<T>::。
    判断是否要使用嵌套类型取决于是否需要在某个类或结构体内部直接使用另一个类或结构体中定义的类型。如果需要在内部使用，那么可以使用嵌套类型来简化代码。如果只是需要定义一些类型别名，而不需要在类或结构体内部使用另一个类或结构体中定义的类型，那么可以使用类型别名来实现。使用哪种方式更适合取决于具体的情况和编码风格。。
    */
    /* 定义抽象节点结构 */
    template <class T>
    struct list_node_base
    {
        /* node_traits<T> 这里的T是为了传递模板参数 */
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        /* 这里只需要用基类节点指针 */
        base_ptr prev;
        base_ptr next;

        /* list_node_base
        的默认构造函数被设为默认，因此编译器会生成一个不带参数的默认构造函数。默认构造函数会对成员变量进行默认初始化。
        如果不写默认构造函数，编译器也会自动生成一个默认构造函数。只有当我们需要对成员变量进行特殊的初始化或执行其他操作时，才需要自定义构造函数。
        在这里，默认构造函数是为了确保 prev 和 next 成员变量在创建对象时被初始化为
        nullptr，以便在后续使用中能正确判断节点的前后指针。
        如果没有这个默认构造函数，这些指针可能会被初始化为随机的值，可能导致程序出现未定义的行为。
      */
        list_node_base() = default;

        /*  将当前的 list_node_base 节点转换为具体的节点类型 list_node<T>，通过
        static_cast 将 base_ptr 转换为
        node_ptr。这个函数用于方便访问链表节点中存储的数据。
        /* self() 是一个成员函数，返回的是当前对象的指针，类型是 base_ptr，即
        list_node_base<T>* 类型。然后，通过 static_cast 强制将其转换为 node_ptr
        类型，即 list_node<T>* 类型。这样的转换是因为 list_node<T> 继承自
        list_node_base<T>，因此 list_node<T>* 可以被视为 list_node_base<T>*。由于
        as_node() 函数返回的是 list_node<T>*
        类型的指针，所以为了将其转换为派生类类类型 ，需要使用 static_cast
        进行显式转换。
        /* static_cast 是 C++
        中的一种类型转换操作符，用于执行静态类型转换。它允许在编译时进行类型转换，并且只在编译时检查类型的有效性。static_cast
        主要用于一般的隐式转换和显式转换，可以用于以下情况：
        基本数据类型的转换：可以将一个基本数据类型转换为另一个基本数据类型，如 int
        转换为 double。
        类指针的向上转换：可以将指向派生类对象的指针转换为指向基类对象的指针。
        类指针的向下转换：可以将指向基类对象的指针转换为指向派生类对象的指针，但需要确保指针指向的对象实际上是派生类对象。
        类引用的类型转换：类似于指针的转换，可以将引用类型转换为其他类型的引用。
        显式转换：可以使用 static_cast 进行显式转换，以便在某些情况下进行类型转换。
        使用 static_cast
        时需要注意，它只能用于已知并合法的类型转换，如果转换不合法或不明确，编译器会报错。因此，在使用
        static_cast
        进行类型转换时，应确保转换的类型关系是合理和正确的。如果需要进行更加动态的类型转换，可以使用
        dynamic_cast 或 reinterpret_cast 等更具体的转换操作符。 */
        node_ptr as_node() { return static_cast<node_ptr>(self()); }

        /* 将当前节点从链表中断开，将 prev 和 next
         * 设置为当前节点本身，即实现了链表节点的删除操作。 */
        void unlink() { prev = next = self(); }

        /*  获取当前节点自身的指针，即将当前节点转换为
         * base_ptr，方便在链表操作中使用。 */
        base_ptr self() { return static_cast<base_ptr>(&*this) }
    };

    /* 继承关系
    继承关系	    基类的 public 成员	            基类的 protected 成员
    基类的 private 成员 public	       可以在派生类和外部访问
    可以在派生类访问，不可在外部访问	      不可在派生类和外部访问 protected
    可以在派生类访问，不可在外部访问	 可以在派生类访问，不可在外部访问
    不可在派生类和外部访问 private	       不可在派生类和外部访问
    不可在派生类和外部访问	                 不可在派生类和外部访问 public
    继承：基类的 public 成员在派生类中为 public
    成员，可以在派生类内部和外部直接访问。 protected 继承：基类的 public
    成员在派生类中为 protected 成员，只能在派生类内部直接访问，外部不能访问。
    private 继承：基类的 public 成员在派生类中为 private
    成员，只能在派生类内部直接访问，外部不能访问。
    在使用继承时，可以根据需要选择合适的访问权限，以实现不同的继承关系和封装效果。通常情况下，public
    继承用于实现接口复用，protected 继承用于实现内部实现的继承，private
    继承用于实现实现细节的隐藏。
    */
    /* 定义具体节点结构 */
    template <class T>
    struct list_node : public list_node_base<T>
    {
        /* 派生类 list_node 重新定义 base_ptr 和 node_ptr
        类型别名是为了更方便地使用它们， 并且与基类 list_node_base
        中的类型别名保持一致。虽然你可以直接使用 list_node_base::base_ptr 和
        list_node_base::node_ptr， 但是使用派生类 list_node
        中重新定义的类型别名可以使代码更加清晰和易读。 */
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        T value; // 数据域

        /* 初始化定义 */
        list_node() = default;
        /* 这样设计的目的是为了支持对传入的参数类型进行优化。常量引用 const T&
        可以接受左值和右值作为参数，而右值引用 T&& 只能接受右值作为参数。

        list_node(const T&
        v)：该构造函数接受一个常量引用作为参数。当传入一个左值（例如变量）时，会通过常量引用来进行拷贝构造，避免对原始数据进行修改。当传入一个右值（例如临时对象或使用
        std::move
        转移的对象）时，也可以通过常量引用进行拷贝构造，但在这种情况下会有一次额外的拷贝操作，因为右值被绑定到了常量引用上。虽然效率上可能有一些损失，但该构造函数可以接受更广泛的输入，因为它可以处理左值和右值。
        const不是严格必须的，但是加上可以防止参数值意外改变，可以提高安全性。&是必须的，因为拷贝构造函数会创建一个临时对象，如果不是引用的话，那么创建这个临时对象就要使用拷贝构造函数，从而无穷递归调用

        list_node(T&&
        v)：该构造函数使用了右值引用作为参数。当传入一个左值时，会通过右值引用进行移动构造（通过
        std::move
        将左值转换为右值引用）。当传入一个右值时，会优先调用移动构造，避免额外的拷贝操作，从而提高效率。这样的设计在处理右值时更高效，因为右值引用表示对象即将被销毁，可以直接使用它的资源，而不需要进行深拷贝。

        综上所述，通过同时定义两个构造函数，并分别使用常量引用和右值引用作为参数，可以支持更广泛的输入类型，并在处理右值时提供更高的效率。这种技巧被称为
        "完美转发"，可以在不损失性能的情况下，更好地处理不同类型的输入参数。 */
        list_node(const T &v) : value(v) {}
        list_node(T &&v) : value(v) {}

        /* &*self() 表示首先通过 self() 获取指向当前对象的指针，然后用 *
         * 解引用该指针，得到当前对象本身。最后，再通过 & 获取当前对象本身的地址。 */
        base_ptr as_base() { return static_cast<base_ptr>(&*self()); }
        node_ptr self() { return static_cast<node_ptr>(&*self()); }
    };

    template <class T>
    struct list_iterator
    {
        /* 规定迭代器内可能用到的所有关于实例的变量类型，便于阅读 */
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator self;

        base_ptr node_;

        /* 构造函数 */
        list_iterator() = default;
        list_iterator(base_ptr x) : node_(x) {}
        list_iterator(node_ptr x) : node_(x->as_base()) {}
        list_iterator(const list_iterator &rhs) : node_(rhs.node_) {}

        /* 重载运算符 */
        /* 在这段代码中，reference operator*() const 是一个重载了解引用操作符 *
      的成员函数。这个函数用于获取迭代器所指向的元素的引用，以便可以通过迭代器修改元素。然而，为了确保迭代器的语义与
      STL 标准一致，这里的 operator*() 函数被声明为 const
      成员函数，即它不能修改迭代器本身的状态或属性。

      为什么要将这个函数声明为 const
      呢？这是因为迭代器的主要作用是遍历容器，而在遍历过程中并不应该修改迭代器自身的状态。迭代器的
      operator*()
      函数返回的是当前位置的元素的引用，它并不应该修改迭代器的内部状态，而只是提供对元素的访问。因此，将
      operator*() 声明为 const 可以防止在迭代过程中不小心修改了迭代器。

      在 STL 中，通过将重要的成员函数声明为
      const，可以保证算法在迭代过程中不会改变容器的状态，从而提高了容器和算法的安全性和可组合性
      */
        reference operator*() const { return node_->as_node()->value; }
        pointer operator->() const { return &(operator*()); }
        /* 1.若前置++时不使用引用返回，当++(++a)时，之后求得的a只是进行自增了一次，而外面的++却没有作用。因为外面的++只是对一个临时值进行加加，而不是a本身。

        2.后置++返回的本身就是一个临时值，在函数结束时就会被销毁。因此对临时返回值进行引用没有意义。
      */
        self &operator++()
        {
            node_ = node_->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--()
        {
            /////////////////////////////
            node_ = node_->next;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this --*this;
            return tmp;
        }

        // 重载比较操作符
        bool operator==(const self &rhs) const { return node_ == rhs.node_; }
        bool operator!=(const self &rhs) const { return node_ != rhs.node_; }
    };

    template <class T>
    struct list_const_iterator
    {
        /* 规定迭代器内可能用到的所有关于实例的变量类型，便于阅读 */
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef list_iterator self;

        base_ptr node_;

        /* 构造函数 */
        list_iterator() = default;
        list_iterator(base_ptr x) : node_(x) {}
        list_iterator(node_ptr x) : node_(x->as_base()) {}

        list_const_iterator(const list_const_iterator &rhs) : node_(rhs.node_) {}

        /* 重载运算符 */
        /* 在这段代码中，reference operator*() const 是一个重载了解引用操作符 *
      的成员函数。这个函数用于获取迭代器所指向的元素的引用，以便可以通过迭代器修改元素。然而，为了确保迭代器的语义与
      STL 标准一致，这里的 operator*() 函数被声明为 const
      成员函数，即它不能修改迭代器本身的状态或属性。

      为什么要将这个函数声明为 const
      呢？这是因为迭代器的主要作用是遍历容器，而在遍历过程中并不应该修改迭代器自身的状态。迭代器的
      operator*()
      函数返回的是当前位置的元素的引用，它并不应该修改迭代器的内部状态，而只是提供对元素的访问。因此，将
      operator*() 声明为 const 可以防止在迭代过程中不小心修改了迭代器。

      在 STL 中，通过将重要的成员函数声明为
      const，可以保证算法在迭代过程中不会改变容器的状态，从而提高了容器和算法的安全性和可组合性
      */
        reference operator*() const { return node_->as_node()->value; }
        pointer operator->() const { return &(operator*()); }
        /* 1.若前置++时不使用引用返回，当++(++a)时，之后求得的a只是进行自增了一次，而外面的++却没有作用。因为外面的++只是对一个临时值进行加加，而不是a本身。

        2.后置++返回的本身就是一个临时值，在函数结束时就会被销毁。因此对临时返回值进行引用没有意义。
      */
        self &operator++()
        {
            node_ = node_->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self &operator--()
        {
            /////////////////////////////
            node_ = node_->next;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this --*this;
            return tmp;
        }

        // 重载比较操作符
        bool operator==(const self &rhs) const { return node_ == rhs.node_; }
        bool operator!=(const self &rhs) const { return node_ != rhs.node_; }
    };

    template <class T>
    class list
    {
    public:
        // 需要用到的类型名
        typedef cheamstl::allocator<T> allocator_type;
        typedef cheamstl::allocator<T> data_allocator;
        typedef cheamstl::allocator<list_node_base<T>> base_allocator;
        typedef cheamstl::allocator<list_node<T>> node_allocator;

        typedef typename allocator_type::value_type value_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type defference_type;

        typedef list_iterator<T> iterator;
        typedef list_const_iterator<T> const_iterator;
        typedef cheamstl::reverse_iterator<iterator> reverse_iterator;
        typedef cheamstl::reverse_iterator<const_iterator> const_reverse_iterator;

        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        allocator_type get_allocator()
        {
            return node_allocator();
        }

    private:
        base_ptr node_;
        size_type size_;

    public:
        list() { fill_init(0, value_type()); }

        explicit list(size_type n)
        {
            fill_init(n, value_type());
        }

        list(size_type n, const T &value)
        {
            fill_init(n, value);
        }

        template <class Iter, typename std::enable_if<cheamstl::is_input_iterator<Iter>::value, int>::type = 0>
        list(Iter first, Iter last)
        {
            copy_init(first, last);
        }

        list(std::initalizer_list<T> ilist)
        {
            copy_init(ilist.begin(), ilist.end());
        }

        list(const list &rhs)
        {
            copy_init(rhs.cbegin(), rhs.cend());
        }

        /* 使用 && 表示右值引用的参数是为了支持移动语义。右值引用是C++11引入的特性，允许我们有效地将资源的所有权从一个对象转移到另一个对象，而不需要进行深拷贝操作，从而提高了性能和资源的利用率。在构造函数中使用右值引用参数，通常用于实现移动构造函数和移动赋值运算符。

在上述代码中的 list(list &&rhs) noexcept 构造函数中，rhs 是一个右值引用，这表示我们可以"偷走" rhs 的资源而不是复制它们。具体原因如下：

避免不必要的数据复制： 当你有一个临时对象或者你明确知道不再需要某个对象的资源时，使用右值引用可以避免进行不必要的数据复制。在构造函数中，这意味着你可以移动 rhs 的资源到新对象，而不需要复制每个元素。

提高性能： 移动构造函数允许你有效地将资源的所有权从一个对象转移到另一个对象，而不会涉及额外的内存分配和数据复制操作，从而提高了性能。

资源管理： 右值引用还有助于更有效地管理资源，如内存、文件句柄等。当一个对象被移动时，通常需要将原对象清空，这有助于确保资源的正确释放或转移。

总之，使用右值引用参数允许你实现移动语义，以更高效地管理资源和提高性能。在合适的情况下，使用移动构造函数和右值引用可以避免不必要的数据复制，同时确保资源的正确管理 */
        list(list &&rhs) noexcept : node_(rhs.node_), size_(rhs.size_)
        {
            rhs.node_ = nullptr;
            rhs.size_ = 0;
        }

        list &operator=(const list &rhs)
        {
            if (this != &rhs)
            {
                assign(rhs.begin(), rhs.end());
            }
            return *this;
        }

        list &operator=(list &&rhs) noexcept
        {
            clear();
            splice(end(), rhs);
            return *this;
        }

        list &operator=(std::initializer_list<T>, ilist)
        {
            list tmp(ilist.begin(), ilist.end());
            swap(tmp);
            return *this;
        }

        ~list()
        {
            if (node_)
            {
                clear();
                base_allocator::deallocate(node_);
                node_ = nullptr;
                size_ = 0;
            }
        }

    public:
        /* 迭代器相关操作 */
        /* 通过提供这两种版本的 begin() 和 end() 函数，链表类允许用户根据需要选择迭代器的类型，
        以便在不同的上下文中灵活使用。如果用户希望遍历链表并对其元素进行修改，可以使用可修改的迭代器；
        如果只是希望遍历链表而不修改元素，可以使用常量迭代器，这可以提高代码的安全性和可读性 */
        iterator begin() noexcept
        {
            return node_->next;
        }

        const_iterator begin() noexcept
        {
            return node_->next;
        }

        iterator end() noexcept
        {
            return node_;
        }

        const_iterator end() noexcept
        {
            return node_;
        }

        reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        /* const_iterator cbegin() const noexcept: 这是成员函数 cbegin() 的声明。它返回一个 const_iterator，表示返回的迭代器是常量的，
        不允许修改链表中的元素。函数自身也被声明为 const，以确保在函数内部不会修改类的成员变量，并指定 noexcept，表示这个函数不会抛出异常。 */
        const_iterator cbegin() const noexcept
        {
            return begin();
        }

        const_iterator cend() const noexcept
        {
            return end();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }

        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        /* 容量相关操作 */
        bool empty() const noexcept
        {
            return node_->next == node_;
        }

        size_type size() const noexcept
        {
            return size_;
        }

        size_type max_size() const noexcept {
            // static_cast<size_type>(-1) 执行了显式类型转换，将 -1 转换为 size_type。这个操作将 -1 转换为 size_type 的最大可能值，因为 size_type 是无符号整数类型，不允许负数值。
            return static_cast<size_type>(-1)}

        /* 访问元素相关操作 */
        reference front()
        {
            CHEAMSTL_DEBUG(!empty());
            return *begin();
        }

    private:
        void fill_init(size_type n, const value_type &value);
        void link_node_at_back(base_ptr first, base_ptr last);
    }

    template <class T>
    void list<T>::fill_init(size_type n, const value_type &value)
    {
        node_ = base_allocator::allocate(1);
        node_->unlink();
        size_ = n;
        try
        {
            for (; n > 0; --n)
            {
                auto node = create_node(value);
                link_nodes_at_back(node->as_base(), node->as_base());
            }
        }
        catch (...)
        {
            clear();
            base_allocator::deallocate(node_);
            node_ = nullptr;
            throw;
        }
    }

    template <class T>
    void list<T>::link_node_at_back(base_ptr first, base_ptr last)
    {
        last->next = node_;
        first->prev = node_->prev;
        first->prev->next = first;
        node_->prev = last;
    }

    template <class T>
    template <class Iter>
    void list<T>::copy_init(Iter first, Iter last)
    {
        node_ = base_allocator::allocate(1);
        node_->unlink();
        size_type n = cheamstl::distance(first, last);
        size_ = n;
        try
        {
            for (; n > 0; --n, ++first)
            {
                auto node = create_node(*first);
                link_nocdes_at_back(node->as_base(), node->as_base());
            }
        }
        catch (...)
        {
            clear();
            base_allocator::deallocate(node_);
            node_ = nullptr;
            throw;
        }
    }

} // namespace cheamstl

#endif