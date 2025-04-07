
#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <functional>
#include <map>
#include <vector>
#include <string>

enum class EventType {
    KeyPressed,
    DrawModeChanged,
};

struct Event {
    EventType type;
    explicit Event(EventType type) : type(type) {}
    virtual ~Event() = default;
};

struct KeyPressedEvent : public Event {
    int keyCode;
    KeyPressedEvent(int keyCode) : Event(EventType::KeyPressed), keyCode(keyCode) {}
};

struct DrawModeChangedEvent : public Event {
    bool lineMode; 
    DrawModeChangedEvent(bool lineMode) : Event(EventType::DrawModeChanged), lineMode(lineMode) {}
};

using EventCallback = std::function<void(const Event&)>;

class EventSystem {
public:
    static EventSystem& getInstance() {
        static EventSystem instance;
        return instance;
    }

    // Add a listener
    void addListener(EventType type, EventCallback callback) {
        m_listeners[type].push_back(callback);
    }

    // Dispatch event
    void dispatchEvent(const Event& event) {
        auto& listeners = m_listeners[event.type];
        for (auto& listener : listeners) {
            listener(event);
        }
    }

private:
    std::map<EventType, std::vector<EventCallback>> m_listeners;
    
    EventSystem() = default;
    ~EventSystem() = default;
    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;
    EventSystem(EventSystem&&) = delete;
    EventSystem& operator=(EventSystem&&) = delete;
};

#endif //EVENTSYSTEM_H
