#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
    //Just going use vector named elements which is declared in BinaryHeap.h
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    int size = elements.size();
    int i = 0;
    
    for(i=0; i < size ;i++){
        
        if(elements[i].uniqueId == uniqueId){
            return false;
        }
    }
    
    //If somehow we came here, then it means we don't have that element in Heap
    
    //Now do the adding:   
    //Creation of new item:

    HeapElement to_add;
    to_add.uniqueId = uniqueId;
    to_add.weight = weight;

    elements.push_back(to_add);
    int hole = elements.size()-1;
    
    //Bubble up part:
    
    while( 0 < hole && elements[hole].weight < elements[(hole-1)/2].weight ){
        
        //Swap the parent and child:
        HeapElement tmp = elements[hole];
        elements[hole] = elements[(hole-1)/2];
        elements[(hole-1)/2] = tmp;
        
        //New hole index:
        hole = (hole-1)/2;
    }
    
    return true;
    
}


bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    
    if(elements.size() == 0){
        return false;
    }
    
    outUniqueId = elements[0].uniqueId;
    outWeight = elements[0].weight;
    
    //Change the first and last element
    elements[0] = elements[elements.size()-1];
    elements.pop_back();
    
    //Bubble down:
    
    int child = 1; //index of first child (left)
    int hole = 0; //start index of parent
    
    HeapElement tmp = elements[hole];
    
    for( ; (hole*2) + 1 < elements.size(); hole = child ){
        
        child = (hole *2) + 1;
        
        if( child != elements.size()-1 && elements[child + 1].weight < elements[child].weight)
               child++;
        
        if( elements[ child ].weight < tmp.weight ){
            elements[ hole ] = elements[ child ];     
        }
        
        else{
            break;     
        }
        
    }
    
    elements[ hole ] = tmp;
    
    return true;
    
    
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    
    int size = elements.size();
    int i = 0;
    double prev_val;
    
    for(i=0; i < size ;i++){
        
        if(elements[i].uniqueId == uniqueId){
            
            prev_val = elements[i].weight; 
            elements[i].weight = newWeight;
            break;
        }
    }
    
    if(i == size){
        return false;
    }
    
    else{
        
        //i is the index of the requested place:
        
        if(prev_val < newWeight){
            
            //We need to bubble down 
            //Bubble down:
            
            int child; //index of first child (left)
            int hole = i; //start index of parent
            
            HeapElement tmp = elements[hole];
            
            for( ; (hole*2) + 1 < elements.size(); hole = child ){
                
                child = (hole *2) + 1;
                
                if( child != elements.size()-1 && elements[child + 1].weight < elements[child].weight)
                       child++;
                
                if( elements[ child ].weight < tmp.weight ){
                    elements[ hole ] = elements[ child ];     
                }
                
                else{
                    break;     
                }
                
            }
            
            elements[ hole ] = tmp;
            
        }
        
        else if(prev_val > newWeight){
            //We need to bubble up
            
            //Bubble up part:
            int hole = i;
            
            while( 0 < hole && elements[hole].weight < elements[(hole-1)/2].weight ){
                
                //Swap the parent and child:
                HeapElement tmp = elements[hole];
                elements[hole] = elements[(hole-1)/2];
                elements[(hole-1)/2] = tmp;
                
                //New hole index:
                hole = (hole-1)/2;
            }
        }
    }
    
    return true;
}

int BinaryHeap::HeapSize() const
{    
    return elements.size();
}


bool BinaryHeap::is_it_there(int uniqueId){
    
    if( uniqueId >= elements.size() or uniqueId < 0){
    
        return false;
    }
    
    else{
        
        return true;
    }
    
}
