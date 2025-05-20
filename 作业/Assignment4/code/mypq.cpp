#include "mypq.h"
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
using namespace std;

class MinHeapPQ : public PriorityQueue
{
private:
    struct HeapElement
    {
        int vertex;
        double priority;
    };
    vector<HeapElement> heap;
    unordered_map<int, size_t> vertexToIndex;

    void heapifyUp(size_t index)
    {
        while (index > 0)
        {
            size_t parent = (index - 1) / 2;
            if (heap[parent].priority <= heap[index].priority)
                break;
            swap(heap[parent], heap[index]);
            vertexToIndex[heap[parent].vertex] = parent;
            vertexToIndex[heap[index].vertex] = index;
            index = parent;
        }
    }

    void heapifyDown(size_t index)
    {
        size_t left, right, smallest;
        while (true)
        {
            left = 2 * index + 1;
            right = 2 * index + 2;
            smallest = index;
            if (left < heap.size() && heap[left].priority < heap[smallest].priority)
                smallest = left;
            if (right < heap.size() && heap[right].priority < heap[smallest].priority)
                smallest = right;
            if (smallest == index)
                break;
            swap(heap[index], heap[smallest]);
            vertexToIndex[heap[index].vertex] = index;
            vertexToIndex[heap[smallest].vertex] = smallest;
            index = smallest;
        }
    }

public:
    ~MinHeapPQ() override = default;

    void insert(int vertex, double priority) override
    {
        heap.push_back({vertex, priority});
        vertexToIndex[vertex] = heap.size() - 1;
        heapifyUp(heap.size() - 1);
    }

    int extractMin() override
    {
        if (heap.empty())
            return -1;
        int minVertex = heap[0].vertex;
        vertexToIndex.erase(minVertex);
        if (heap.size() > 1)
        {
            heap[0] = heap.back();
            vertexToIndex[heap[0].vertex] = 0;
        }
        heap.pop_back();
        if (!heap.empty())
            heapifyDown(0);
        return minVertex;
    }

    void decreaseKey(int vertex, double newPriority) override
    {
        if (!contains(vertex))
            return;
        size_t index = vertexToIndex[vertex];
        if (newPriority >= heap[index].priority)
            return;
        heap[index].priority = newPriority;
        heapifyUp(index);
    }

    bool contains(int vertex) const override
    {
        return vertexToIndex.find(vertex) != vertexToIndex.end();
    }

    bool isEmpty() const override
    {
        return heap.empty();
    }
};

// 工厂方法定义
PriorityQueue *PriorityQueue::create()
{
    return new MinHeapPQ();
}
