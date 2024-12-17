#include <stdint.h>
#include <assert.h>


#define __none_elt__ static_cast<_INT_>(-1)

template <class T, size_t stack_size, typename _INT_ = uint16_t>

    class SmallList{
public:
    const T& operator[](_INT_ p) const{ assert(p>0 && p<size()); return ld.data[p]; }
    T& operator[](_INT_ p) { assert(p>=0 && p<size()); return ld.data[p]; }
    void push_back(const T& d){ ld.data[ld.num++] = d; }
    void clear(){ ld.num = 0; }
    const T* data() const { return ld.data; }
    T* data(){return ld.data; }
    const size_t size() const { return ld.num; }
    const bool empty() { return ld.num==0; }
    const bool full() { return ld.num==stack_size; }
private:
    struct LD{
        T data[stack_size];
        _INT_ num = 0;
    }ld;
};


template <class T, size_t stack_size>
class Queue{
public:
    const unsigned size(){ return ld.num; }
    const bool empty() { return ld.num==0; }
    const bool full() { return ld.num==stack_size; }

    void push_back(const T& d){
        assert(stack_size>ld.num);
        ld.data[ld.head] = d;
        ld.head = (ld.head+1) % stack_size;
        ld.num++;
    }

    T pop_front(){
        assert(ld.num!=0);
        T data = ld.data[ld.tail];
        ld.tail = (ld.tail +1) % stack_size;
        ld.num--;
        return data;
    }

    void clear(){ld.num=0; ld.head=0; ld.tail=0;}

private:
    struct LD{
        T data[stack_size];
        uint16_t head=0, tail=0, num=0;
    }ld;
};



template <class T, size_t stack_size, typename _INT_ = uint16_t>
class FreeList{
public:
    FreeList() : first_free(__none_elt__){}
    
    _INT_ insert(const T& d){
        if(first_free != __none_elt__){
            _INT_ index = first_free;
            first_free = data[first_free].next;
            data[index].data = d;
            return index;
        }else{
            Node nd;
            nd.data = d;
            data.push_back(nd);
            return data.size()-1;
        }
    }
    
    void erase(_INT_ n){
        data[n].next = first_free;
        first_free = n;
    }

    void clear(){
        data.clear();
        first_free = __none_elt__;
    }
    T& operator[](_INT_ n){
        return data[n].data;
    }
    
    const T& operator[](_INT_ n) const{
        return data[n].data;
    }
    
    size_t size(){ return data.size(); }
    bool empty(){ return data.empty(); }
    bool full(){ return data.full(); }
private:
    _INT_ first_free;
    union Node{
        T data;
        _INT_ next;
    };
    SmallList<Node, stack_size, _INT_> data;
};




template <class T, size_t stack_size, typename _INT_>
class List{
public:
    struct Node{
        T data;
        _INT_ next;
    };
    
    class ListIter{
    private:
        friend List;
        FreeList<Node, stack_size, _INT_>* data;
        _INT_ cur, prev;
    public:
        ListIter(FreeList<Node, stack_size, _INT_>* data, _INT_ cur, _INT_ prev) : data(data), cur(cur), prev(prev){}
        T operator*   (){return  (*data)[cur].data;};
        T* operator-> (){return &((*data)[cur].data);};
        ListIter operator++()   {prev=cur; cur=(*data)[cur].next; return *this;}
        ListIter operator++(int){prev=cur; cur=(*data)[cur].next; return *this;}
        friend bool operator!=(const ListIter& a, const ListIter& b){
            return a.cur != b.cur;
        }
        friend bool operator==(const ListIter& a, const ListIter& b){
            return a.cur == b.cur;
        }
    };
    
    
public:
    List(){clear();}
    ListIter begin(){return ListIter(&data, head, __none_elt__);}
    ListIter end()  {return ListIter(&data, __none_elt__, __none_elt__);}  
    
    void push_back(const T& d){
        _INT_ ptr = data.insert({.data=d, .next=__none_elt__});
        if(tail==__none_elt__){ // если это первый элемент
            head = tail = ptr; // привязываем к нему и голову и хвост
        }else{
            data[tail].next = ptr; // связываем с хвостом
            tail = ptr; // меняем хвост на новый
        }
    }
    
    void push_front(const T& d){
        _INT_ ptr = data.insert({d, head});
        if(tail==__none_elt__){ // если первый элемент
            tail = head = ptr; // то и голову и хвост ссылаем на него
        }else{
            head = ptr; // иначе ссылаем только голову
        }
    }
    
    void insert(const ListIter& itr, const T& d){
        if (itr.prev == __none_elt__) {
            push_front(d);
        } else {
            data[itr.prev].next = data.insert({.data=d, .next=itr.cur});
        }
    }

    ListIter erase(const ListIter& itr){
        Node er_nd = data[itr.cur];

        if(itr.prev != __none_elt__)
            data[itr.prev].next = er_nd.next;
        if(itr.cur == head)
            head = er_nd.next;
        if(itr.cur == tail)
            tail = itr.prev;

        data.erase(itr.cur);
        return ListIter(&data, er_nd.next, itr.prev);
    }

    void clear(){
        head = tail = __none_elt__;
        data.clear();
    }    

    bool empty(){ return data.empty(); }
    bool full(){ return data.full(); }
    size_t size(){ return data.size(); }

private:
    _INT_ head=__none_elt__, tail=__none_elt__;
    FreeList<Node, stack_size, _INT_> data;    
};
