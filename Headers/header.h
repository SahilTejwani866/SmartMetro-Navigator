#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_STATIONS 75
#define MAX_NAME_LEN 100
#define MAX_LINE_SIZE 256

// Structure to store a pair of distance and time
typedef struct {
    int distance;
    int time;
} EdgePair;

// Graph structure
typedef struct {
    int numVertices;  // Number of stations
    EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS];  // Adjacency matrix storing distance and time
} Graph;


// Structure for the heap
typedef struct {
	int distance;
	int node;
} pqItem;

// Structure for the queue
typedef struct {
	char arr[MAX_STATIONS][MAX_NAME_LEN];
	int front;
	int rear;
}Queue;

void createGraph(Graph *);
void readStationsFromCSV(const char *, char stations[MAX_STATIONS][MAX_NAME_LEN], int *);
void displayStations(char stations[MAX_STATIONS][MAX_NAME_LEN], int);
void addMoreStations(Graph *);
void addStation(Graph *, char *, int, int);
void appendToCSV(const char *, char *, int, int);
int findStationIndex(Graph *, char *, char stations[MAX_STATIONS][MAX_NAME_LEN], int);
bool isNumeric(char *);
void trimSpaces(char *);
void addEdge(Graph *, int, int, int, int);
void readCSV(Graph *, const char *, char stations[MAX_STATIONS][MAX_NAME_LEN], int);
void displayMap(Graph *g, char stations[MAX_STATIONS][MAX_NAME_LEN]);
void appendEdge(const char *, char *, char *, int , int );
void addMoreConnections(const char *, Graph *, char *, char *, int , int , char stations[MAX_STATIONS][MAX_NAME_LEN], int );
void removefromCSV1(const char *, char *);
void removeConnections(const char *, char *);
void removeStation(Graph *, char *, char stations[MAX_STATIONS][MAX_NAME_LEN], int * cnt);
void updateConnectionsForCSV(const char *, char *, char *, int , int );
void updateConnections(char *, char *, Graph *, char stations[MAX_STATIONS][MAX_NAME_LEN], int );
void minHeapify(pqItem *, int , int );
int getShortestDistance(int , int , EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int );
int getShortestTime(int , int , EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int );

void initQueue(Queue *);
int isFull(Queue *);
int isEmpty(Queue *);
void enqueue(Queue *, char *);
void dequeue(Queue *);
const char* peek(Queue *q);

void reconstructPath(int , int , int pred[MAX_STATIONS], int path[MAX_STATIONS], int *, Queue *, char stations[MAX_STATIONS][MAX_NAME_LEN]);
void getShortestPath(int , int , EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int , int pred[MAX_STATIONS]);
void printPath(Queue *);
void insertToHistory(char *, char *, double );
void askUserTravelStatus(char *, char *, double );
void displayHistory(const char *);
void runGUI(char stations[MAX_STATIONS][MAX_NAME_LEN], int stationCount, EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], const char* csvFilePath, const char* connectionsCSV);
