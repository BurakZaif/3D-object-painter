#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    // TODO:
    for(int i=0; i<vertexPositions.size(); i++){
        Vertex temporary;
        temporary.id = i;
        temporary.position3D = vertexPositions[i];
        vertices.push_back(temporary);
    }
    for(int i=0; i<vertexPositions.size(); i++){
        std::list<Vertex*> yeni;
        for(int j=0; j<edges.size(); j++){
            if(edges[j].vertexId0==i){
                Vertex *tmp = &vertices[edges[j].vertexId1];
                yeni.push_back(tmp);
            }
            else if(edges[j].vertexId1==i){
                Vertex *tmp = &vertices[edges[j].vertexId0];
                yeni.push_back(tmp);
            }
        }
        adjList.push_back(yeni);
    }
    for(int i=0; i<adjList.size();i++){
        adjList[i].sort();
    }
}


double MeshGraph::AverageDistanceBetweenVertices() const
{
    // TODO:
    double total=0;
    double edgecnt = 0;
    for(int i=0; i<adjList.size(); i++){
        std::list<Vertex*>::const_iterator itr = adjList[i].begin();
        for (;itr != adjList[i].end(); itr++){
            //double distance = sqrt(pow(vertices[i].position3D.x - (*itr)->position3D.x,2) + pow(vertices[i].position3D.y - (*itr)->position3D.y,2) + pow(vertices[i].position3D.z - (*itr)->position3D.z,2));
            double distance = (*itr)->position3D.Distance(vertices[i].position3D,(*itr)->position3D);
            total += distance;
            edgecnt++; 
        }
    }
    return total/edgecnt;
}

double MeshGraph::AverageEdgePerVertex() const
{
    // TODO:
    double edgecnt = 0;
    int count=0;
    for(int i=0; i<adjList.size(); i++, count=i){
        std::list<Vertex*>::const_iterator itr = adjList[i].begin();
        for (;itr != adjList[i].end(); itr++){
            edgecnt++; 
            }
    }
    return (edgecnt/count)/2;
}

int MeshGraph::TotalVertexCount() const
{
    // TODO:
    int count=0;
    for(int i=0; i<adjList.size(); i++, count=i){
        std::list<Vertex*>::const_iterator itr = adjList[i].begin();
        for (;itr != adjList[i].end(); itr++){
            }
    }
    return count;
}

int MeshGraph::TotalEdgeCount() const
{
    // TODO:
    int edgecnt=0;
    for(int i=0; i<adjList.size(); i++){
        std::list<Vertex*>::const_iterator itr = adjList[i].begin();
        for (;itr != adjList[i].end(); itr++){
            edgecnt++; 
            }
    }
    return edgecnt/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    // TODO:
    int edgecnt=0;
    int flag =0;
    for(int i=0; i<adjList.size(); i++){
        if(i == vertexId){
            flag = 1;
        }
    }
    if(flag == 0){
        return -1;
    }
    for(int i=0; i<adjList.size(); i++){
        if(i == vertexId){
            std::list<Vertex*>::const_iterator itr = adjList[i].begin();
            for (;itr != adjList[i].end(); itr++){
                edgecnt++; 
            }
        }
    }
    return edgecnt;
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    // TODO:
    outVertexIds.clear();
    for(int i=0; i<adjList.size(); i++){
        if(i == vertexId){
            std::list<Vertex*>::const_iterator itr = adjList[i].begin();
            for (;itr != adjList[i].end(); itr++){
                outVertexIds.push_back((*itr)->id);
            }
        }
    }
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    // TODO:
    if(vertices.size()<=vertexIdFrom||vertices.size()<=vertexIdTo||vertexIdFrom<0||vertexIdTo<0){
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for(int i=0; i<outputColorAllVertex.size(); i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    BinaryHeap heapcolor;
    std::vector<double> mesafe;
    std::vector<int> previous;
    mesafe.resize(vertices.size());
    previous.resize(vertices.size());
    for(int i=0; i<vertices.size(); i++){
        mesafe[i]= INFINITY;
        previous[i] = -1;
    }
    mesafe[vertexIdFrom] = 0;
    for(int i=0; i<vertices.size(); i++){
        heapcolor.Add(vertices[i].id,mesafe[i]);
    }
    while(heapcolor.HeapSize()!=0){
        Vertex v;
        double x;
        bool a = heapcolor.PopHeap(v.id,x);
        std::list<Vertex*>::const_iterator itr = adjList[v.id].begin();
        for (;itr != adjList[v.id].end(); itr++){
            double distance = (*itr)->position3D.Distance(vertices[v.id].position3D,(*itr)->position3D);
            double new_dist = mesafe[v.id] + distance;
            if(new_dist < mesafe[(*itr)->id]){
                mesafe[(*itr)->id] = new_dist;
                previous[(*itr)->id] = v.id;
                heapcolor.ChangePriority((*itr)->id,new_dist);
            }
        }
    }
    
    int path = vertexIdTo;
    while(path != vertexIdFrom){
        outputColorAllVertex[path] = color;
        path = previous[path];
    }
    outputColorAllVertex[vertexIdFrom] = color;
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    // TODO:
    if(vertices.size()<=vertexId||vertices.size()<=vertexId||vertexId<0||vertexId<0){
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for(int i=0; i<outputColorAllVertex.size();i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    std::vector<bool> check;
    std::vector<double> distance;
    distance.resize(vertices.size(),INFINITY);
    check.resize(vertices.size(),false);
    BinaryHeap heap;
    BinaryHeap depth;
    check[vertexId]=true;
    heap.Add(vertexId, 0);
    if(type == FILTER_BOX){
        if(-alpha<=0 && 0<=alpha){
            outputColorAllVertex[vertexId].r = std::floor((color.r)*1);
            outputColorAllVertex[vertexId].g = std::floor((color.g)*1);
            outputColorAllVertex[vertexId].b = std::floor((color.b)*1);
        }
        else{
            outputColorAllVertex[vertexId].r = std::floor((color.r)*0);
            outputColorAllVertex[vertexId].g = std::floor((color.g)*0);
            outputColorAllVertex[vertexId].b = std::floor((color.b)*0);
        }
    }
    else{
        outputColorAllVertex[vertexId].r = color.r;
        outputColorAllVertex[vertexId].g = color.g;
        outputColorAllVertex[vertexId].b = color.b;
    }
    distance[vertexId]=0;
    depth.Add(vertexId,0);
    double counter = 1;
    int derin = 0;
    while(heap.HeapSize()!=0){
        int myid = 0;
        double myweight = 0;
        int did = 0;
        double dwe = 0;
        heap.PopHeap(myid,myweight);
        depth.PopHeap(did,dwe);
        if(dwe==maxDepth){
            break;
        }
        derin = dwe + 1;
        std::list<Vertex*>::const_iterator itr = adjList[myid].begin();
        for (;itr != adjList[myid].end(); itr++){
            if(!check[(*itr)->id]){
                check[(*itr)->id] = true;
                heap.Add((*itr)->id, counter);
                distance[(*itr)->id] = (*itr)->position3D.Distance(vertices[myid].position3D,(*itr)->position3D) + distance[myid];
                if(type == FILTER_BOX){
                    box(alpha,color,outputColorAllVertex, distance[(*itr)->id], (*itr)->id);
                }
                else{
                    gaus(alpha,color,outputColorAllVertex, distance[(*itr)->id], (*itr)->id);
                }
                depth.Add((*itr)->id, derin);
                counter++;
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    // TODO:
    if(vertices.size()<=vertexId||vertices.size()<=vertexId||vertexId<0||vertexId<0){
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for(int i=0; i<outputColorAllVertex.size();i++){
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    std::vector<bool> check;
    std::vector<double> distance;
    distance.resize(vertices.size(),INFINITY);
    check.resize(vertices.size(),false);
    BinaryHeap heap;
    BinaryHeap depth;
    check[vertexId]=true;
    heap.Add(vertexId, 0);
    if(type == FILTER_BOX){
        if(-alpha<=0 && 0<=alpha){
            outputColorAllVertex[vertexId].r = std::floor((color.r)*1);
            outputColorAllVertex[vertexId].g = std::floor((color.g)*1);
            outputColorAllVertex[vertexId].b = std::floor((color.b)*1);
        }
        else{
            outputColorAllVertex[vertexId].r = std::floor((color.r)*0);
            outputColorAllVertex[vertexId].g = std::floor((color.g)*0);
            outputColorAllVertex[vertexId].b = std::floor((color.b)*0);
        }
    }
    else{
        outputColorAllVertex[vertexId].r = color.r;
        outputColorAllVertex[vertexId].g = color.g;
        outputColorAllVertex[vertexId].b = color.b;
    }
    distance[vertexId]=0;
    depth.Add(vertexId,0);
    double counter = 1;
    int derin = 0;
    while(heap.HeapSize()!=0){
        int myid = 0;
        double myweight = 0;
        int did = 0;
        double dwe = 0;
        heap.PopHeap(myid,myweight);
        depth.PopHeap(did,dwe);
        if(dwe==maxDepth){
            break;
        }
        derin = dwe + 1;
        std::list<Vertex*>::const_iterator itr = adjList[myid].begin();
        for (;itr != adjList[myid].end(); itr++){
            if(!check[(*itr)->id]){
                check[(*itr)->id] = true;
                heap.Add((*itr)->id, counter);
                distance[(*itr)->id] = (*itr)->position3D.Distance((*itr)->position3D,vertices[vertexId].position3D);
                if(type == FILTER_BOX){
                    box(alpha,color,outputColorAllVertex, distance[(*itr)->id], (*itr)->id);
                }
                else{
                    gaus(alpha,color,outputColorAllVertex, distance[(*itr)->id], (*itr)->id);
                }
                depth.Add((*itr)->id, derin);
                counter++;
            }
        }
    }
    
    
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}