//
// Created by Shaun on 30/04/2025.
//

// AssetRegistry.cpp
#include "AssetRegistry.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

AssetRegistry& AssetRegistry::Get() {
    static AssetRegistry instance;
    return instance;
}

void AssetRegistry::Initialize(const std::string& assetsDir,
                               const std::string& registryFile) {
    m_assetsDir = assetsDir;
    m_registryFile = registryFile;

    // Ensure assets directory exists
    fs::create_directories(m_assetsDir);

    // Load existing registry
    LoadRegistry();

    // Scan any new files in Assets/ that aren't in the registry.json
    ScanAssetsDir();

    // Persist in case new entries were added
    Save();
}

void AssetRegistry::LoadRegistry() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream in(m_registryFile);
    if (!in.good()) return;

    json j;
    in >> j;
    for (auto& item : j["assets"]) {
        AssetEntry e;
        e.name     = item["name"].get<std::string>();
        e.srcPath  = item["srcPath"].get<std::string>();
        e.destPath = item["destPath"].get<std::string>();
        e.thumbKey = item.value("thumbKey", "placeholder");
        m_entries.push_back(std::move(e));
    }
}

void AssetRegistry::ScanAssetsDir() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& p : fs::recursive_directory_iterator(m_assetsDir)) {
        if (!p.is_regular_file()) continue;
        std::string pathStr = p.path().string();
        bool found = false;
        for (auto& e : m_entries) {
            if (e.destPath == pathStr) { found = true; break; }
        }
        if (!found) {
            AssetEntry e;
            e.name     = p.path().filename().string();
            e.srcPath  = pathStr;
            e.destPath = pathStr;
            e.thumbKey = "placeholder";
            m_entries.push_back(std::move(e));
        }
    }
}

void AssetRegistry::ImportAsset(const std::string& filePath) {
    namespace fs = std::filesystem;
    fs::path src(filePath);
    fs::path dst = fs::path(m_assetsDir) / src.filename();
    std::error_code ec;
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        std::cerr << "AssetRegistry: Failed to copy asset '"
                  << filePath << "': " << ec.message() << std::endl;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // avoid duplicates
        for (auto& e : m_entries) {
            if (e.destPath == dst.string()) return;
        }
        AssetEntry e;
        e.name     = dst.filename().string();
        e.srcPath  = filePath;
        e.destPath = dst.string();
        e.thumbKey = "placeholder";
        m_entries.push_back(std::move(e));
    }

    // now that the lock is gone, it's safe to save
    Save();
}

const std::vector<AssetEntry>& AssetRegistry::GetAssets() const {
    return m_entries;
}

void AssetRegistry::Save() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    json j;
    j["assets"] = json::array();
    for (auto& e : m_entries) {
        j["assets"].push_back({
            {"name",     e.name},
            {"srcPath",  e.srcPath},
            {"destPath", e.destPath},
            {"thumbKey", e.thumbKey}
        });
    }

    std::ofstream out(m_registryFile);
    out << j.dump(4);
}
