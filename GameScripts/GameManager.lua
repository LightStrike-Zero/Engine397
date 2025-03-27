-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of

assetPath = "Assets/survival_guitar_backpack_scaled/scene.gltf"


local success, io_module = pcall(require, "io")
if not success then
    print("Error: 'io' module is not available in this environment")
else
    print("io module is available")
end


-- Function to read the asset paths from the config file
function load_asset_paths(filename)
    local assets = {}  -- Table to store valid asset paths

    -- Open the file for reading
    local file = io.open(filename, "r")
    if not file then
        print("Error: Could not open file " .. filename)
        return assets
    end

    -- Read each line
    for line in file:lines() do
        local flag, path = line:match("([^,]+),(.+)")
        if flag == "load" then
            table.insert(assets, path)  -- Store only "load" paths
        end
    end

    -- Close the file
    file:close()

    return assets
end

-- Example usage
local asset_paths = load_asset_paths("asset_paths.config")

-- Print the loaded asset paths
for _, path in ipairs(asset_paths) do
    print("Loading asset:", path)
end


--
print("End of GameManager.lua")