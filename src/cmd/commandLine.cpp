#include <iostream>

#include "CAE/Cmd/CommandLine.hpp"

void cae::CommandLine::run()
{
    m_thread = std::thread(
        [this]()
        {
            std::string line;
            while (m_running)
            {
                if (!std::getline(std::cin, line))
                    break;

                if (line == "quit")
                {
                    m_running = false;
                }
                else if (line == "restart")
                {
                    m_engine->restart();
                }
                else if (line == "coucou")
                {
                    std::cout << "coucou toi !\n";
                }
            }
        });
}

void cae::CommandLine::stop()
{
    m_running = false;
    if (m_thread.joinable())
        m_thread.join();
}