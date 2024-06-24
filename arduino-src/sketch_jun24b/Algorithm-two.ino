#include <Arduino.h>
#include <vector>
#include <map>
#include <set>
#include <limits.h>

using namespace std;

// Pin-Konfiguration
const int SENSOR_PINS[6] = {34, 35, 32, 33, 25, 26};
const int MOTOR_LEFT_FORWARD = 14;
const int MOTOR_LEFT_BACKWARD = 12;
const int MOTOR_RIGHT_FORWARD = 27;
const int MOTOR_RIGHT_BACKWARD = 26;

struct Node {
    int x, y;
    bool operator<(const Node& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }
};

struct Edge {
    Node from, to;
    int weight;
};

class MazeRobot {
public:
    MazeRobot();
    void run();

private:
    Node current_position;
    Node start_position;
    Node end_position;
    map<Node, vector<Edge>> graph;
    set<Node> visited_nodes;

    void follow_line();
    bool at_node();
    void handle_node();
    vector<Edge> find_possible_paths(Node node);
    bool at_end();
    void move_forward();
    void turn_left();
    void turn_right();
    void stop_motors();
    void move_forward_for(int duration);
    void move_backward_for(int duration);
    int read_sensors();
    void calculate_shortest_path();
    void return_to_start();
    void follow_shortest_path();
};

MazeRobot::MazeRobot() : current_position{0, 0}, start_position{0, 0}, end_position{INT_MAX, INT_MAX} {}

void MazeRobot::run() {
    follow_line();
    calculate_shortest_path();
    return_to_start();
    follow_shortest_path();
}

void MazeRobot::follow_line() {
    while (!at_end()) {
        int sensor_values = read_sensors();
        if (sensor_values == 0b000111) { // Mittel-Sensoren auf Linie
            move_forward();
        } else if (sensor_values & 0b000011) { // Links von der Linie
            turn_left();
        } else if (sensor_values & 0b001100) { // Rechts von der Linie
            turn_right();
        }
        if (at_node()) {
            handle_node();
        }
    }
}

bool MazeRobot::at_node() {
    // Prüfen, ob der Roboter an einem Knotenpunkt ist (mehrere Sensoren aktiv)
    int sensor_values = read_sensors();
    return sensor_values == 0b111111;
}

void MazeRobot::handle_node() {
    Node current_node = current_position;
    if (visited_nodes.find(current_node) == visited_nodes.end()) {
        visited_nodes.insert(current_node);
        graph[current_node] = find_possible_paths(current_node);
    }
}

vector<Edge> MazeRobot::find_possible_paths(Node node) {
    vector<Edge> paths;
    // Implementiere Logik, um mögliche Wege von einem Knotenpunkt zu finden
    return paths;
}

bool MazeRobot::at_end() {
    return current_position == end_position;
}

void MazeRobot::move_forward() {
    digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void MazeRobot::turn_left() {
    digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void MazeRobot::turn_right() {
    digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void MazeRobot::stop_motors() {
    digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void MazeRobot::move_forward_for(int duration) {
    move_forward();
    delay(duration);
    stop_motors();
}

void MazeRobot::move_backward_for(int duration) {
    digitalWrite(MOTOR_LEFT_FORWARD, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
    digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
    delay(duration);
    stop_motors();
}

int MazeRobot::read_sensors() {
    int sensor_values = 0;
    for (int i = 0; i < 6; i++) {
        sensor_values |= (digitalRead(SENSOR_PINS[i]) << i);
    }
    return sensor_values;
}

void MazeRobot::calculate_shortest_path() {
    // Implementiere Dijkstra oder A* Algorithmus, um den kürzesten Weg zu berechnen
}

void MazeRobot::return_to_start() {
    // Implementiere Logik, um den Roboter zum Startpunkt zurückzubringen
}

void MazeRobot::follow_shortest_path() {
    // Implementiere Logik, um dem kürzesten Weg zu folgen
}

void setup() {
    // Setup Code für Sensoren und Motoren
    for (int i = 0; i < 6; i++) {
        pinMode(SENSOR_PINS[i], INPUT);
    }
    pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
    pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
    pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
    pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
    
    MazeRobot robot;
    robot.run();
}

void loop() {
    // Hauptschleife bleibt leer, da der Algorithmus in der setup-Funktion gestartet wird
}
