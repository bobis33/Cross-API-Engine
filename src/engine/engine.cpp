#include "CAE/Engine/Engine.hpp"
#include "CAE/Common.hpp"

cae::Engine::Engine(const std::function<std::shared_ptr<IAudio>()> &audioFactory,
                    const std::function<std::shared_ptr<IInput>()> &inputFactory,
                    const std::function<std::shared_ptr<INetwork>()> &networkFactory,
                    const std::function<std::shared_ptr<IRenderer>()> &rendererFactory,
                    const std::function<std::shared_ptr<IWindow>()> &windowFactory)
    : m_audioPlugin(audioFactory()), m_inputPlugin(inputFactory()), m_networkPlugin(networkFactory()),
      m_rendererPlugin(rendererFactory()), m_windowPlugin(windowFactory()), m_clock(std::make_unique<utl::Clock>())
{
    m_windowPlugin->create(Window::NAME, {.width = Window::WIDTH, .height = Window::HEIGHT});
}

void cae::Engine::run() const
{
    while (!m_windowPlugin->shouldClose())
    {
        m_windowPlugin->pollEvents();
    }
}

void cae::Engine::stop()
{
    m_windowPlugin->close();

    m_audioPlugin = nullptr;
    m_inputPlugin = nullptr;
    m_networkPlugin = nullptr;
    m_rendererPlugin = nullptr;
    m_windowPlugin = nullptr;
}
