#ifndef CONTROLLER_H
#define CONTROLLER_H


/**
 * @class Controller
 * @brief Application controller managing CLI argument parsing and module dispatching.
 */
class Controller {
public:
    Controller() = default;

    /**
     * @brief Executes the CLI application lifecycle.
     * @param argc Command argument count.
     * @param argv Command argument array.
     * @return int Return status code (0 for success, 1 for usage error).
     */
    int run(int argc, char* argv[]);
};

#endif