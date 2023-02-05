#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
    // TODO: or not
    cnt = 0;
    elements.resize(1);
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    for(int i=1; i<=cnt; i++){
        if(elements[i].uniqueId==uniqueId)
            return false;
    }
    HeapElement tmp;
    tmp.uniqueId = uniqueId;
    tmp.weight = weight;
    elements[0] = tmp;

    if((cnt + 1) == ((int) elements.size()))
        elements.resize(elements.size()*2 + 1);

    int hole = ++cnt;
    for( ; tmp.weight < elements[hole/2].weight; hole /= 2 )
        elements[hole] = elements[hole/2];
    elements[hole] = tmp;
    return true;
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    // TODO:
    if(cnt == 0)
        return false;
    
    HeapElement tmp = elements[1];
    outUniqueId = tmp.uniqueId;
    outWeight = tmp.weight;
    elements[1] = elements[cnt--];
    bubbleDown(1);
    return true;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    // TODO:
    int flag =0, tmp=0;
    for(int i=1; i<=cnt; i++  ){
        if(elements[i].uniqueId == uniqueId){
            elements[i].weight = newWeight;
            flag = 1;
            tmp = i;
            break;
        }
    }
    if(flag==1){
    if (tmp != 1 && elements[tmp].weight < elements[tmp/2].weight)
      bubbleUp(tmp);
  else {
    if ((tmp*2 <= cnt && elements[tmp].weight > elements[tmp*2].weight)
        || (tmp*2+1 <= cnt && elements[tmp].weight > elements[tmp*2+1].weight))
        bubbleDown(tmp);
  }
  return true;
}
    else{
        return false;
    }
}

int BinaryHeap::HeapSize() const
{
    // TODO:
    return cnt;
}