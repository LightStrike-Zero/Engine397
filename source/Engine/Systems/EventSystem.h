/**
 * @file EventSystem.h
 * @brief Defines a simple event system for handling key presses and rendering mode changes.
 * @author Probably Shaun
 * @date March 2025
 */

#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <functional>
#include <map>
#include <vector>
#include <string>

/**
 * @brief Types of events that can be dispatched.
 */
enum class EventType {
    KeyPressed,
    DrawModeChanged,
};

/**
 * @brief Base for all events.
 */
struct Event {
    EventType type;

    /**
     * @brief Construct a new Event object.
     * @param type The type of the event.
     */
    explicit Event(EventType type) : type(type) {}

    /// Virtual destructor
    virtual ~Event() = default;
};

/**
 * @brief Event triggered when a key is pressed.
 */
struct KeyPressedEvent : public Event {
    int keyCode;

    /**
     * @brief Construct a new Key Pressed Event object.
     * @param keyCode The key code of the pressed key.
     */
    KeyPressedEvent(int keyCode) : Event(EventType::KeyPressed), keyCode(keyCode) {}
};

/**
 * @brief Event triggered when the draw mode changes.
 */
struct DrawModeChangedEvent : public Event {
    bool lineMode;

    /**
     * @brief Construct a new Draw Mode Changed Event object.
     * @param lineMode True for wireframe mode, false for solid mode.
     */
    DrawModeChangedEvent(bool lineMode) : Event(EventType::DrawModeChanged), lineMode(lineMode) {}
};

/// Callback type for event handling
using EventCallback = std::function<void(const Event&)>;

/**
 * @brief Singleton class that manages event listeners and dispatches events.
 */
class EventSystem {
public:

    /**
     * @brief Get the singleton instance of the EventSystem.
     * @return Reference to the EventSystem instance.
     */
    static EventSystem& getInstance() {
        static EventSystem instance;
        return instance;
    }

    /**
     * @brief Add a listener for a specific event type.
     * @param type The type of event to listen for.
     * @param callback The callback to invoke when the event occurs.
     */
    void addListener(EventType type, EventCallback callback) {
        m_listeners[type].push_back(callback);
    }

    /**
     * @brief Dispatch an event to all registered listeners.
     * @param event The event to dispatch.
     */
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
