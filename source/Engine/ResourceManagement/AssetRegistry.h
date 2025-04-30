//
// Created by Shaun on 30/04/2025.
//

#ifndef ASSETREGISTRY_H
#define ASSETREGISTRY_H


#include <string>
#include <vector>
#include <filesystem>
#include <mutex>

struct AssetEntry {
    std::string name;       
    std::string srcPath;    
    std::string destPath;   
    std::string thumbKey;   
};

class AssetRegistry {
public:
    static AssetRegistry& Get();

    void Initialize(const std::string& assetsDir,
                    const std::string& registryFile);

    void ImportAsset(const std::string& filePath);

    const std::vector<AssetEntry>& GetAssets() const;

    void Save() const;

private:
    AssetRegistry() = default;
    ~AssetRegistry() = default;
    AssetRegistry(const AssetRegistry&) = delete;
    AssetRegistry& operator=(const AssetRegistry&) = delete;

    void LoadRegistry();
    void ScanAssetsDir();

    std::string m_assetsDir;
    std::string m_registryFile;
    std::vector<AssetEntry> m_entries;
    mutable std::mutex m_mutex; // thread safety
};

#endif //ASSETREGISTRY_H
