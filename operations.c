#include "./Headers/header.h"

// Function is used to create or init the graph
void createGraph(Graph *g) {
    g->numVertices = 0;
    // Initialize all distances and times to -1 that is not direct connections
    for (int i = 0; i < MAX_STATIONS; i++) {
        for (int j = 0; j < MAX_STATIONS; j++) {
            g->adjMatrix[i][j].distance = -1;
            g->adjMatrix[i][j].time = -1; 
        }
    }
}

// Funciton is used to store the station names from the csv file to the stations array
void readStationsFromCSV(const char *filename, char stations[MAX_STATIONS][MAX_NAME_LEN], int *stationCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }
    char line[MAX_NAME_LEN];
    *stationCount = 0;
    while (fgets(line, sizeof(line), file)) {
    
        // Remove trailing newline character
        line[strcspn(line, "\r\n")] = '\0';
        
        // Use strtok to split the line by comma
        char *token = strtok(line, ",");
        
        if (*stationCount < MAX_STATIONS) {
            strcpy(stations[*stationCount], token);
            (*stationCount)++;
        } else {
            printf("Maximum limit of adding stations is reached.\n");
            break;
        }
    }
    fclose(file);
}


// Function to display all station names in the graph
void displayStations(char stations[MAX_STATIONS][MAX_NAME_LEN], int cnt) {
    printf("\n***********************************************************************\n");

    for (int i = 0; i < cnt; i++) {
    	
        printf("%d. %s\n", i, stations[i]);
    }

    printf("\n***********************************************************************\n");
}


// Function is used to add the station is the csv and graph also
void addMoreStations(Graph *g) {
    	char stationName[MAX_NAME_LEN];
    	int xCoord, yCoord;
    	char choice;
    	do {
        	printf("Enter station name to add: ");
        	scanf("%s", stationName);
        
        	printf("Enter X coordinate: ");
        	scanf("%d", &xCoord);
        
        	printf("Enter Y coordinate: ");
        	scanf("%d", &yCoord);
        
        	addStation(g, stationName, xCoord, yCoord);
        
        	printf("Do you want to add another station? (y/n): ");
        	scanf(" %c", &choice);
    	} while (choice == 'y' || choice == 'Y');
}

void addStation(Graph *g, char *stationName, int xCoord, int yCoord) {
    	if (g->numVertices >= MAX_STATIONS) {
        	printf("Error: Maximum station limit reached!\n");
        	return;
    	}
    	const char *filename = "Csv_Files/station.csv";
    	g->numVertices++;
    	appendToCSV(filename, stationName, xCoord, yCoord);
}

void appendToCSV(const char *filename, char *newStation, int xCoord, int yCoord) {
    	FILE *file = fopen(filename, "a");  // Open in append mode
    	if (!file) {
    	    printf("Error opening file: %s\n", filename);
    	    return;
    	}
    	// Write the new station with coordinates to the file
    	fprintf(file, "%s,%d,%d\n", newStation, xCoord, yCoord);
    	
    	fclose(file);
    	printf("Station '%s' added to the file successfully.\n", newStation);
}

// Function is used to read the csv
void readCSV(Graph *g, const char *filename, char stations[MAX_STATIONS][MAX_NAME_LEN], int stationCnt) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Buffer to store each line
    char line[MAX_LINE_SIZE];
    char *token;

    // Skip header line
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Error reading header line");
        fclose(file);
        return;
    }

    // Read each line from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove trailing newline
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        // Initialize variables
        char srcName[MAX_NAME_LEN] = {0}, destName[MAX_NAME_LEN] = {0};
        int distance = 0, time = 0;

        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(srcName, token, MAX_NAME_LEN - 1);
        }
        
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(destName, token, MAX_NAME_LEN - 1);
        }
        
        token = strtok(NULL, ",");
        if (token != NULL && isNumeric(token)) {
            distance = atoi(token);
        }
        
        token = strtok(NULL, ",");
        if (token != NULL && isNumeric(token)) {
            time = atoi(token);
        }

        // Find station indices
        int srcIndex = findStationIndex(g, srcName, stations, stationCnt);
        if (srcIndex == -1) {
            srcIndex = g->numVertices - 1;
        }

        int destIndex = findStationIndex(g, destName, stations, stationCnt);
        if (destIndex == -1) {
            destIndex = g->numVertices - 1;
        }

        addEdge(g, srcIndex, destIndex, distance, time);
    }
    fclose(file);
}

// Function is used to find the index of the station from the stations array
int findStationIndex(Graph *g, char *name, char stations[MAX_STATIONS][MAX_NAME_LEN], int cnt) {
	trimSpaces(name);
	
	for(int i = 0; i < cnt; i++ ) { 
		char tempName[MAX_NAME_LEN];
		strcpy(tempName, stations[i]);
		trimSpaces(tempName);

		if (strcmp(tempName, name) == 0) {
		    return i;
		}
	}
	
	return -1;
	// element not found
}

// Function is used to trim xtra white spaces
void trimSpaces(char *str) {
    	// Trim leading spaces
    	char *start = str;
    	while (*start == ' ') {
    		start++;
    	}
    
    	// Trim trailing spaces
    	char *end = start + strlen(start) - 1;
    	while (end > start && (*end == ' ' || *end == '\n')) {
    		end--;
    	}
    	*(end + 1) = '\0';  // Null-terminate the trimmed string

    	// Shift the trimmed string back to the original buffer
    	memmove(str, start, strlen(start) + 1);
}


// Function is used to check is Numeric or not
bool isNumeric(char *str) {
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

// Function to add an edge between two stations with distance and time
void addEdge(Graph *g, int src, int dest, int distance, int time) {
    if (src >= g->numVertices || dest >= g->numVertices) {
        printf("Error: Invalid station index!\n");
        return;
    }
    g->adjMatrix[src][dest].distance = distance;
    g->adjMatrix[src][dest].time = time;

    g->adjMatrix[dest][src].distance = distance;
    g->adjMatrix[dest][src].time = time;
}

// Function is used to display the map
void displayMap(Graph *g, char stations[MAX_STATIONS][MAX_NAME_LEN]) {
	printf("Delhi Metro Map (Compact View):\n");

    	for (int i = 0; i < g->numVertices; i++) {
        	printf("%-30s --> ", stations[i]);
        	
        	// Track if any connections exist
        	int hasConnections = 0;

        	for (int j = 0; j < g->numVertices; j++) {
            		if (g->adjMatrix[i][j].distance != -1) {
                	printf("%s (%dkm, %dmin)  ", stations[j],
                       	g->adjMatrix[i][j].distance, g->adjMatrix[i][j].time);
                	hasConnections = 1;  
                	// Found a valid connection3
            	}
        }

	if (!hasConnections) {
	    printf("No direct connections");
	}
	printf("\n");
    }
}

// Function is used to add more connections
void addMoreConnections(const char *filename, Graph *g, char *srcName, char *destName, int dist, int time, char stations[MAX_STATIONS][MAX_NAME_LEN], int cnt) {
	int src = findStationIndex(g, srcName, stations, cnt);
	int dest = findStationIndex(g, destName, stations, cnt);
	
	if (src == -1 || dest == -1) {
        	printf("Error: One or both stations not found!\n");
        	return;
    	}
    	addEdge(g, src, dest, dist, time);
    	appendEdge(filename, srcName, destName, dist, time);
    	
    	
    	printf("New connection between %s and %s is created", srcName, destName);
}

//Function is used to append the edge in the csv file
void appendEdge(const char *filename, char *srcName, char *destName, int distance, int time) {
    // Open the file in append mode
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Append the new edge to the CSV file in the format: src,dest,distance,time
    fprintf(file, "%s,%s,%d,%d\n", srcName, destName, distance, time);

    fclose(file);
}

// Function is remove the station from graph
void removeStation(Graph *g, char *stationName, char stations[MAX_STATIONS][MAX_NAME_LEN], int *cnt) {
	getchar(); // Consume the leftover newLine form previos input
	printf("Enter station name to remove: ");
    	fgets(stationName, MAX_NAME_LEN, stdin);
    	
    	// Remove trailing newline
    	stationName[strcspn(stationName, "\n")] = '\0';
    	  
    	// Trim input name
    	trimSpaces(stationName);  
    	
    	int index = findStationIndex(g, stationName, stations, *cnt);
    	
    	if (index == -1) {
		printf("Error: Station not found!\n");
		return;
	}
	
	// Shift rows up to remove the station from the adjacency matrix
    	for (int i = index; i < g->numVertices - 1; i++) {
        	for (int j = 0; j < g->numVertices; j++) {
        	    g->adjMatrix[i][j] = g->adjMatrix[i + 1][j];
        	}
    	}

    	// Shift columns left to remove the station from the adjacency matrix
    	for (int j = index; j < g->numVertices - 1; j++) {
        	for (int i = 0; i < g->numVertices - 1; i++) {
        	    g->adjMatrix[i][j] = g->adjMatrix[i][j + 1];
        	}
    	}

    	// Shift station names up in the station list
    	for (int i = index; i < ((*cnt) - 1); i++) {
    	    strcpy(stations[i], stations[i + 1]);
    	}

    	// Decrease the total number of stations
    	g->numVertices--;


	// now remove the station from the station csv then call the read csv to update station array, and its count
	removefromCSV1("Csv_Files/station.csv", stationName);
	readStationsFromCSV("Csv_Files/station.csv", stations, cnt);
	
	
	// now remove the station from the delhimetro csv
	removeConnections("Csv_Files/delhiMetro.csv", stationName);
	
    	printf("Station '%s' removed successfully.\n", stationName);
}

// function is used to remove the station form the first csv
void removefromCSV1(const char *filename, char *stationName) {
	FILE *file = fopen(filename, "r");
	if (!file) {
        	printf("Error: Unable to open file %s\n", filename);
        	return;
    	}

    	char tempFilename[] = "temp.csv";
    	FILE *tempFile = fopen(tempFilename, "w");
    	if (!tempFile) {
        	printf("Error: Unable to create temporary file.\n");
        	fclose(file);
        	return;
    	}

    	char line[MAX_LINE_SIZE];
    	int lineFound = 0;

    	while (fgets(line, sizeof(line), file)) {
        	// Remove trailing newline character
        	line[strcspn(line, "\r\n")] = '\0';
	
        	// Use strtok to extract the first token (station name)
        	char *token = strtok(line, ",");
	        char *xCoordToken = strtok(NULL, ",");
	        char *yCoordToken = strtok(NULL, ",");
        	
        	// Skip the line that matches the station name
        	if (token && strcmp(token, stationName) == 0) {
        	    lineFound = 1;
        	    continue;
        	}
	
        	// Write the line to the temporary file
        	fprintf(tempFile, "%s,%s,%s\n", 
		    token ? token : "", 
		    xCoordToken ? xCoordToken : "", 
		    yCoordToken ? yCoordToken : "");
    	}	

    	fclose(file);
    	fclose(tempFile);

    	// Replace original file with the temporary file
    	if (lineFound) {
    	    remove(filename);
    	    rename(tempFilename, filename);
    	    printf("Station '%s' removed successfully from the file.\n", stationName);
    	}
    	else {
    	    remove(tempFilename);
    	    printf("Station '%s' not found in the file.\n", stationName);
    	}
}	
// Function is remove the connections of that particular stations
void removeConnections(const char *filename, char *stationName) {
	FILE *file = fopen(filename, "r");
    	if (!file) {
        	printf("Error opening file: %s\n", filename);
        	return;
    	}

    	FILE *tempFile = fopen("temp.csv", "w");
    	if (!tempFile) {
        	printf("Error creating temporary file.\n");
        	fclose(file);
        	return;
    	}

    	char line[256];
    	int found = 0;

    	while (fgets(line, sizeof(line), file)) {
        	char lineCopy[256];
        	strcpy(lineCopy, line);

        	char *token;
        	char stationA[100], stationB[100];

	        token = strtok(lineCopy, ",");
	        if (token) {
	        	strcpy(stationA, token);
		}
		
	        token = strtok(NULL, ",");
	        if (token) {
	        	strcpy(stationB, token);
		}
	
	        // Check if the station matches
	        if (strcmp(stationA, stationName) != 0 && strcmp(stationB, stationName) != 0) {
	            fprintf(tempFile, "%s", line);
	        }
	        else {
	            found = 1;
	        }
	}
	fclose(file);
	fclose(tempFile);
	
	if (found) {
	        remove(filename);
	        rename("temp.csv", filename);
	        printf("Connections involving station '%s' removed successfully.\n", stationName);
	} else {
	        remove("temp.csv");
	        printf("No connections found involving station '%s'.\n", stationName);
	}	
}

// Function is used to update the connections between the src and dest
void updateConnections(char *srcName, char *destName, Graph *g, char stations[MAX_STATIONS][MAX_NAME_LEN], int cnt) {
	
	// first upadate the connection in the struct then in the csv file
	int srcIndex = findStationIndex(g, srcName, stations, cnt);
    	int destIndex = findStationIndex(g, destName, stations, cnt);

    	if (srcIndex == -1 || destIndex == -1) {
        	printf("One or both stations not found.\n");
        	return;
    	}

    	int newDistance, newTime;
    	printf("Enter the new distance between %s and %s (in km): ", srcName, destName);
    	scanf("%d", &newDistance);

    	printf("Enter the new travel time between %s and %s (in minutes): ", srcName, destName);
    	scanf("%d", &newTime);

    	g->adjMatrix[srcIndex][destIndex].distance = newDistance;
    	g->adjMatrix[srcIndex][destIndex].time = newTime;

    	g->adjMatrix[destIndex][srcIndex].distance = newDistance;
    	g->adjMatrix[destIndex][srcIndex].time = newTime;
    	
    	
	updateConnectionsForCSV("Csv_Files/delhiMetro.csv", srcName, destName, newDistance, newTime);
    	printf("Connection between %s and %s updated successfully.\n", srcName, destName);
}

// Function is used to update the connections in the csv file
void updateConnectionsForCSV(const char *filename, char *srcName, char *destName, int newD, int newT) {
	FILE *file = fopen(filename, "r");
    	if (!file) {
        	printf("Error opening file: %s\n", filename);
        	return;
    	}

    	FILE *tempFile = fopen("temp.csv", "w");
    	if (!tempFile) {
        	printf("Error creating temporary file.\n");
        	fclose(file);
        	return;
    	}

    	char line[256];
    	int updated = 0;

    	while (fgets(line, sizeof(line), file)) {
        	char lineCopy[256];
        	strcpy(lineCopy, line);

        	char *token;
        	char stationA[100], stationB[100];

        	token = strtok(lineCopy, ",");
        	if (token) {
        		strcpy(stationA, token);
		}
		
        	token = strtok(NULL, ",");
        	if (token) {
        		strcpy(stationB, token);
		}
		
        	if ((strcmp(stationA, srcName) == 0 && strcmp(stationB, destName) == 0) ||
        	    (strcmp(stationA, destName) == 0 && strcmp(stationB, srcName) == 0)) {
        		fprintf(tempFile, "%s,%s,%d,%d\n", stationA, stationB, newD, newT);
        	  	updated = 1;
        	}
        	else {
            		fprintf(tempFile, "%s", line);
        	}
    	}

	fclose(file);
    	fclose(tempFile);

    	if (updated) {
        	remove(filename);
        	rename("temp.csv", filename);
        	printf("Connection between '%s' and '%s' updated successfully.\n", srcName, destName);
    	} else {
        	remove("temp.csv");
        	printf("No connection found between '%s' and '%s'.\n", srcName, destName);
    	}
}



//Function is used to minHeapify the contents
void minHeapify(pqItem *pq, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && pq[left].distance < pq[smallest].distance) {
        smallest = left;
    }
    if (right < n && pq[right].distance < pq[smallest].distance) {
        smallest = right;
    }

    if (smallest != i) {
        pqItem temp = pq[i];
        pq[i] = pq[smallest];
        pq[smallest] = temp;
        minHeapify(pq, n, smallest);
    }
}

// Function is used to find teh shortest distance between them
int getShortestDistance(int src, int dest, EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int numVertices) {
	    int dist[MAX_STATIONS];
	    int visited[MAX_STATIONS] = {0};

	    for (int i = 0; i < numVertices; i++) {
	        dist[i] = INT_MAX;
	    }
	    dist[src] = 0;

	    pqItem pq[MAX_STATIONS];
	    int pqSize = 0;

	    pqItem newItem;
	    newItem.distance = 0;
	    newItem.node = src;
	    pq[pqSize++] = newItem;
	
	    while (pqSize > 0) {

	        pqItem top = pq[0];
	        pq[0] = pq[pqSize - 1];
	        pqSize--;
	        minHeapify(pq, pqSize, 0);
	
	        int node = top.node;
	        if (visited[node]) {
	            continue;
	        }
	
	        visited[node] = 1;
	
	        for (int adjNode = 0; adjNode < numVertices; adjNode++) {
	            if (adjMatrix[node][adjNode].distance > 0 && adjMatrix[node][adjNode].distance != INT_MAX && dist[node] != INT_MAX) {
	                int edgeWeight = adjMatrix[node][adjNode].distance;
	
	                if (dist[node] + edgeWeight < dist[adjNode]) {
	                    dist[adjNode] = dist[node] + edgeWeight;
	
	                    pqItem newItem;
	                    newItem.distance = dist[adjNode];
	                    newItem.node = adjNode;
	                    pq[pqSize++] = newItem;

	                    for (int i = (pqSize - 1) / 2; i >= 0; i--) {
	                        minHeapify(pq, pqSize, i);
	                    }
	                }
	            }
	        }
	    }
	
	    return dist[dest];
}

// Function is used to find teh shortest time between them
int getShortestTime(int src, int dest, EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int numVertices) {
	int time[MAX_STATIONS];
	int visited[MAX_STATIONS] = {0};

	    for (int i = 0; i < numVertices; i++) {
	        time[i] = INT_MAX;
	    }
	    time[src] = 0;

	    pqItem pq[MAX_STATIONS];
	    int pqSize = 0;

	    pqItem newItem;
	    newItem.distance = 0;
	    newItem.node = src;
	    pq[pqSize++] = newItem;
	
	    while (pqSize > 0) {

	        pqItem top = pq[0];
	        pq[0] = pq[pqSize - 1];
	        pqSize--;
	        minHeapify(pq, pqSize, 0);
	
	        int node = top.node;
	        if (visited[node]) {
	            continue;
	        }
	
	        visited[node] = 1;
	
	        for (int adjNode = 0; adjNode < numVertices; adjNode++) {
	            if (adjMatrix[node][adjNode].time > 0 && adjMatrix[node][adjNode].time != INT_MAX && time[node] != INT_MAX) {
	                int edgeWeight = adjMatrix[node][adjNode].time;

	                if (time[node] + edgeWeight < time[adjNode]) {
	                    time[adjNode] = time[node] + edgeWeight;
	
	                    pqItem newItem;
	                    newItem.distance = time[adjNode];
	                    newItem.node = adjNode;
	                    pq[pqSize++] = newItem;

	                    for (int i = (pqSize - 1) / 2; i >= 0; i--) {
	                        minHeapify(pq, pqSize, i);
	                    }
	                }
	            }
	        }
	    }
	
	    return time[dest];
}


// Function used to store the shortest path
void getShortestPath(int src, int dest, EdgePair adjMatrix[MAX_STATIONS][MAX_STATIONS], int numVertices, int pred[MAX_STATIONS]) {
	int dist[MAX_STATIONS];
	int visited[MAX_STATIONS] = {0};

	    for (int i = 0; i < numVertices; i++) {
	        dist[i] = INT_MAX;
	    }
	    dist[src] = 0;
	    
	    for(int i =0; i < numVertices; i++) {
	    	pred[i] = -1;
	    }

	    pqItem pq[MAX_STATIONS];
	    int pqSize = 0;

	    pqItem newItem;
	    newItem.distance = 0;
	    newItem.node = src;
	    pq[pqSize++] = newItem;
	
	    while (pqSize > 0) {

	        pqItem top = pq[0];
	        pq[0] = pq[pqSize - 1];
	        pqSize--;
	        minHeapify(pq, pqSize, 0);
	
	        int node = top.node;
	        if (visited[node]) {
	            continue;
	        }
	
	        visited[node] = 1;
	
	        for (int adjNode = 0; adjNode < numVertices; adjNode++) {
	            if (adjMatrix[node][adjNode].distance > 0 && adjMatrix[node][adjNode].distance != INT_MAX && dist[node] != INT_MAX) {
	                int edgeWeight = adjMatrix[node][adjNode].distance;
	
	                // Relaxation step
	                if (dist[node] + edgeWeight < dist[adjNode]) {
	                    dist[adjNode] = dist[node] + edgeWeight;
	                    pred[adjNode] = node;
	
	                    pqItem newItem;
	                    newItem.distance = dist[adjNode];
	                    newItem.node = adjNode;
	                    pq[pqSize++] = newItem;

	                    for (int i = (pqSize - 1) / 2; i >= 0; i--) {
	                        minHeapify(pq, pqSize, i);
	                    }
	                }
	            }
	        }
	    }
}

// Function is used to back the path
void reconstructPath(int src, int dest, int pred[MAX_STATIONS], int path[MAX_STATIONS], int *pathLength, Queue *q, char stations[MAX_STATIONS][MAX_NAME_LEN]) {
	int node = dest;
	*pathLength = 0;
	
	while(node != -1) {
		path[*pathLength] = node;
		(*pathLength)++;
		node = pred[node];
	}
	
	for(int i=0; i < *pathLength/2; i++) {
		int temp = path[i];
		path[i] = path[*pathLength - 1- i];
		path[*pathLength - 1 - i] = temp;
	}
	
	for (int i = 0; i < *pathLength; i++) {
        	enqueue(q, stations[path[i]]);
    	}
}

// Function is used to init the queue
void initQueue(Queue *q) {
	q -> front = 0;
	q -> rear = 0;
}

// Function is used tot check whether queue is full or not
int isFull(Queue *q) {
	return (q -> rear + 1) % MAX_STATIONS == q -> front;
}

// Function is usedto check queue is empty or not
int isEmpty(Queue *q) {
	return q -> front == q -> rear;
}

// Function is used to add the station to the queue
void enqueue(Queue *q, char *stationName) {
	if(isFull(q)) {
		printf("Queue is full! Connot enqueue.\n");
		return;
	}
	
	strcpy(q->arr[q->rear], stationName);
	q -> rear = (q -> rear + 1) % MAX_STATIONS;
}

// Function is used to pop the element from the queue
void dequeue(Queue *q) {
	if(isEmpty(q)) {
		printf("Queue is empty! Cannot dequeue.\n");
		return;
	}
	
	strcpy(q->arr[q->front], "");
	q->front = (q->front + 1) % MAX_STATIONS;
}

//Function is used to find the peek element of the queeu
const char* peek(Queue *q) {
    if (isEmpty(q)) {
        return NULL;
    }
    return q->arr[q->front];
}

// Function is used to print the path
void printPath(Queue *q) {
    if (isEmpty(q)) {
        printf("The path queue is empty.\n");
        return;
    }

    printf("Shortest Path: ");
    int current = q->front;
    while (current != q->rear) {
        printf("%s ", q->arr[current]);
        current = (current + 1) % MAX_STATIONS;
        if (current != q->rear) {
            printf("-> ");
        }
    }
    printf("\n");
}


// Function is used to ask user about there travel status
void askUserTravelStatus(char *src, char *destination, double fare) {
	char response[5];

    	printf("Have you traveled to %s? (yes/no): ", destination);
    	scanf("%s", response);

    	if (strcmp(response, "yes") == 0) {
        	printf("That's great! We hope you enjoyed your journey to %s.\n", destination);
        	
        	// store in the csv file the history
        	insertToHistory(src, destination, fare);
    	}
    	else if (strcmp(response, "no") == 0) {
        	printf("We hope you plan to visit %s soon!\n", destination);
    	}
    	else {
        	printf("Invalid response. Please answer with 'yes' or 'no'.\n");
    	}
    	return;
}

// Function is used to add the history to the csv file
void insertToHistory(char *src, char *destination, double fare) {
	const char *filename = "Csv_Files/history.csv";
    	const char *tempFilename = "temp.csv";
    	FILE *file = fopen(filename, "r");
    
    	if (!file) {
    	    printf("Error opening file: %s\n", filename);
    	    return;
    	}
	
    	FILE *tempFile = fopen(tempFilename, "w");
    	if (!tempFile) {
    	    printf("Error creating temporary file.\n");
    	    fclose(file);
    	    return;
    	}
    
    	// Write the new entry at the top of the temporary file
    	fprintf(tempFile, "%s,%s,%.2f\n", src, destination, fare);
    	
    	// Copy the existing content of the original file to the temporary file
    	char line[MAX_LINE_SIZE];
    	while (fgets(line, MAX_LINE_SIZE, file)) {
    	    fprintf(tempFile, "%s", line);
    	}

    	fclose(file);
    	fclose(tempFile);
    	
    	// Replace the original file with the temporary file
    	if (rename(tempFilename, filename) != 0) {
    	    printf("Error renaming temporary file.\n");
    	    remove(tempFilename);
    	}
    	
    	return;
}

// Function is used to display the history
void displayHistory(const char *file) {
    FILE *fp = fopen(file, "r");

    if (!fp) {
        printf("Error: Unable to open file %s\n", file);
        return;
    }

    char line[MAX_LINE_SIZE];

    printf("\nTravel History:\n");
    printf("%-20s %-20s %-10s\n", "Source Station", "Destination Station", "Fare");
    printf("--------------------------------------------------------------\n");

    while (fgets(line, MAX_LINE_SIZE, fp)) {
    
        char *srcStation = strtok(line, ",");
        char *destStation = strtok(NULL, ",");
        char *fareStr = strtok(NULL, ",");

        if (srcStation && destStation && fareStr) {
            // Convert fare string to float
            float fare = strtof(fareStr, NULL);
            
            // Print the parsed data
            printf("%-20s %-20s ₹%-10.2f\n", srcStation, destStation, fare);
        }
    }

    fclose(fp);
    printf("--------------------------------------------------------------\n");
}
