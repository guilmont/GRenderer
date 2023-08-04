#include "core.h"

namespace GRender::dialog::internal {
    namespace fs = std::filesystem;
    // Used
    int inputCompletion(ImGuiInputTextCallbackData* data);

    class DialogImpl {
    public:
        static DialogImpl* Instance() {
            static DialogImpl dialog;
            return &dialog;
        }

        DialogImpl(const DialogImpl&) = delete;
        DialogImpl& operator=(const DialogImpl&) = delete;

        void setMainPath(const fs::path& mainPath);

        void openDirectory(const std::string& title, const std::function<void(const fs::path&)>& callback);
        void openFile(const std::string& title, const std::vector<std::string>& extensions, const std::function<void(const fs::path&)>& callback);
        void saveFile(const std::string& title, const std::vector<std::string>& extensions, const std::function<void(const fs::path&)>& callback);

        void showDialog();

    private:
        DialogImpl();
        ~DialogImpl(void) = default;

        // Gets all directories and files available at current path.
        void updateAvailablePaths(void);

        // Specialized windows
        void showOpenDirectory(void);
        void showOpenFile(void);
        void showSaveFile(void);
        void fileExistsPopup(void);

        // Child widget with folders and files
        bool systemDisplay(void);

    private:
        // window size
        const glm::vec2 mSize = { 720.0f, 480.0f };

        // internal control
        bool mActive = false;
        bool mExistsPopup = false;
        void (DialogImpl::* internalShow)(void) = nullptr;

        // titles and extensions
        std::string mTitle;
        std::vector<std::string> mExtensions;
        std::string mCurrentExt = "";

        // callback info
        std::function<void(const std::filesystem::path&)> mCallback = nullptr;

        // File path variables
        std::string filename = "";
        std::filesystem::path mainpath;
        std::vector<std::filesystem::path> availablePaths;
        friend int inputCompletion(ImGuiInputTextCallbackData* data);
    };
} // namespace GRender