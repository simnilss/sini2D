// sini::Window is basically a wrapper for SDL_Window, handling a few of the
// details "under the hood" as well as providing a wrapper for SDL_WindowFlags
//
// sini::Window is clearly similar to and has taken inspiration from
// sfz::sdl::Window from sfzCore and SkipIfZeroCommon by Peter Hillerström,
// (https://github.com/SkipIfZero). However, I don't see the point in going
// out of my way to find other solutions when I like the way he's done it. This
// does not necessarily represent the quality of Peter's work.

#include <iostream>
#include <assert.h>
#include <algorithm>    // For std::sort

#include "sini/sdl/Window.hpp"

namespace sini {

// Helper functions
// -----------------------------------------------------------------------------
namespace {
SDL_Window* createWindow(const char* title, vec2i pos, vec2i size, Uint32 flags)
{
    SDL_Window* win_ptr = nullptr;
    win_ptr = SDL_CreateWindow(title, pos.x,
        pos.y, size.x, size.y, flags);
    // Error check
    if (win_ptr == NULL) {
        std::cerr << "Error when creating SDL Window: "
            << SDL_GetError() << std::endl;
        std::terminate();
    }
    return win_ptr;
}

Uint32 processFlags(std::initializer_list<WindowProperties> flags) {
    Uint32 temp = 0;
    for (WindowProperties flag : flags)
        temp |= static_cast<Uint32>(flag);
    return temp;
}
} // unnamed namespace


// Constructors and destructor
// -----------------------------------------------------------------------------
Window::Window(const char* title) noexcept
    : win_ptr(createWindow(title, { SDL_WINDOWPOS_UNDEFINED }, { 640, 480 },
        processFlags({ WindowProperties::OPENGL, WindowProperties::RESIZABLE })))
{}
Window::Window(const char* title, vec2i size) noexcept
    : win_ptr(createWindow(title, { SDL_WINDOWPOS_UNDEFINED }, size,
        processFlags({ WindowProperties::OPENGL, WindowProperties::RESIZABLE })))
{}
Window::Window(const char* title, vec2i size, std::initializer_list<WindowProperties> flags) noexcept
    : win_ptr(createWindow(title, { SDL_WINDOWPOS_UNDEFINED }, size,
        processFlags(flags)))
{}
Window::Window(const char* title, vec2i pos, vec2i size, std::initializer_list<WindowProperties> flags) noexcept
    : win_ptr(createWindow(title, pos, size, processFlags(flags)))
{}

Window::~Window() noexcept
{
    SDL_DestroyWindow(win_ptr);
}


// Functions
// -----------------------------------------------------------------------------
int32_t Window::width() const noexcept
{
    int width;
    SDL_GetWindowSize(win_ptr, &width, nullptr);
    return static_cast<int32_t>(width);
}

int32_t Window::height() const noexcept
{
    int height;
    SDL_GetWindowSize(win_ptr, nullptr, &height);
    return static_cast<int32_t>(height);
}

vec2i Window::dimensions() const noexcept
{
    int width, height;
    SDL_GetWindowSize(win_ptr, &width, &height);
    return vec2i{ static_cast<int32_t>(width), static_cast<int32_t>(height) };
}

int32_t Window::drawWidth() const noexcept
{
    int draw_width;
    SDL_GL_GetDrawableSize(win_ptr, &draw_width, nullptr);
    return static_cast<int32_t>(draw_width);
}

int32_t Window::drawHeight() const noexcept
{
    int draw_height;
    SDL_GL_GetDrawableSize(win_ptr, nullptr, &draw_height);
    return static_cast<int32_t>(draw_height);
}

vec2i Window::drawingDimensions() const noexcept
{
    int draw_width, draw_height;
    SDL_GL_GetDrawableSize(win_ptr, &draw_width, &draw_height);
    return vec2i{ static_cast<int32_t>(draw_width), static_cast<int32_t>(draw_height) };
}

void Window::setSize(int width, int height) noexcept
{
    assert(width > 0);
    assert(height > 0);
    SDL_SetWindowSize(win_ptr, width, height);
}

void Window::setFullscreen(FullscreenMode mode, int display_index) noexcept
{
    Uint32 fullscreen_mode = static_cast<Uint32>(mode);

    if (mode == FullscreenMode::EXCLUSIVE) {
        // Check if the chosen display index is valid
        const int n_displays = SDL_GetNumVideoDisplays();
        if (display_index >= n_displays || display_index < -1) {
            std::cerr << "Invalid display index: " << display_index
                      << ". Using 0 instead." << std::endl;
            display_index = 0;
        }

        // Check what display to use
        // (-1 means the current one, which is the default argument)
        if (display_index == -1) {
            display_index = SDL_GetWindowDisplayIndex(win_ptr);
            if (display_index < 0) {
                std::cerr << "SDL_GetWindowDisplayIndex() failed: "
                          << SDL_GetError() << std::endl;
                std::cerr << "Using display 0 instead" << std::endl;
                display_index = 0;
            }
        }

        // Set the display
        SDL_DisplayMode display_mode;
        if (SDL_GetDesktopDisplayMode(display_index, &display_mode) != 0) {
            std::cerr << "SDL_GetDesktopDisplayMode() failed: "
                      << SDL_GetError() << std::endl;
        }
        if (SDL_SetWindowDisplayMode(win_ptr, &display_mode) != 0) {
            std::cerr << "SDL_SetDisplayMode() failed: "
                      << SDL_GetError() << std::endl;
        }
    }

    if (SDL_SetWindowFullscreen(win_ptr, fullscreen_mode) != 0) {
        std::cerr << "SDL_SetWindowFullscreen() failed: "
            << SDL_GetError() << std::endl;
    }
}

void Window::setVSync(VSync mode) noexcept
{
    int vsync_interval = static_cast<int>(mode);
    if (SDL_GL_SetSwapInterval(vsync_interval) != 0) {
        // Error check
        std::cerr << "SDL_GL_SetSwapInterval() failed: "
            << SDL_GetError() << std::endl;
    }
}



// Other functions
// -----------------------------------------------------------------------------
std::vector<vec2i> getAvailableResolutions() noexcept
{
    // Check all available displays
    const int n_displays = SDL_GetNumVideoDisplays();
    if (n_displays < 1) { // Error check
        std::cerr << "SDL_GetNumVideoDisplays() failed: "
            << SDL_GetError() << std::endl;
    }

    std::vector<vec2i> resolutions;
    SDL_DisplayMode display_mode;
    for (int display_idx = 0; display_idx < n_displays; display_idx++) {

        int n_display_modes = SDL_GetNumDisplayModes(display_idx);
        if (n_display_modes < 1) { // Error check
            std::cerr << "SDL_GetNuimDisplayModes() failed: "
                << SDL_GetError() << std::endl;
            continue;
        }
        resolutions.reserve(resolutions.size() + n_display_modes);

        for (int mode_idx = 0; mode_idx < n_display_modes; mode_idx++) {
            // Reset display_mode
            display_mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
            // Get next display mode and error check
            if (SDL_GetDisplayMode(display_idx, mode_idx, &display_mode) != 0) {
                std::cerr << "SDL_GetDisplayMode() failed: "
                    << SDL_GetError() << std::endl;
            }
            else {
                // Add to list
                resolutions.push_back(vec2i{ display_mode.w, display_mode.h });
            }
        }
    }

    // Sort available resolutions by vertical resolution
    std::sort(resolutions.begin(), resolutions.end(),
        [](vec2i left, vec2i right) { return left.y < right.y; });
    // Remove duplicates
    for (int i = 1; i < static_cast<int>(resolutions.size()); i++) {
        if (resolutions[i] == resolutions[i - 1]) {
            resolutions.erase(resolutions.begin() + i);
            i -= 1;
        }
    }

    return resolutions;
}

} // namespace sini
