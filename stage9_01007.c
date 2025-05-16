#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define FAN_COUNT 500
#define CLUSTER_COUNT 3

#define REAL_FAN 0
#define FAKE_FAN 1
#define ANTI_FAN 2

#define MAX_NAME_LEN 20

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int trueType;   // randomly assigned REAL, FAKE, ANTI
    double x;       // feature 1 (e.g., loyalty score)
    double y;       // feature 2 (e.g., activity level)
    int cluster;    // assigned cluster after K-means
} Fan;

typedef struct {
    double x;
    double y;
    int clusterType; // cluster index 0..2 representing REAL/FAKE/ANTI fan group approx
} Centroid;

// Generate random double between min and max
double randDouble(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Calculate Euclidean distance between two points
double distance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx*dx + dy*dy);
}

// Initialize fans with random trueType and random features based on type
void initFans(Fan fans[], int count) {
    for (int i = 0; i < count; i++) {
        fans[i].id = i + 1;
        snprintf(fans[i].name, MAX_NAME_LEN, "Fan%03d", fans[i].id);

        // Randomly assign true type
        fans[i].trueType = rand() % CLUSTER_COUNT;

        // Assign features based on type to make clustering feasible
        switch (fans[i].trueType) {
            case REAL_FAN:
                fans[i].x = randDouble(7.0, 10.0);
                fans[i].y = randDouble(7.0, 10.0);
                break;
            case FAKE_FAN:
                fans[i].x = randDouble(3.0, 6.9);
                fans[i].y = randDouble(3.0, 6.9);
                break;
            case ANTI_FAN:
                fans[i].x = randDouble(0.0, 2.9);
                fans[i].y = randDouble(0.0, 2.9);
                break;
        }
        fans[i].cluster = -1;
    }
}

// Assign each fan to the closest centroid cluster
int assignClusters(Fan fans[], int count, Centroid centroids[]) {
    int changed = 0;
    for (int i = 0; i < count; i++) {
        double minDist = 1e9;
        int bestCluster = -1;
        for (int c = 0; c < CLUSTER_COUNT; c++) {
            double dist = distance(fans[i].x, fans[i].y, centroids[c].x, centroids[c].y);
            if (dist < minDist) {
                minDist = dist;
                bestCluster = c;
            }
        }
        if (fans[i].cluster != bestCluster) {
            fans[i].cluster = bestCluster;
            changed = 1;
        }
    }
    return changed;
}

// Update centroids to mean of assigned fans
void updateCentroids(Fan fans[], int count, Centroid centroids[]) {
    double sumX[CLUSTER_COUNT] = {0};
    double sumY[CLUSTER_COUNT] = {0};
    int countPerCluster[CLUSTER_COUNT] = {0};

    for (int i = 0; i < count; i++) {
        int c = fans[i].cluster;
        if (c >= 0) {
            sumX[c] += fans[i].x;
            sumY[c] += fans[i].y;
            countPerCluster[c]++;
        }
    }
    for (int c = 0; c < CLUSTER_COUNT; c++) {
        if (countPerCluster[c] > 0) {
            centroids[c].x = sumX[c] / countPerCluster[c];
            centroids[c].y = sumY[c] / countPerCluster[c];
        }
    }
}

// Print cluster assignment per fan
void printFanClusters(Fan fans[], int count) {
    printf("Fan Clusters:\n");
    for (int i = 0; i < count; i++) {
        const char *clusterName;
        switch (fans[i].cluster) {
            case REAL_FAN: clusterName = "REAL_FAN"; break;
            case FAKE_FAN: clusterName = "FAKE_FAN"; break;
            case ANTI_FAN: clusterName = "ANTI_FAN"; break;
            default: clusterName = "UNKNOWN"; break;
        }
        printf("ID:%3d %-7s True:%s Cluster:%s\n", fans[i].id, fans[i].name,
            (fans[i].trueType == REAL_FAN ? "REAL_FAN" :
             fans[i].trueType == FAKE_FAN ? "FAKE_FAN" : "ANTI_FAN"),
             clusterName);
    }
}

// Bonus: Print fans grouped by cluster
void printClustersGrouped(Fan fans[], int count) {
    printf("\nFans grouped by cluster:\n");
    for (int c = 0; c < CLUSTER_COUNT; c++) {
        printf("Cluster %d (%s):\n", c,
            (c == REAL_FAN ? "REAL_FAN" : c == FAKE_FAN ? "FAKE_FAN" : "ANTI_FAN"));
        for (int i = 0; i < count; i++) {
            if (fans[i].cluster == c) {
                printf("  ID:%3d %-7s True:%s\n", fans[i].id, fans[i].name,
                    (fans[i].trueType == REAL_FAN ? "REAL_FAN" :
                     fans[i].trueType == FAKE_FAN ? "FAKE_FAN" : "ANTI_FAN"));
            }
        }
    }
}

// classifyFan function - main entry point for the task
void classifyFan(void) {
    srand((unsigned int)time(NULL));

    Fan fans[FAN_COUNT];
    Centroid centroids[CLUSTER_COUNT];

    initFans(fans, FAN_COUNT);

    // Initialize centroids - pick random fans as initial centroids
    for (int c = 0; c < CLUSTER_COUNT; c++) {
        int idx = rand() % FAN_COUNT;
        centroids[c].x = fans[idx].x;
        centroids[c].y = fans[idx].y;
        centroids[c].clusterType = c;
    }

    int changed;
    int iter = 0;
    do {
        changed = assignClusters(fans, FAN_COUNT, centroids);
        updateCentroids(fans, FAN_COUNT, centroids);
        iter++;
    } while (changed && iter < 100);

    printf("K-means converged in %d iterations\n\n", iter);

    printFanClusters(fans, FAN_COUNT);

    printClustersGrouped(fans, FAN_COUNT);
}

int main(void) {
    classifyFan();
    return 0;
}
