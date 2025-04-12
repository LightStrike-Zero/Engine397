/**
 * @file IWindow.h
 * @brief Abstract interface for a platform-independent window system.
 * @author Probably Shaun + 1 function by Buko
 * @date March 2025
 */

#ifndef IWINDOW_H
#define IWINDOW_H

/**
 * @class IWindow
 * @brief Provides an interface for window management.
 */
class IWindow {
public:
    virtual ~IWindow() {}

    /**
     * @brief Checks whether the window should close.
     * @return True if a close event has been triggered.
     */
    virtual bool ShouldClose() const = 0;

    /// Polls system events such as input.
    virtual void PollEvents() = 0;

    /// Swaps front and back buffers.
    virtual void SwapBuffers() = 0;

    /**
     * @brief Gets the size of the framebuffer in pixels.
     * @param width width of the framebuffer.
     * @param height height of the framebuffer.
     */
    virtual void GetFramebufferSize(int &width, int &height) const = 0;

    /**
     * @brief Sets the input mode
     * @param mode The input mode.
     * @param value The new mode value.
     */
    virtual void SetInputMode(int mode, int value) = 0;

    /**
     * @brief Gets the time in seconds since the window was created.
     * @return Time in seconds.
     */
    virtual float GetTime() const = 0;

    /**
     * @brief Returns the native window pointer.
     * @return A pointer to the native window object.
     */
    virtual void* GetNativeWindow() const = 0;

    /**
     * @brief Sets the flag for whether the window should close.
     * @param value True to request window close.
     */
    virtual void SetShouldClose(bool value) = 0; // buko
    virtual void pollInputEvents() = 0; //custom function to poll input events, can be untied from window loop in the future
};

#endif // IWINDOW_H
