#ifndef MYPQ_H
#define MYPQ_H

class PriorityQueue
{
public:
    virtual ~PriorityQueue() {}
    virtual void insert(int vertex, double priority) = 0;
    virtual int extractMin() = 0;
    virtual void decreaseKey(int vertex, double newPriority) = 0;
    virtual bool contains(int vertex) const = 0;
    virtual bool isEmpty() const = 0;

    // 创建优先队列实例的工厂方法
    static PriorityQueue *create();
};

#endif // MYPQ_H