#ifndef maze_app_hpp
#define maze_app_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <cstdint> // Necessary for UINT32_MAX
#include <algorithm> // Necessary for std::min/std::max
#include <fstream> //Necessary for loading the shaders
#include <array>
#include <filesystem> // for path stuff


class HelloTriangleApplication
{
public:
    HelloTriangleApplication(char *argv[]);
    ~HelloTriangleApplication() {};
    void run();
    
private:
    GLFWwindow* window;
    
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    bool bUsingPortabilityExtension;
    
    //SwapChain member vars
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    
    std::vector<VkCommandBuffer> commandBuffers;
    
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    
    VkCommandPool commandPool;
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;
    bool  framebufferResized = false;
    
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    
    std::string pathToExecutable;
    
    int nbFrames = 0;
    double lastTime = 0;
    
    //Camera stuffs
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float mouseLastX = 0.0f, mouseLastY = 0.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    bool bFirstMouse = true;
    bool bInputEnabled = true;
    
    
    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    
    std::vector<VkPolygonMode> polygonRenderingModes = {VK_POLYGON_MODE_FILL, VK_POLYGON_MODE_LINE};
    size_t currentPolygonIndex = 0;
    bool changedPolygonMode;
    
    
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        
        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    
    // Functions for app
    void mainLoop();
    
    void loadModel();
    void initWindow();
    void initVulkan();
    void initInput();
    void cleanup();
    
    // Functions for Vulkan
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
    void createDescriptorPool();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createDescriptorSets();
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    void createDescriptorSetLayout();
    void createIndexBuffer();
    void createVertexBuffer();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createSyncObjects();
    void createCommandBuffers();
    void createCommandPool();
    void createFramebuffers();
    void createRenderPass();
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char>& shaderCode);
    void createImageViews();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createSwapChain();
    void createSurface();
    void getDeviceExtensions();
    void createLogicalDevice();
    void pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool checkValidationLayerSupport();
    void checkSupportedExtensions(std::vector<const char*> reqExtensions);
    std::vector<const char *> getRequriedExtensions();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void drawFrame();
    void createInstance();
    
    
    
    
    //Functions for Window
    void showFPS(GLFWwindow *pWindow);
    
    //Functions for Input
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow *window);
    
    
    //Utility functions
    static std::vector<char> readFile(const std::string& filename);
    
};


#endif /* maze_app_hpp */
