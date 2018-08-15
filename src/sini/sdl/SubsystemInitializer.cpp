// sini::SubsystemInitializer initializes SDL subsystems with SDL_Init() when
// constructed and SDL_Quit() on destruction, so the initializer object
// should be alive as long as the specified subsystems are utilized. It can also
// be used to initialize other subsystems after being constructed.

#include <iostream>
#include <exception>    // For std::terminate

#include "sini/sdl/SubsystemInitializer.hpp"

namespace sini {

namespace {
// Combine SDL flags
inline Uint32 processFlags(std::initializer_list<SubsystemFlags> flags) noexcept
{
    Uint32 temp = 0;
    for (SubsystemFlags flag : flags)
        temp |= static_cast<Uint32>(flag);
    return temp;
}
} // unnamed namespace

// Constructor
// -----------------------------------------------------------------------------
SubsystemInitializer::SubsystemInitializer(std::initializer_list<SubsystemFlags> flags) noexcept
{
    Uint32 SDL_flag = processFlags(flags);
    if (SDL_Init(SDL_flag) != 0) {
        // Error check
        std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
        std::terminate();
    }
}

// Destructor
// -----------------------------------------------------------------------------
SubsystemInitializer::~SubsystemInitializer() noexcept
{
    SDL_Quit();
}

// Functions
// -----------------------------------------------------------------------------
void SubsystemInitializer::initSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept
{
    Uint32 SDL_flag = processFlags(flags);
    if (SDL_InitSubSystem(SDL_flag) != 0) {
        std::cerr << "SDL_InitSubSystem() failed: " << SDL_GetError() << std::endl;
        std::terminate();
    }
}
void SubsystemInitializer::quitSubsystem(std::initializer_list<SubsystemFlags> flags) noexcept
{
    Uint32 SDL_flag = processFlags(flags);
    SDL_QuitSubSystem(SDL_flag);
}

} // namespace sini
