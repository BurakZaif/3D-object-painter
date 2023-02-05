#pragma once

#include <vector>

struct HeapElement
{
    int     uniqueId;
    double  weight;
};

class BinaryHeap
{
    private:
    std::vector<HeapElement> elements;
    
    
    void bubbleDown(int hole) {
        int child;
        HeapElement tmp = elements[hole];
    
        for( ; hole*2 <= cnt; hole = child) {
            child = hole * 2;
            if(child != cnt && elements[child + 1].weight < elements[child].weight)
                child++;
            if(elements[child].weight < tmp.weight)
                elements[hole] = elements[child];
            else
                break;
        }
        elements[hole] = tmp;
    }
    
    void bubbleUp(int hole) {
        int parent;
        HeapElement tmp = elements[hole];
    
        for( ; hole/2 > 0 && tmp.weight < elements[hole/2].weight; hole = parent) {
            parent = hole/2;
            elements[hole] = elements[parent];
        }
        elements[hole] = tmp;
    }


    // Do not remove this the tester will utilize this
    // to access the private parts.
    friend class HW3Tester;

    protected:
    public:
        // Constructors & Destructor
                            BinaryHeap();
        //
        int cnt;
        bool                Add(int uniqueId, double weight);
        bool                PopHeap(int& outUniqueId, double& outWeight);
        bool                ChangePriority(int uniqueId, double newWeight);
        int                 HeapSize() const;
};