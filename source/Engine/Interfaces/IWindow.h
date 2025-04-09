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
    virtual void pollInputEvents() = 0; //custom function to poll input events, can be untied from window loop in the future
};

#endif // IWINDOW_H
