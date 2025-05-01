//
// Created by light on 1/05/2025.
//

#ifndef PROTOTYPEREGISTRY_H
#define PROTOTYPEREGISTRY_H

#include <string>
#include <vector>
#include <mutex>

// Entry representing a spawnable archetype
struct PrototypeEntry {
    std::string id;                // Unique archetype name
    // Future: component data can be stored here (JSON or structured)
};

class PrototypeRegistry {
public:
    // Access the singleton instance
    static PrototypeRegistry& Get();

    /**
     * Initialize the registry by loading existing prototypes from the given JSON file.
     * If the file doesn't exist, starts with an empty list.
     */
    void Initialize(const std::string& registryFile);

    /**
     * Creates a new empty archetype with the given id.
     * Returns true if created, false if an entry with the same id already exists.
     */
    bool CreatePrototype(const std::string& id);

    /**
     * Returns the list of all registered archetypes.
     */
    const std::vector<PrototypeEntry>& GetPrototypes() const;

    /**
     * Saves the current registry back to the JSON file.
     */
    void Save() const;

private:
    PrototypeRegistry() = default;
    ~PrototypeRegistry() = default;
    PrototypeRegistry(const PrototypeRegistry&) = delete;
    PrototypeRegistry& operator=(const PrototypeRegistry&) = delete;

    // Load from disk into m_entries
    void LoadRegistry();

    std::string m_registryFile;
    std::vector<PrototypeEntry> m_entries;
    mutable std::mutex m_mutex;
};

#endif //PROTOTYPEREGISTRY_H
