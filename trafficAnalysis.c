#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INTERVALS 24  // 24 hours divided into 1-hour intervals

// Structure for each node in the segment tree
typedef struct {
    double avg_speed;
    double avg_congestion;
} SegmentNode;

// Segment tree array
SegmentNode *segmentTree;

// Input data for each 1-hour interval
typedef struct {
    double speed;
    double congestion;
} TrafficData;

TrafficData inputData[INTERVALS];

// Function to merge two nodes (now summing the values)
SegmentNode merge(SegmentNode left, SegmentNode right) {
    SegmentNode result;
    result.avg_speed = left.avg_speed + right.avg_speed;  // Sum the speeds
    result.avg_congestion = left.avg_congestion + right.avg_congestion;  // Sum the congestion levels
    return result;
}

// Function to build the segment tree
void buildTree(int node, int start, int end) {
    if (start == end) {
        // Leaf node
        segmentTree[node].avg_speed = inputData[start].speed;
        segmentTree[node].avg_congestion = inputData[start].congestion;
    } else {
        int mid = (start + end) / 2;
        buildTree(2 * node + 1, start, mid);
        buildTree(2 * node + 2, mid + 1, end);
        segmentTree[node] = merge(segmentTree[2 * node + 1], segmentTree[2 * node + 2]);
    }
}

// Query the segment tree (sums the values within the range)
SegmentNode query(int node, int start, int end, int l, int r) {
    if (r < start || l > end) {
        // Return a neutral node if the range is completely outside the query range
        SegmentNode neutral = {0, 0};
        return neutral;
    }
    if (l <= start && r >= end) {
        // Current segment is completely within the query range
        return segmentTree[node];
    }
    int mid = (start + end) / 2;
    SegmentNode left = query(2 * node + 1, start, mid, l, r);
    SegmentNode right = query(2 * node + 2, mid + 1, end, l, r);
    return merge(left, right);
}

// Update the segment tree
void update(int node, int start, int end, int idx, TrafficData newData) {
    if (start == end) {
        // Update the leaf node
        segmentTree[node].avg_speed = newData.speed;
        segmentTree[node].avg_congestion = newData.congestion;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node + 1, start, mid, idx, newData);
        } else {
            update(2 * node + 2, mid + 1, end, idx, newData);
        }
        // Recompute the current node after update
        segmentTree[node] = merge(segmentTree[2 * node + 1], segmentTree[2 * node + 2]);
    }
}

int main() {
    // Allocate memory for the segment tree
    int treeSize = 2 * pow(2, ceil(log2(INTERVALS))) - 1;
    segmentTree = (SegmentNode *)malloc(treeSize * sizeof(SegmentNode));

    // Sample input data for 24 intervals (you can replace this with actual data)
    for (int i = 0; i < INTERVALS; i++) {
        inputData[i].speed = (rand() % 50) + 20;  // Random speed between 20 and 69 km/h
        inputData[i].congestion = (rand() % 2000) + 500;  // Random congestion between 500 and 2499 vehicles per hour
    }

    buildTree(0, 0, INTERVALS - 1);

    // Query the average data for a range
    int l, r;
    printf("Your Query: ");
    scanf("%d%d", &l, &r);

    // Query the segment tree
    SegmentNode result = query(0, 0, INTERVALS - 1, l, r);

    // Calculate the averages for the query range
    int rangeLength = r - l + 1;
    double averageSpeed = result.avg_speed / rangeLength;
    double averageCongestion = result.avg_congestion / rangeLength;

    // Output the results
    printf("AVERAGE DATA FROM %d HOURS TO %d HOURS:\n", l, r);
    printf("\tSpeed: %.2f kmph\n", averageSpeed);
    printf("\tCongestion: %d vph\n", (int)averageCongestion);

    while (1) {
        int ch, index, newSpeed, newCongestion, newRangeLength;
        printf("UPDATE CHOICES:\n\t1. Update Node\n\t2. Update Query\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch) {
        case 1:
            printf("Node index to be updated: ");
            scanf("%d", &index);
            printf("New Speed and Congestion Level: ");
            scanf("%d%d", &newSpeed, &newCongestion);
            TrafficData newData = {newSpeed, newCongestion};
            update(0, 0, INTERVALS - 1, index, newData);

            // Query again after the update
            result = query(0, 0, INTERVALS - 1, l, r);

            // Calculate the new averages
            averageSpeed = result.avg_speed / rangeLength;
            averageCongestion = result.avg_congestion / rangeLength;

            printf("UPDATED AVERAGE DATA FROM %d HOURS TO %d HOURS:\n", l, r);
            printf("\tSpeed: %.2f kmph\n", averageSpeed);
            printf("\tCongestion: %d vph\n", (int)averageCongestion);
            break;
        case 2:
            printf("New Query: ");
            scanf("%d%d", &l, &r);

            // Query again after the update
            result = query(0, 0, INTERVALS - 1, l, r);
            newRangeLength = r - l + 1;

            // Calculate the new averages
            averageSpeed = result.avg_speed / newRangeLength;
            averageCongestion = result.avg_congestion / newRangeLength;

            printf("UPDATED AVERAGE DATA FROM %d HOURS TO %d HOURS:\n", l, r);
            printf("\tSpeed: %.2f km/h\n", averageSpeed);
            printf("\tCongestion: %d vph\n", (int)averageCongestion);
            break;
        default:
            printf("--------ANALYSIS TERMINATED--------\n");
            free(segmentTree);
            exit(0);
        }
    }

    // Free memory
    free(segmentTree);
    return 0;
}