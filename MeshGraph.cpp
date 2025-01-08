#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

MeshGraph::MeshGraph(const std::vector<Double3> &vertexPositions,
                     const std::vector<IdPair> &edges)
{

    int i = 0;
    for (i = 0; i < vertexPositions.size(); i++)
    {
        // Creating the new "Vertex node":
        Vertex new_element;
        new_element.id = i;
        new_element.position3D = vertexPositions[i];

        // Adding:
        vertices.push_back(new_element);
    }

    int k = 0;
    for (k = 0; k < vertices.size(); k++)
    {
        std::list<Vertex *> to_be_added;

        int j = 0;
        for (j = 0; j < edges.size(); j++)
        {

            int first = edges[j].vertexId0;
            int second = edges[j].vertexId1;

            if (first == k or second == k)
            {

                if (first == k)
                {
                    Vertex *ptr = &vertices[second];
                    to_be_added.push_back(ptr);
                }

                else if (second == k)
                {
                    Vertex *ptr = &vertices[first];
                    to_be_added.push_back(ptr);
                }
            }
        }

        adjList.push_back(to_be_added);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{

    double counter = 0;
    double result = 0;

    for (int i = 0; i < adjList.size(); i++)
    {
        int base = i; // This is the "base"
        std::list<Vertex *>::const_iterator it;

        for (it = adjList[i].begin(); it != adjList[i].end(); ++it)
        {
            result = result + Double3::Distance(vertices[base].position3D, (*it)->position3D);
            counter++;
        }
    }

    result = result / 2;
    return result / (counter / 2);
}

double MeshGraph::AverageEdgePerVertex() const
{
    double counter = 0;

    for (int i = 0; i < adjList.size(); i++)
    {
        counter = counter + adjList[i].size();
    }

    return counter / (2 * (vertices.size()));
}

int MeshGraph::TotalVertexCount() const
{
    return vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int counter = 0;

    for (int i = 0; i < adjList.size(); i++)
    {
        counter = counter + adjList[i].size();
    }

    return counter / 2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    for (int i = 0; i < vertices.size(); i++)
    {

        if (vertices[i].id == vertexId)
        {
            return adjList[i].size();
        }
    }

    return -1;
}

void MeshGraph::ImmediateNeighbours(std::vector<int> &outVertexIds,
                                    int vertexId) const
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].id == vertexId)
        {
            std::list<Vertex *>::const_iterator it;

            for (it = adjList[i].begin(); it != adjList[i].end(); ++it)
            {
                outVertexIds.push_back((*it)->id);
            }
        }
    }
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color> &outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color &color) const
{

    if ((vertexIdFrom >= vertices.size() or vertexIdFrom < 0) or (vertexIdTo >= vertices.size() or vertexIdTo < 0))
    {

        // If given vertexes are out of range (e.g it's not in the graph...)

        outputColorAllVertex.resize(0);
    }

    else
    {

        outputColorAllVertex.resize(0);

        std::vector<int> previous(vertices.size(), -1);
        std::vector<bool> visited(vertices.size(), false);
        std::vector<double> distances(vertices.size(), INFINITY);

        distances[vertexIdFrom] = 0;

        BinaryHeap my_heap;
        my_heap.Add(vertexIdFrom, distances[vertexIdFrom]);

        while (my_heap.HeapSize() > 0)
        {
            int current_vertex_id;
            double weight;

            my_heap.PopHeap(current_vertex_id, weight);

            if (visited[current_vertex_id])
            {
                continue;
            }

            else
            {
                for (std::list<Vertex *>::const_iterator it = adjList[current_vertex_id].begin(); it != adjList[current_vertex_id].end(); it++)
                {
                    double dist = distances[current_vertex_id] + Double3::Distance(vertices[current_vertex_id].position3D, (*it)->position3D);

                    if (dist < distances[(*it)->id])
                    {
                        previous[(*it)->id] = current_vertex_id;
                        distances[(*it)->id] = dist;

                        if (my_heap.is_it_there((*it)->id))
                        {
                            my_heap.ChangePriority((*it)->id, dist);
                        }

                        else
                        {
                            my_heap.Add((*it)->id, dist);
                        }
                    }
                }

                visited[current_vertex_id] = true;
            }
        }

        std::vector<int> the_path;
        int tracker = vertexIdTo;

        while (previous[tracker] != -1)
        {
            the_path.push_back(previous[tracker]);
            tracker = previous[tracker];
        }

        the_path.push_back(vertexIdTo);

        Color black;
        black.r = 0;
        black.b = 0;
        black.g = 0;

        for (int i = 0; i < vertices.size(); i++)
        {
            outputColorAllVertex.push_back(black);
        }

        for (int i = 0; i < the_path.size(); i++)
        {
            outputColorAllVertex[the_path[i]] = color;
        }
    }
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color> &outputColorAllVertex,
                                     int vertexId, const Color &color,
                                     int maxDepth, FilterType type,
                                     double alpha) const
{
    if (vertexId >= vertices.size() or vertexId < 0)
    {
        // If given vertexId is out of range (e.g it's not in the graph...)
        outputColorAllVertex.resize(0);
    }

    else
    {
        std::vector<int> all_depths(vertices.size(), -1);
        std::vector<bool> visited(vertices.size(), false);

        // Made everything black firstly

        all_depths[vertexId] = 0;
        outputColorAllVertex.resize(0);

        Color black;
        black.r = 0;
        black.b = 0;
        black.g = 0;

        for (int i = 0; i < vertices.size(); i++)
        {
            outputColorAllVertex.push_back(black);
        }

        std::vector<int> valid_vertice_ids;
        std::vector<double> filter_coef(vertices.size(), -1);
        std::vector<double> distances(vertices.size(), INFINITY);

        distances[vertexId] = 0;
        double first_one_filtered;
        valid_vertice_ids.push_back(vertexId);

        if (type == FILTER_GAUSSIAN)
        {
            first_one_filtered = std::exp(-(pow(0.0, 2) / pow(alpha, 2)));
        }

        if (type == FILTER_BOX)
        {
            if (-alpha <= 0.0 and 0.0 <= alpha)
            {
                first_one_filtered = 1;
            }

            else
            {
                first_one_filtered = 0;
            }
        }

        filter_coef[vertexId] = first_one_filtered;

        int count = 0; // This is for the que impl.

        BinaryHeap my_min_heap;
        my_min_heap.Add(vertexId, count);

        int depth = 0;
        int base = vertexId;

        while (my_min_heap.HeapSize() > 0)
        {

            int curr_vertex; // Unique ID
            double sira;     // Weight

            my_min_heap.PopHeap(curr_vertex, sira);

            if (visited[curr_vertex])
            {
                continue;
            }

            else
            {
                // std:: cout << curr_vertex << '\n';
                visited[curr_vertex] = true;
                int curr;

                std::vector<int> sorted;

                for (std::list<Vertex *>::const_iterator it = adjList[curr_vertex].begin(); it != adjList[curr_vertex].end(); it++)
                {
                    sorted.push_back((*it)->id);
                }

                std::sort(sorted.begin(), sorted.end());

                for (int k = 0; k < sorted.size(); k++)
                {
                    curr = sorted[k];
                    // std::cout << sorted[k] << '\n';

                    double dist = distances[curr_vertex] + Double3::Distance(vertices[curr_vertex].position3D, vertices[curr].position3D);

                    // distances[curr] = dist;
                    double filtered_version;

                    if (all_depths[curr] == -1)
                    {
                        all_depths[curr] = all_depths[curr_vertex] + 1;
                    }

                    if (distances[curr] == INFINITY)
                    {
                        distances[curr] = dist;

                        if (type == FILTER_GAUSSIAN)
                        {
                            filtered_version = std::exp(-(pow(dist, 2) / pow(alpha, 2)));
                        }

                        if (type == FILTER_BOX)
                        {

                            if (-alpha <= dist and dist <= alpha)
                            {
                                filtered_version = 1;
                            }

                            else
                            {
                                filtered_version = 0;
                            }
                        }

                        filter_coef[curr] = filtered_version;
                    }

                    count++;
                    my_min_heap.Add(curr, count);
                }
            }
        }

        for (int j = 0; j < vertices.size(); j++)
        {
            if (all_depths[j] < maxDepth + 1 and all_depths[j] != -1)
            {

                Color filtered_color;

                filtered_color.r = (unsigned char)(((double)color.r) * filter_coef[j]);
                filtered_color.g = (unsigned char)(((double)color.g) * filter_coef[j]);
                filtered_color.b = (unsigned char)(((double)color.b) * filter_coef[j]);

                outputColorAllVertex[j] = filtered_color;
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color> &outputColorAllVertex,
                                      int vertexId, const Color &color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{

    if (vertexId >= vertices.size() or vertexId < 0)
    {
        // If given vertexId is out of range (e.g it's not in the graph...)

        outputColorAllVertex.resize(0);
    }

    else
    {
        std::vector<int> all_depths(vertices.size(), -1);
        std::vector<bool> visited(vertices.size(), false);

        // Made everything color black first

        all_depths[vertexId] = 0;

        outputColorAllVertex.resize(0);

        Color black;
        black.r = 0;
        black.b = 0;
        black.g = 0;

        for (int i = 0; i < vertices.size(); i++)
        {
            outputColorAllVertex.push_back(black);
        }

        std::vector<double> filter_coef(vertices.size(), -1);
        ;

        double first_one_filtered;

        std::vector<int> valid_vertice_ids;
        valid_vertice_ids.push_back(vertexId);

        if (type == FILTER_GAUSSIAN)
        {
            first_one_filtered = std::exp(-(pow(0.0, 2) / pow(alpha, 2)));
        }

        if (type == FILTER_BOX)
        {

            if (-alpha <= 0.0 and 0.0 <= alpha)
            {
                first_one_filtered = 1;
            }

            else
            {
                first_one_filtered = 0;
            }
        }

        filter_coef[vertexId] = first_one_filtered;

        int count = 0; // This is for the que impl.

        BinaryHeap my_min_heap;
        my_min_heap.Add(vertexId, count);

        int depth = 0;
        int base = vertexId;

        while (my_min_heap.HeapSize() > 0)
        {

            int curr_vertex; // Unique ID
            double sira;     // Weight

            my_min_heap.PopHeap(curr_vertex, sira);

            if (visited[curr_vertex])
            {
                continue;
            }

            else
            {

                visited[curr_vertex] = true;
                int curr;

                std::vector<int> sorted;

                for (std::list<Vertex *>::const_iterator it = adjList[curr_vertex].begin(); it != adjList[curr_vertex].end(); it++)
                {
                    sorted.push_back((*it)->id);
                }

                std::sort(sorted.begin(), sorted.end());

                for (int k = 0; k < sorted.size(); k++)
                {

                    curr = sorted[k];
                    double dist = Double3::Distance(vertices[base].position3D, vertices[curr].position3D);
                    double filtered_version;

                    if (all_depths[curr] == -1)
                    {
                        all_depths[curr] = all_depths[curr_vertex] + 1;
                    }

                    if (type == FILTER_GAUSSIAN)
                    {
                        filtered_version = std::exp(-(pow(dist, 2) / pow(alpha, 2)));
                    }

                    if (type == FILTER_BOX)
                    {
                        if (-alpha <= dist and dist <= alpha)
                        {
                            filtered_version = 1;
                        }

                        else
                        {
                            filtered_version = 0;
                        }
                    }

                    count++;
                    my_min_heap.Add(curr, count);

                    filter_coef[curr] = filtered_version;
                }
            }
        }

        for (int j = 0; j < vertices.size(); j++)
        {

            if (all_depths[j] < maxDepth + 1 and all_depths[j] != -1)
            {
                Color filtered_color;

                filtered_color.r = (unsigned char)(((double)color.r) * filter_coef[j]);
                filtered_color.g = (unsigned char)(((double)color.g) * filter_coef[j]);
                filtered_color.b = (unsigned char)(((double)color.b) * filter_coef[j]);

                outputColorAllVertex[j] = filtered_color;
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color> &colors,
                                 const std::string &fileName)
{
    std::stringstream s;
    for (int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }

    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color> &colors)
{

    for (int i = 0; i < static_cast<int>(colors.size()); i++)
    {

        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}
