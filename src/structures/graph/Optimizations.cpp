#include "../../../headers/structures/graph/Optimizations.h"
#include <pthread.h>

float Optimizations::calculateDistance(Point point1, Point point2) {
    float dist;
    int hashNum = hashingDuplicateDistances(point1, point2);
    if (hashNum == 0) {
        if (strcmp(this->metrics, "manhattan") == 0) {
            dist = Metrics::manhattanDistance(point1.getCoordinates(),
                                              point2.getCoordinates(),
                                              this->dimensions);
        } else {
            dist = Metrics::euclideanDistance(point1.getCoordinates(),
                                              point2.getCoordinates(),
                                              this->dimensions);
        }
        //put dist in to the hashMap
        DistanceContents newHashNum;
        newHashNum.id = point2.getId();
        // Unlock the mutex
        newHashNum.dist = dist;
        pthread_rwlock_wrlock(&hashMapRwlock); // Lock the mutex
        this->hashMap.insert(point1, newHashNum);
        pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex
    } else {
        pthread_rwlock_rdlock(&pointslock);
        Point hashPoint = this->points.at(hashNum - 1);
        pthread_rwlock_unlock(&pointslock);

        DistanceContents content;

        pthread_rwlock_rdlock(&hashMapRwlock); // Lock the mutex
        this->hashMap.find(hashPoint, content);
        pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex

        dist = content.dist;
    }
    char buffer[100];
    sprintf(buffer, "Distance between %d and %d is %f", point1.getId(), point2.getId(), dist);
    LOG_INFO(buffer);
    return dist;
}

void Optimizations::sampling() {
    srand(static_cast<unsigned>(time(NULL)));
    float p = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.0f - 0.1f)));
    // rest of your code
    int percentageToUse = p * this->K;
    int availableNeighbors = 0;

    for (int i = 0; i < this->numOfPoints; i++) {
        Vector<Neighbors> reversedNeighbors;
        Vector<Neighbors> neighborses;
        this->graph.find(this->points.at(i), neighborses);
        this->reverseNN.find(this->points.at(i), reversedNeighbors);
        if (percentageToUse > neighborses.getSize()) {
            percentageToUse = neighborses.getSize();
        }
        //find the available neighbors to be sampled
        for (int j = 0; j < neighborses.getSize(); j++) {
            if (neighborses.at(j).getHasBeenChoosen() == 0) {
                neighborses.at(j).setHasBeenChoosen(1);
                availableNeighbors++;
                if (availableNeighbors == percentageToUse)
                    break;
            }
        }
    }
}

void Optimizations::initSampling() {
    printf(" INIT SAMPLING\n");
    hasBeenInitialized = true;
    for (int i = 0; i < this->numOfPoints; i++) {
        Vector<Neighbors> reversedNeighbors;
        Vector<Neighbors> neighbors;
        this->points.at(i).setHasBeenChoosen(0);
        this->graph.find(this->points.at(i), neighbors);
        this->reverseNN.find(this->points.at(i), reversedNeighbors);
        for (int j = 0; j < reversedNeighbors.getSize(); j++) {
            reversedNeighbors.at(j).setHasBeenChoosen(0);
            neighbors.at(j).setHasBeenChoosen(0);
        }
    }
}

void Optimizations::setd(float d) {
    this->d = d;
}

float Optimizations::getd() {
    return this->d;
}

int Optimizations::checkDuplicate(Point point1, Point point2, Vector<Neighbors> neighborsList1,
                                  Vector<Neighbors> neighborsList2) {
    if (point1.getId() == point2.getId()) {
        return 1;
    }

    //if extended neighbor exist in the neighbor list
    for (int l = 0; l < neighborsList1.getSize(); l++) {
        if (neighborsList1.at(l).getId() == point2.getId()) {
            return 2;
        }
    }
    for (int l = 0; l < neighborsList2.getSize(); l++) {
        if (neighborsList2.at(l).getId() == point1.getId()) {
            return 3;
        }
    }
    return 0;
}

int Optimizations::incrementalSearch(Neighbors &n1, Neighbors &n2) {
    if ((n1.getFlag() || n2.getFlag()) && (n1.getHasBeenChoosen() == 1 && n2.getHasBeenChoosen() == 1)) {
        return 1;
    }
    return 0;
}

int Optimizations::hashingDuplicateDistances(Point &point1, Point &point2) {
    DistanceContents hashNum;
    pthread_rwlock_rdlock(&hashMapRwlock); // Lock the mutex
    this->hashMap.find(point1, hashNum);
    pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex
    if (hashNum.id == point2.getId()) {
        return point1.getId();
    }
    pthread_rwlock_rdlock(&hashMapRwlock); // Lock the mutex
    this->hashMap.find(point2, hashNum);
    pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex
    if (hashNum.id == point1.getId()) {
        return point2.getId();
    } else {
        return 0;
    }
}

void Optimizations::initFlags() {
    //for every point in the graph
    for (int i = 0; i < this->numOfPoints; i++) {
        Vector<Neighbors> neighborsV;
        //find neighbors of the point
        this->graph.find(this->points.at(i), neighborsV);
        for (int v = 0; v < neighborsV.getSize(); v++) {
            //for every neighbor change the flag

            neighborsV.at(v).setFlag(true);
        }
    }
}

UnorderedMap<Point, Vector<Neighbors>> Optimizations::localJoin(int i, int &count, Vector<Point> &tempPointVector) {
    //save the changes temporary
    UnorderedMap<Point, Vector<Neighbors>> tempGraph;


    //find current point with the neighbor vector of it
    Point currentPoint = this->points.at(i);
    Vector<Neighbors> currentNeighborsList;
    this->graph.find(currentPoint, currentNeighborsList);


    //for every neighbor of the current point
    for (int j = 0; j < this->K; j++) {
        //find neighbor point with the neighbor vector of it
        Point neighborPoint1 = this->points.at(currentNeighborsList.at(j).getId() - 1);
        Vector<Neighbors> neighborsList1;
        this->graph.find(neighborPoint1, neighborsList1);
        neighborsList1.sort();

        //for every second neighbor of the current point
        for (int k = 0; k < this->K; k++) {
            Point neighborPoint2 = this->points.at(currentNeighborsList.at(k).getId() - 1);
            Vector<Neighbors> neighborsList2;
            this->graph.find(neighborPoint2, neighborsList2);

            //find max distance of neighbors
            neighborsList1.sort();
            float maxDistance1 = neighborsList1.at(this->K - 1).getDistance();

            neighborsList2.sort();
            float maxDistance2 = neighborsList2.at(this->K - 1).getDistance();

            if (checkDuplicate(neighborPoint1, neighborPoint2, neighborsList1, neighborsList2)) {
                continue;
            }

            //if neighbor1 or neighbor2 is new in the local join
            if (incrementalSearch(currentNeighborsList.at(j), currentNeighborsList.at(k))) {
                // //change the flag of neighbor1 and neighbor2 to 2 as will be calculated in local join
                // currentNeighborsList.at(j).setHasBeenChoosen(2);
                // currentNeighborsList.at(k).setHasBeenChoosen(2);
                //calculate distance between neighbor1 and neighbor2
                float dist = calculateDistance(neighborPoint1, neighborPoint2);

                //put neighbor point 2 to point 1
                if (dist < maxDistance1) {
                    //and remove max neighbor of point1
                    removeReverseNeighbor(neighborsList1.at(this->K - 1), neighborPoint1);

                    Neighbors tempNeighbor(neighborPoint2.getId(), dist, neighborPoint2.getCoordinates());
                    tempNeighbor.setFlag(false);
                    tempNeighbor.setHasBeenChoosen(0);
                    neighborsList1.at(this->K - 1) = tempNeighbor;
                    neighborsList1.sort();
                    tempGraph.insert(neighborPoint1, neighborsList1);
                    tempPointVector.push_back(neighborPoint1);
                    count++;

                    //put reverse point1 to reverse point2
                    putReverseNeighbor(tempNeighbor, currentPoint);
                }
                //put neighbor point 1 to point 2
                if (dist < maxDistance2) {
                    //and remove max neighbor of point2
                    removeReverseNeighbor(neighborsList2.at(this->K - 1), neighborPoint2);

                    Neighbors tempNeighbor(neighborPoint1.getId(), dist, neighborPoint1.getCoordinates());
                    tempNeighbor.setFlag(false);
                    tempNeighbor.setHasBeenChoosen(0);
                    neighborsList2.at(this->K - 1) = tempNeighbor;
                    neighborsList2.sort();
                    tempGraph.insert(neighborPoint2, neighborsList2);
                    tempPointVector.push_back(neighborPoint2);
                    count++;

                    //put reverse point2 to reverse point1
                    putReverseNeighbor(tempNeighbor, currentPoint);
                }
            }
        }
    }
    return tempGraph;
}

int Optimizations::KNN() {
    int repeatFlag = 0;

    //for every point in the graph
    for (int i = 0; i < this->numOfPoints; i++) {
        sampling();
        int count = 0;
        //save the points that will be inserted in the graph
        Vector<Point> tempPointVector;

        UnorderedMap<Point, Vector<Neighbors>> tempGraph = localJoin(i, count, tempPointVector);


        //for every new neighbor vector of points
        for (int p = 0; p < count; p++) {
            Vector<Neighbors> neighborsV;
            tempGraph.find(tempPointVector.at(p), neighborsV);
            //replace the Point with the new neighbor vector
            this->graph.insert(tempPointVector.at(p), neighborsV);
            repeatFlag = 1;
        }
        //early termination
        if (count < static_cast<int>(d * this->K)) {
            printf("Early termination\n");
            return 0;
        }
    }
    return repeatFlag;
}

void Optimizations::findKNearestNeighborsForPoint(const Point &queryPoint) {
    srand(time(NULL));
    Vector<Neighbors> uniqueNumbers;
    int randomNumber;
    int flag;
    //generate K random neighbors for query point
    for (int i = 0; i < this->K; i++) {
        flag = 1;
        while (flag) {
            flag = 0;
            randomNumber = (rand() % this->numOfPoints) + 1;
            for (int j = 0; j < i; j++) {
                if (uniqueNumbers.at(j).getId() == randomNumber) {
                    flag = 1;
                    break;
                }
            }
        }

        //convert point to neighbor
        Point neighborPoint = this->points.at(randomNumber - 1);
        float dist;
        if (strcmp(this->metrics, "manhattan") == 0) {
            dist = Metrics::manhattanDistance(neighborPoint.getCoordinates(), queryPoint.getCoordinates(),
                                              this->dimensions);
        } else {
            dist = Metrics::euclideanDistance(neighborPoint.getCoordinates(), queryPoint.getCoordinates(),
                                              this->dimensions);
        }

        Neighbors neighbor(neighborPoint.getId(), dist, neighborPoint.getCoordinates());
        neighbor.setFlag(true);
        uniqueNumbers.push_back(neighbor);
    }
    //insert query point to the graph
    this->graph.insert(queryPoint, uniqueNumbers);
    this->points.push_back(queryPoint);
    this->numOfPoints++;

    sortKNeighbors();
    while (!KNN());
    //    sortKNeighbors();


    //    //remove query point from the graph
    //    printNeighbors(queryPoint.getId());
    //    this->graph.remove(queryPoint);
    //    this->points.remove(queryPoint);
    //    this->numOfPoints--;
}

void Optimizations::initReverseNN() {
    //for every point in the graph
    for (int i = 0; i < this->numOfPoints; i++) {
        Vector<Neighbors> neighborsV;
        //        //find neighbors of the point
        this->graph.find(this->points.at(i), neighborsV);
        for (int v = 0; v < neighborsV.getSize(); v++) {
            putReverseNeighbor(neighborsV.at(v), this->points.at(i));
        }
    }
}

void Optimizations::removeReverseNeighbor(const Neighbors &neighbor, const Point &point) {
    //convert neighbor to point
    Point reversePoint = this->points.at(neighbor.getId() - 1);

    Vector<Neighbors> tempNeighborsV;
    this->reverseNN.find(reversePoint, tempNeighborsV);

    //convert point to neighbor
    Neighbors reverseNeighbor(point.getId(), neighbor.getDistance(), point.getCoordinates());
    tempNeighborsV.remove(reverseNeighbor);

    this->reverseNN.insert(reversePoint, tempNeighborsV);
}

void Optimizations::putReverseNeighbor(const Neighbors &neighbor, const Point &point) {
    //convert neighbor to point
    Point reversePoint = this->points.at(neighbor.getId() - 1);

    Vector<Neighbors> tempNeighborsV;
    if (!this->reverseNN.find(reversePoint, tempNeighborsV)) {
        this->reverseNN.insert(reversePoint, tempNeighborsV);
    }

    //convert point to neighbor
    Neighbors reverseNeighbor(point.getId(), neighbor.getDistance(), point.getCoordinates());
    tempNeighborsV.push_back(reverseNeighbor);

    this->reverseNN.insert(reversePoint, tempNeighborsV);
}

void Optimizations::printReverseNN(char *outputFile) {
    FILE *file;
    file = fopen(outputFile, "w");
    for (int i = 0; i < this->numOfPoints; ++i) {
        Vector<Neighbors> neighborsVector;
        //print graph
        this->reverseNN.find(this->points.at(i), neighborsVector);
        fprintf(file, "point: %d{\n", i + 1);
        for (int j = 0; j < neighborsVector.getSize(); j++) {
            int neighborId = neighborsVector.at(j).getId();
            float neighborDistance = neighborsVector.at(j).getDistance();
            fprintf(file, "point: %d", neighborId);
            fprintf(file, " distance: %f\n", neighborDistance);
        }
        fprintf(file, "\n}\n");
    }
    fclose(file);
}

void Optimizations::writeHashMap(Point point, DistanceContents content) {
    pthread_rwlock_wrlock(&hashMapRwlock); // Lock the mutex
    char buffer[100];
    sprintf(buffer, "content: %f\n ", content.dist);
    LOG_INFO(buffer);
    this->hashMap.insert(point, content); // Write to the hashMap
    pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex
}

Optimizations::Optimizations() {
    pthread_rwlock_init(&hashMapRwlock, NULL);

}

DistanceContents Optimizations::readHashMap(Point point) {
    DistanceContents contents;
    pthread_rwlock_rdlock(&hashMapRwlock); // Lock the read-write lock for reading
    hashMap.find(point, contents);
    pthread_rwlock_unlock(&hashMapRwlock); // Unlock the read-write lock
    return contents;

}

float Optimizations::calculateNormDistance(Point point1, Point point2) {
    float squareNorm1 = point1.getSquareNorm();
    float squareNorm2 = point2.getSquareNorm();
    float squaredDistance;
    int hashNum = hashingDuplicateDistances(point1, point2);
    if (hashNum == 0) {

        // Calculate the inner product of the two points
        float innerProduct = 0.0;
        for (int i = 0; i < point1.getCoordinates().getSize(); i++) {
            innerProduct += point1.getCoordinates()[i] * point2.getCoordinates()[i];
        }

        // Calculate the squared distance between the two points
        squaredDistance = squareNorm1 + squareNorm2 - 2 * innerProduct;
        DistanceContents newHashNum;
        newHashNum.id = point2.getId();
        // Unlock the mutex
        newHashNum.dist = squaredDistance;
        pthread_rwlock_wrlock(&hashMapRwlock); // Lock the mutex
        this->hashMap.insert(point1, newHashNum);
        pthread_rwlock_unlock(&hashMapRwlock);
    } else {
        pthread_rwlock_rdlock(&pointslock);
        Point hashPoint = this->points.at(hashNum - 1);
        pthread_rwlock_unlock(&pointslock);

        DistanceContents content;

        pthread_rwlock_rdlock(&hashMapRwlock); // Lock the mutex
        this->hashMap.find(hashPoint, content);
        pthread_rwlock_unlock(&hashMapRwlock); // Unlock the mutex

        squaredDistance = content.dist;
    }
    // Return the squared distance
    return squaredDistance;
}
