/*
 * Define classes
 */
class CrossCoords {
  public:
    int tag = 0;
    int xCoords = 0;
    int yCoords = 0;
};

class CrossWay {
  public:
    CrossCoords From;
    CrossCoords To;
    int waySteps = 0;
};

/*
 * Define variables
 */
CrossWay **crossWays = nullptr;
uint8_t numCrossWays = 0;

int currentXCoords = 0;
int currentYCoords = 0;

CrossCoords latestCross;

int stepsToLatestCross = 0;

bool movementIsVertical = false; // false: horizontal, true: vertical
bool isEndDiscovered = false

void setup() {
  // Initialize serial communication and other setup code
  Serial.begin(9600);
  // Any other setup initialization
}

void loop() {
  // Main loop where you can call readMaze and driveMaze functions
  readMaze();
  driveMaze();
}

void readMaze() {
  do{
    while(!(detectEnd() && isEndDiscovered)){
      driveThroughMaze()
    }

    if(!detectStart()){
      driveBackToStart()
    }

  }while(!(detectStart() && isEndDiscovered))
}

void driveThroughMaze(){
  // If cross detected through sensors, add Cross to array
      if (detectCross()) {  // Placeholder for actual sensor detection logic
        CrossWay *newCrossWay = new CrossWay();

        // Initialize newCrossWay with actual detected data
        newCrossWay->From = latestCross;
        newCrossWay->To.tag = getTag(true);
        newCrossWay->To.xCoords = currentXCoords;
        newCrossWay->To.yCoords = currentYCoords;
        newCrossWay->waySteps = stepsToLatestCross;

        // Add newCrossWay to the array
        addCrossWay(newCrossWay);

        // Update latestCross
        latestCross = newCrossWay->To;

        // Reset stepsToLatestCross
        stepsToLatestCross = 0;
      } else {
        // Step forward and count the steps by increasing the current X/Y Coordinate
        if (movementIsVertical) {
          updateCoords('y');
        } else {
          updateCoords('x');
        }

        stepsToLatestCross++;
      }
}

CrossWay* calculateWay() {
  // Logic for calculating the best way out of ways array
  // Example: shortest way to the end
  CrossWay *shortestWay = nullptr;
  int minSteps = INT_MAX;

  for (int i = 0; i < numCrossWays; i++) {
    if (crossWays[i]->waySteps < minSteps) {
      minSteps = crossWays[i]->waySteps;
      shortestWay = crossWays[i];
    }
  }

  return shortestWay;
}

void driveMaze() {
  // Logic for driving the maze
  // Example: follow path based on crossWays array
  CrossWay* bestWay = calculateWay();

  if (bestWay != nullptr) {
    // Navigate through the maze following the best way
    Serial.print("Following the best way from (");
    Serial.print(bestWay->From.xCoords);
    Serial.print(", ");
    Serial.print(bestWay->From.yCoords);
    Serial.print(") to (");
    Serial.print(bestWay->To.xCoords);
    Serial.print(", ");
    Serial.print(bestWay->To.yCoords);
    Serial.println(")");
    
    // Add logic to actually move the robot along the best way
  }
}

// Placeholder for actual sensor detection function
bool detectCross() {
  // Add actual sensor detection logic
  // Return true if cross detected, else return false
  return false;  // Example placeholder
}

bool detectStart(){

}

bool detectEnd(){

}

// Function to get the next tag value
int getTag(bool isNewTag = true) {
  if (isNewTag) {
    int maxTag = 0;

    // Iterate through crossWays to find the highest tag value
    for (int i = 0; i < numCrossWays; i++) {
      if (crossWays[i]->From.tag > maxTag) {
        maxTag = crossWays[i]->From.tag;
      }
      if (crossWays[i]->To.tag > maxTag) {
        maxTag = crossWays[i]->To.tag;
      }
    }

    // Return the highest tag + 1
    return maxTag + 1;
  } else {
    return latestCross->To.tag;
  }
}

// Update current coordinates based on the movement
void updateCoords(char axis) {
  if (axis == 'x') {
    currentXCoords += 1; // Example update for x-axis
  } else if (axis == 'y') {
    currentYCoords += 1; // Example update for y-axis
  }
}

// Add new CrossWay to the array
void addCrossWay(CrossWay *newCrossWay) {
  // Create a new array with the new size
  CrossWay **tempArray = new CrossWay*[numCrossWays + 1];
  
  // Copy old array elements to the new array
  for (int i = 0; i < numCrossWays; i++) {
    tempArray[i] = crossWays[i];
  }

  // Add the new CrossWay to the new array
  tempArray[numCrossWays] = newCrossWay;

  // Update crossWays to point to the new array
  delete[] crossWays;
  crossWays = tempArray;

  // Update the number of crossWays
  numCrossWays++;
}

int calculateWaySteps() {
  // Add logic to calculate the number of steps
  return stepsToLatestCross; // Return the steps since the last cross
}
