#include <iostream>
#include <map>
#include <string>
#include <thread>
//#include <unistd.h>
#include <chrono>
#include <condition_variable>
#include <functional>

// Enum to represent states
enum class LightState {
    Off,
    Blinking,
    Solid
};

// State machine class
class LightSystem {
    using EventHandler = std::function<void()>;

public:
    LightSystem() : currentState(LightState::Off) {}

    // Toggles the state between Off and Blinking
    void handleButtonPress() {
        switch (currentState) {
        case LightState::Off:
            transitionTo(LightState::Blinking);
            break;
        case LightState::Blinking:
            transitionTo(LightState::Off);
            break;
        case LightState::Solid:
            break;
        }
    }

    // Simulate timer timeout in blinking state, transition to Solid
    void handleTimerTimeout() {
        if (currentState == LightState::Blinking) {
            transitionTo(LightState::Solid);
        }
    }

    // Handle the current state
    void update() {
        switch (currentState) {
        case LightState::Off:
            std::cout << "Light: Off" << std::endl;
            break;
        case LightState::Blinking:
            std::cout << "Light: Blinking" << std::endl;
            break;
        case LightState::Solid:
            std::cout << "Light: Solid" << std::endl;
            break;
        }
    }

private:
    LightState currentState;

    void transitionTo(LightState nextState) {
        std::cout << "Transitioning from " << stateToString(currentState)
            << " to " << stateToString(nextState) << std::endl;
        currentState = nextState;
    }

    // Utility function to convert state to string
    std::string stateToString(LightState state) const {
        switch (state) {
        case LightState::Off:
            return "Off";
        case LightState::Blinking:
            return "Blinking";
        case LightState::Solid:
            return "Solid";
        }
    }
};

int main() {
    LightSystem lightSystem;

    std::thread timerThread([&]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate 2-second timer
            lightSystem.handleTimerTimeout();
            lightSystem.update();
        }
        });

    while (true) {
        std::string input;
        std::cout << "Enter 'b' to press button, 'q' to quit: ";
        std::getline(std::cin, input);

        if (input == "b") {
            lightSystem.handleButtonPress();
            lightSystem.update();
        }
        else if (input == "q") {
            break;
        }
        else {
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }

    timerThread.join();
    return 0;
}