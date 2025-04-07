#ifndef IWINDOW_H
#define IWINDOW_H


class IWindow {
public:
    virtual ~IWindow() {}

    virtual bool ShouldClose() const = 0;
    virtual void PollEvents() = 0;
    virtual void SwapBuffers() = 0;
    virtual void GetFramebufferSize(int &width, int &height) const = 0;
    virtual void SetInputMode(int mode, int value) = 0;
    virtual float GetTime() const = 0;
    virtual void* GetNativeWindow() const = 0;
};

#endif // IWINDOW_H
