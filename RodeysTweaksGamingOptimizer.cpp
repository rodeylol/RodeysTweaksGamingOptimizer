#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <functional>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstdlib>
#include <ctime>

// ===============================
// GameOptimizer Class
// ===============================
class GameOptimizer {
private:
    struct Setting {
        std::string name;
        int value;
        int minValue;
        int maxValue;

        Setting(std::string n, int v, int minV, int maxV)
            : name(n), value(v), minValue(minV), maxValue(maxV) {}
    };

    std::vector<Setting> settings;

public:
    void addSetting(const std::string& name, int defaultValue, int minValue, int maxValue) {
        settings.emplace_back(name, defaultValue, minValue, maxValue);
    }

    void optimizeSettings(int targetPerformance) {
        for (auto& setting : settings) {
            setting.value = std::min(setting.maxValue, std::max(setting.minValue, targetPerformance / 10));
            std::cout << "Optimized " << setting.name << " to " << setting.value << "\n";
        }
    }

    void printSettings() const {
        std::cout << "Current Settings:\n";
        for (const auto& setting : settings) {
            std::cout << "- " << setting.name << ": " << setting.value << "\n";
        }
    }

    std::vector<Setting> getSettings() const { return settings; }
    void updateSetting(const std::string& name, int value) {
        for (auto& setting : settings) {
            if (setting.name == name) {
                setting.value = std::min(setting.maxValue, std::max(setting.minValue, value));
                std::cout << "Updated " << setting.name << " to " << setting.value << "\n";
            }
        }
    }
};

// ===============================
// GameTweaker Class
// ===============================
class GameTweaker {
private:
    std::map<std::string, std::function<void()>> tweaks;

public:
    void addTweak(const std::string& name, std::function<void()> tweakFunction) {
        tweaks[name] = tweakFunction;
    }

    void applyTweak(const std::string& name) {
        if (tweaks.find(name) != tweaks.end()) {
            std::cout << "Applying tweak: " << name << "\n";
            tweaks[name]();
        } else {
            std::cout << "Tweak not found: " << name << "\n";
        }
    }

    void listTweaks() const {
        std::cout << "Available Tweaks:\n";
        for (const auto& tweak : tweaks) {
            std::cout << "- " << tweak.first << "\n";
        }
    }
};

// ===============================
// SettingsManager Class
// ===============================
class SettingsManager {
private:
    std::string filePath;

public:
    SettingsManager(const std::string& file) : filePath(file) {}

    void saveSettings(const GameOptimizer& optimizer) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for saving: " << filePath << "\n";
            return;
        }

        file << "Game Settings:\n";
        for (const auto& setting : optimizer.getSettings()) {
            file << setting.name << "=" << setting.value << "\n";
        }

        std::cout << "Settings saved to " << filePath << "\n";
    }

    void loadSettings(GameOptimizer& optimizer) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for loading: " << filePath << "\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string name;
            int value;

            if (std::getline(ss, name, '=') && (ss >> value)) {
                optimizer.updateSetting(name, value);
            }
        }

        std::cout << "Settings loaded from " << filePath << "\n";
    }
};

// ===============================
// PerformanceProfiler Class
// ===============================
class PerformanceProfiler {
private:
    int fps;
    int cpuUsage;
    int gpuUsage;

public:
    PerformanceProfiler() : fps(0), cpuUsage(0), gpuUsage(0) {}

    void analyzePerformance() {
        fps = 60 + (std::rand() % 21 - 10); // Random FPS between 50 and 70
        cpuUsage = 40 + (std::rand() % 21); // Random CPU usage between 40% and 60%
        gpuUsage = 50 + (std::rand() % 21); // Random GPU usage between 50% and 70%

        std::cout << "Performance Analysis:\n";
        std::cout << "- FPS: " << fps << "\n";
        std::cout << "- CPU Usage: " << cpuUsage << "%\n";
        std::cout << "- GPU Usage: " << gpuUsage << "%\n";
    }

    int getFPS() const { return fps; }
    int getCPUUsage() const { return cpuUsage; }
    int getGPUUsage() const { return gpuUsage; }
};

// ===============================
// Helper Function to Initialize Tweaks
// ===============================
void initializeTweaks(GameTweaker& tweaker) {
    tweaker.addTweak("Boost FPS", []() {
        std::cout << "Reducing shadow quality and texture resolution for higher FPS.\n";
    });

    tweaker.addTweak("Enhance Graphics", []() {
        std::cout << "Increasing shadow quality and texture resolution for better visuals.\n";
    });

    tweaker.addTweak("Reduce Input Lag", []() {
        std::cout << "Disabling V-Sync to reduce input lag.\n";
    });
}

// ===============================
// Main Function
// ===============================
int main() {
    std::srand(std::time(nullptr)); // Seed random number generator

    GameOptimizer optimizer;
    GameTweaker tweaker;
    PerformanceProfiler profiler;
    SettingsManager settingsManager("settings.txt");

    // Initialize settings and tweaks
    optimizer.addSetting("Resolution", 1080, 720, 2160);
    optimizer.addSetting("Texture Quality", 3, 1, 5);
    optimizer.addSetting("Shadow Quality", 2, 1, 4);
    initializeTweaks(tweaker);

    // Load settings from file
    settingsManager.loadSettings(optimizer);

    // Analyze performance and optimize settings
    profiler.analyzePerformance();
    int targetPerformance = profiler.getFPS() > 55 ? 60 : 50;
    optimizer.optimizeSettings(targetPerformance);

    // Save settings to file
    settingsManager.saveSettings(optimizer);

    // Display settings
    optimizer.printSettings();

    // List and apply tweaks
    tweaker.listTweaks();
    tweaker.applyTweak("Boost FPS");

    return 0;
}
// ===============================
// Utility Functions for User Input
// ===============================
namespace UserInput {
    // Function to get an integer input from the user
    int getIntInput(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt << " (" << min << "-" << max << "): ";
            std::cin >> value;

            if (std::cin.fail() || value < min || value > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please try again.\n";
            } else {
                return value;
            }
        }
    }

    // Function to get a string input from the user
    std::string getStringInput(const std::string& prompt) {
        std::string value;
        std::cout << prompt << ": ";
        std::cin.ignore(); // Clear any leftover input
        std::getline(std::cin, value);
        return value;
    }
}

// ===============================
// Advanced Performance Profiler
// ===============================
class AdvancedPerformanceProfiler : public PerformanceProfiler {
private:
    std::string gpuModel;
    int availableMemory;

public:
    AdvancedPerformanceProfiler() : gpuModel("Unknown GPU"), availableMemory(0) {}

    void analyzeAdvancedPerformance() {
        // Simulate advanced profiling
        gpuModel = "NVIDIA GeForce RTX 3080"; // Example GPU
        availableMemory = 8192 + (std::rand() % 2049); // Random memory between 8GB and 10GB

        // Call base class performance analysis
        analyzePerformance();

        std::cout << "Advanced Performance Analysis:\n";
        std::cout << "- GPU Model: " << gpuModel << "\n";
        std::cout << "- Available Memory: " << availableMemory << " MB\n";
    }

    std::string getGPUModel() const { return gpuModel; }
    int getAvailableMemory() const { return availableMemory; }
};

// ===============================
// Interactive Menu System
// ===============================
class InteractiveMenu {
private:
    GameOptimizer& optimizer;
    GameTweaker& tweaker;
    AdvancedPerformanceProfiler& profiler;
    SettingsManager& settingsManager;

public:
    InteractiveMenu(GameOptimizer& opt, GameTweaker& twk, AdvancedPerformanceProfiler& prof, SettingsManager& sm)
        : optimizer(opt), tweaker(twk), profiler(prof), settingsManager(sm) {}

    void displayMenu() {
        while (true) {
            std::cout << "\n=== Gaming Optimizer Menu ===\n";
            std::cout << "1. View Current Settings\n";
            std::cout << "2. Optimize Settings\n";
            std::cout << "3. Apply a Tweak\n";
            std::cout << "4. Performance Analysis\n";
            std::cout << "5. Save Settings\n";
            std::cout << "6. Load Settings\n";
            std::cout << "7. Exit\n";

            int choice = UserInput::getIntInput("Choose an option", 1, 7);

            switch (choice) {
            case 1:
                optimizer.printSettings();
                break;
            case 2:
                optimizeSettings();
                break;
            case 3:
                applyTweak();
                break;
            case 4:
                profiler.analyzeAdvancedPerformance();
                break;
            case 5:
                settingsManager.saveSettings(optimizer);
                break;
            case 6:
                settingsManager.loadSettings(optimizer);
                break;
            case 7:
                std::cout << "Exiting program. Goodbye!\n";
                return;
            default:
                std::cout << "Invalid option. Please try again.\n";
            }
        }
    }

private:
    void optimizeSettings() {
        int targetPerformance = UserInput::getIntInput("Enter target performance score", 30, 100);
        optimizer.optimizeSettings(targetPerformance);
    }

    void applyTweak() {
        tweaker.listTweaks();
        std::string tweakName = UserInput::getStringInput("Enter the name of the tweak to apply");
        tweaker.applyTweak(tweakName);
    }
};

// ===============================
// Main Function with Interactive Menu
// ===============================
int main() {
    std::srand(std::time(nullptr)); // Seed random number generator

    GameOptimizer optimizer;
    GameTweaker tweaker;
    AdvancedPerformanceProfiler profiler;
    SettingsManager settingsManager("settings.txt");

    // Initialize settings and tweaks
    optimizer.addSetting("Resolution", 1080, 720, 2160);
    optimizer.addSetting("Texture Quality", 3, 1, 5);
    optimizer.addSetting("Shadow Quality", 2, 1, 4);
    initializeTweaks(tweaker);

    // Create the interactive menu
    InteractiveMenu menu(optimizer, tweaker, profiler, settingsManager);

    // Display the menu
    menu.displayMenu();

    return 0;
}
// ===============================
// Dynamic Configuration Manager
// ===============================
class ConfigManager {
private:
    std::map<std::string, std::string> config;

public:
    // Load configuration from a file
    void loadConfig(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open configuration file: " << filePath << "\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key, value;
            if (std::getline(ss, key, '=') && std::getline(ss, value)) {
                config[key] = value;
                std::cout << "Loaded config: " << key << " = " << value << "\n";
            }
        }
    }

    // Save configuration to a file
    void saveConfig(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open configuration file for saving: " << filePath << "\n";
            return;
        }

        for (const auto& pair : config) {
            file << pair.first << "=" << pair.second << "\n";
        }

        std::cout << "Configuration saved to " << filePath << "\n";
    }

    // Get a configuration value
    std::string getConfig(const std::string& key, const std::string& defaultValue = "") const {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : defaultValue;
    }

    // Set a configuration value
    void setConfig(const std::string& key, const std::string& value) {
        config[key] = value;
        std::cout << "Set config: " << key << " = " << value << "\n";
    }
};

// ===============================
// Real-Time Optimization Tuning
// ===============================
class RealTimeOptimizer {
private:
    GameOptimizer& optimizer;
    AdvancedPerformanceProfiler& profiler;

public:
    RealTimeOptimizer(GameOptimizer& opt, AdvancedPerformanceProfiler& prof)
        : optimizer(opt), profiler(prof) {}

    void monitorAndOptimize() {
        std::cout << "\n=== Real-Time Optimization ===\n";
        while (true) {
            profiler.analyzeAdvancedPerformance();

            int currentFPS = profiler.getFPS();
            if (currentFPS < 50) {
                std::cout << "Low FPS detected (" << currentFPS << "). Adjusting settings...\n";
                optimizer.optimizeSettings(40);
            } else if (currentFPS > 60) {
                std::cout << "High FPS detected (" << currentFPS << "). Enhancing quality...\n";
                optimizer.optimizeSettings(70);
            } else {
                std::cout << "Stable FPS detected (" << currentFPS << "). No adjustments needed.\n";
            }

            optimizer.printSettings();

            // Exit loop if the user wants to stop monitoring
            std::cout << "Press 'q' to stop real-time optimization or any other key to continue...\n";
            char choice;
            std::cin >> choice;
            if (choice == 'q' || choice == 'Q') {
                break;
            }
        }
    }
};

// ===============================
// Integration with Main Program
// ===============================
int main() {
    std::srand(std::time(nullptr)); // Seed random number generator

    GameOptimizer optimizer;
    GameTweaker tweaker;
    AdvancedPerformanceProfiler profiler;
    SettingsManager settingsManager("settings.txt");
    ConfigManager configManager;

    // Load initial configuration
    configManager.loadConfig("config.txt");

    // Initialize settings and tweaks
    optimizer.addSetting("Resolution", 1080, 720, 2160);
    optimizer.addSetting("Texture Quality", 3, 1, 5);
    optimizer.addSetting("Shadow Quality", 2, 1, 4);
    initializeTweaks(tweaker);

    // Load settings from file
    settingsManager.loadSettings(optimizer);

    // Display the menu system
    InteractiveMenu menu(optimizer, tweaker, profiler, settingsManager);
    menu.displayMenu();

    // Real-time optimization
    RealTimeOptimizer realTimeOptimizer(optimizer, profiler);
    std::cout << "Starting real-time optimization...\n";
    realTimeOptimizer.monitorAndOptimize();

    // Save final configuration and settings
    configManager.setConfig("last_run", "successful");
    configManager.saveConfig("config.txt");
    settingsManager.saveSettings(optimizer);

    return 0;
}
#include <fstream>
#include <stdexcept>
#include <ctime>

// ===============================
// Logger Class
// ===============================
class Logger {
private:
    std::ofstream logFile;

public:
    Logger(const std::string& fileName) {
        logFile.open(fileName, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + fileName);
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const std::string& message) {
        std::time_t now = std::time(nullptr);
        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        logFile << "[" << timeBuffer << "] " << message << "\n";
        std::cout << "[" << timeBuffer << "] " << message << "\n"; // Optional: also print to console
    }
};

// ===============================
// Error Handling Utility
// ===============================
namespace ErrorHandler {
    void handleError(const std::string& errorMessage, Logger& logger) {
        logger.log("ERROR: " + errorMessage);
        std::cerr << "ERROR: " << errorMessage << "\n";
    }
}

// ===============================
// Integration with Main Program
// ===============================
int main() {
    std::srand(std::time(nullptr)); // Seed random number generator

    // Initialize core components
    GameOptimizer optimizer;
    GameTweaker tweaker;
    AdvancedPerformanceProfiler profiler;
    SettingsManager settingsManager("settings.txt");
    ConfigManager configManager;

    // Initialize logging
    Logger logger("optimizer.log");

    try {
        logger.log("Starting the Gaming Optimizer program...");

        // Load initial configuration
        configManager.loadConfig("config.txt");
        logger.log("Configuration loaded from config.txt");

        // Initialize settings and tweaks
        optimizer.addSetting("Resolution", 1080, 720, 2160);
        optimizer.addSetting("Texture Quality", 3, 1, 5);
        optimizer.addSetting("Shadow Quality", 2, 1, 4);
        initializeTweaks(tweaker);
        logger.log("Default settings and tweaks initialized");

        // Load settings from file
        settingsManager.loadSettings(optimizer);
        logger.log("Settings loaded from settings.txt");

        // Display the menu system
        InteractiveMenu menu(optimizer, tweaker, profiler, settingsManager);
        menu.displayMenu();

        // Real-time optimization
        RealTimeOptimizer realTimeOptimizer(optimizer, profiler);
        logger.log("Entering real-time optimization mode...");
        realTimeOptimizer.monitorAndOptimize();

        // Save final configuration and settings
        configManager.setConfig("last_run", "successful");
        configManager.saveConfig("config.txt");
        logger.log("Configuration saved to config.txt");

        settingsManager.saveSettings(optimizer);
        logger.log("Settings saved to settings.txt");

        logger.log("Program completed successfully.");
    } catch (const std::exception& ex) {
        ErrorHandler::handleError(ex.what(), logger);
        return EXIT_FAILURE;
    } catch (...) {
        ErrorHandler::handleError("An unknown error occurred.", logger);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <functional>

// ===============================
// Multi-threading Utilities
// ===============================
class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::vector<std::function<void()>> tasks;
    std::mutex tasksMutex;
    bool stopThreads;

    void threadLoop() {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(tasksMutex);
                if (stopThreads && tasks.empty()) break;

                if (!tasks.empty()) {
                    task = std::move(tasks.back());
                    tasks.pop_back();
                }
            }

            if (task) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    ThreadPool(size_t numThreads) : stopThreads(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&ThreadPool::threadLoop, this);
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(tasksMutex);
            stopThreads = true;
        }

        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void addTask(std::function<void()> task) {
        std::unique_lock<std::mutex> lock(tasksMutex);
        tasks.emplace_back(std::move(task));
    }
};

// ===============================
// Benchmarking Utilities
// ===============================
class Benchmark {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        endTime = std::chrono::high_resolution_clock::now();
    }

    double getElapsedTime() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0;
    }

    void printResults(const std::string& operationName) const {
        std::cout << operationName << " completed in " << getElapsedTime() << " seconds.\n";
    }
};

// ===============================
// Parallel Optimization
// ===============================
class ParallelOptimizer {
private:
    GameOptimizer& optimizer;
    ThreadPool threadPool;

public:
    ParallelOptimizer(GameOptimizer& opt, size_t numThreads)
        : optimizer(opt), threadPool(numThreads) {}

    void parallelOptimize(int targetPerformance) {
        Benchmark benchmark;
        benchmark.start();

        for (const auto& setting : optimizer.getSettings()) {
            threadPool.addTask([&]() {
                std::cout << "Optimizing " << setting.name << " in a separate thread.\n";
                optimizer.updateSetting(setting.name, targetPerformance / 10);
            });
        }

        benchmark.stop();
        benchmark.printResults("Parallel Optimization");
    }
};

// ===============================
// Integration with Main Program
// ===============================
int main() {
    std::srand(std::time(nullptr)); // Seed random number generator

    // Initialize core components
    GameOptimizer optimizer;
    GameTweaker tweaker;
    AdvancedPerformanceProfiler profiler;
    SettingsManager settingsManager("settings.txt");
    ConfigManager configManager;
    Logger logger("optimizer.log");

    try {
        logger.log("Starting the Gaming Optimizer program...");

        // Load initial configuration
        configManager.loadConfig("config.txt");
        logger.log("Configuration loaded from config.txt");

        // Initialize settings and tweaks
        optimizer.addSetting("Resolution", 1080, 720, 2160);
        optimizer.addSetting("Texture Quality", 3, 1, 5);
        optimizer.addSetting("Shadow Quality", 2, 1, 4);
        initializeTweaks(tweaker);
        logger.log("Default settings and tweaks initialized");

        // Load settings from file
        settingsManager.loadSettings(optimizer);
        logger.log("Settings loaded from settings.txt");

        // Display the menu system
        InteractiveMenu menu(optimizer, tweaker, profiler, settingsManager);
        menu.displayMenu();

        // Real-time optimization
        RealTimeOptimizer realTimeOptimizer(optimizer, profiler);
        logger.log("Entering real-time optimization mode...");
        realTimeOptimizer.monitorAndOptimize();

        // Parallel optimization
        ParallelOptimizer parallelOptimizer(optimizer, 4); // 4 threads
        std::cout << "Starting parallel optimization...\n";
        parallelOptimizer.parallelOptimize(50);
        logger.log("Parallel optimization completed.");

        // Save final configuration and settings
        configManager.setConfig("last_run", "successful");
        configManager.saveConfig("config.txt");
        logger.log("Configuration saved to config.txt");

        settingsManager.saveSettings(optimizer);
        logger.log("Settings saved to settings.txt");

        logger.log("Program completed successfully.");
    } catch (const std::exception& ex) {
        ErrorHandler::handleError(ex.what(), logger);
        return EXIT_FAILURE;
    } catch (...) {
        ErrorHandler::handleError("An unknown error occurred.", logger);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
