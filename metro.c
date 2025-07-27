#include "./Headers/header.h"

int main(int argc, char *argv[]) {
	// Open the CSV file for reading
	const char *fp = argv[1];
	if (fp == NULL)
	{
		printf("Error opening file: %s\n", argv[1]);
		// This is means that file is not opened so exit
		return 1;
	}

	Graph metro;
	createGraph(&metro);

	char stations[MAX_STATIONS][MAX_NAME_LEN];
	// Tis is used to store the station names in the array
	int stationCount = 0;

	// Read station names from csv
	readStationsFromCSV("Csv_Files/station.csv", stations, &stationCount);
	// now station array is update

	metro.numVertices = stationCount;

	// read the csv that contain all the connections
	readCSV(&metro, fp, stations, stationCount);

	int choice;
	do
	{
		printf("\nMenu:\n");
		printf("1. Display stations\n");
		printf("2. Add more stations\n");
		printf("3. Display metro map\n");
		printf("4. Add a new connection between stations\n");
		printf("5. Remove a station\n");
		printf("6. Update the Connection\n");
		printf("7. Get Shortest distance\n");
		printf("8. Get Shortest time\n");
		printf("9. Get Shortest Path\n");
		printf("10. Get the minimum Fare of the journey\n");
		printf("11. Display the travelling History\n");
		// printf("12. Display the map using the GUI\n");
		printf("13. EXIT\n");
		printf("Select an option (1-13): ");
		scanf(" %d", &choice);

		switch (choice)
		{
		case 1:
		{
			displayStations(stations, stationCount);
			break;
		}

		case 2:
		{
			addMoreStations(&metro);
			readStationsFromCSV("Csv_Files/station.csv", stations, &stationCount);
			break;
		}

		case 3:
		{
			displayMap(&metro, stations);
			break;
		}

		case 4:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];
			int distance, time;

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			printf("Enter distance (in km): ");
			scanf("%d", &distance);

			printf("Enter time (in minutes): ");
			scanf("%d", &time);
			addMoreConnections("Csv_Files/delhiMetro.csv", &metro, srcName, destName, distance, time, stations, stationCount);
			break;
		}

		case 5:
		{
			char stationName[MAX_NAME_LEN];
			removeStation(&metro, stationName, stations, &stationCount);
			break;
		}

		case 6:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			updateConnections(srcName, destName, &metro, stations, stationCount);
			break;
		}

		case 7:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			int src = findStationIndex(&metro, srcName, stations, stationCount);
			int dest = findStationIndex(&metro, destName, stations, stationCount);

			int distanceAns = getShortestDistance(src, dest, metro.adjMatrix, metro.numVertices);

			if (distanceAns == INT_MAX)
			{
				printf("No path exists between %s and %s for distance.\n", srcName, destName);
			}
			else
			{
				printf("The Shortest Distance between %s and %s is %d\n", srcName, destName, distanceAns);
			}
			break;
		}

		case 8:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			int src = findStationIndex(&metro, srcName, stations, stationCount);
			int dest = findStationIndex(&metro, destName, stations, stationCount);

			int TimeAns = getShortestTime(src, dest, metro.adjMatrix, metro.numVertices);

			if (TimeAns == INT_MAX)
			{
				printf("No path exists between %s and %s for distance.\n", srcName, destName);
			}
			else
			{
				printf("The Shortest Time between %s and %s is %d\n", srcName, destName, TimeAns);
			}
			break;
		}

		case 9:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			int src = findStationIndex(&metro, srcName, stations, stationCount);
			int dest = findStationIndex(&metro, destName, stations, stationCount);

			Queue q;
			initQueue(&q);

			int pred[MAX_STATIONS];
			int path[MAX_STATIONS];

			getShortestPath(src, dest, metro.adjMatrix, metro.numVertices, pred);

			int pathLength = 0;
			reconstructPath(src, dest, pred, path, &pathLength, &q, stations);

			printPath(&q);
			break;
		}

		case 10:
		{
			char srcName[MAX_NAME_LEN], destName[MAX_NAME_LEN];

			getchar();
			printf("Enter source station name: ");
			fgets(srcName, MAX_NAME_LEN, stdin);
			srcName[strcspn(srcName, "\n")] = '\0';

			printf("Enter destination station name: ");
			fgets(destName, MAX_NAME_LEN, stdin);
			destName[strcspn(destName, "\n")] = '\0';

			int src = findStationIndex(&metro, srcName, stations, stationCount);
			int dest = findStationIndex(&metro, destName, stations, stationCount);

			int distanceAns = getShortestDistance(src, dest, metro.adjMatrix, metro.numVertices);

			double minFare = 4.4 * distanceAns;
			printf("Minimum Fare: ₹%.2f\n", minFare);

			askUserTravelStatus(srcName, destName, minFare);
			break;
		}

		case 11:
		{
			displayHistory("Csv_Files/history.csv");
			break;
		}

		// case 12: {
		// 	runGUI(stations, stationCount, metro.adjMatrix, "Csv_Files/station.csv", "Csv_Files/delhiMetro.csv");
		// 	break;
		// }
		case 12:
		{
			printf("Exiting the system\n");
			return 0;
		}
		}

	} while (choice != 13);

	return 0;
}
