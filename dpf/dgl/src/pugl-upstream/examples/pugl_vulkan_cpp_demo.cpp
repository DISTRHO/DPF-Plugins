/*
  Copyright 2019-2020 David Robillard <d@drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
  An example of drawing with Vulkan.

  This is an example of using Vulkan for pixel-perfect 2D drawing.  It uses
  the same data and shaders as pugl_shader_demo.c and attempts to draw the
  same thing, except using Vulkan.

  Since Vulkan is a complicated and very verbose API, this example is
  unfortunately much larger than the others.  You should not use this as a
  resource to learn Vulkan, but it provides a decent demo of using Vulkan with
  Pugl that works nicely on all supported platforms.
*/

#include "demo_utils.h"
#include "file_utils.h"
#include "rects.h"
#include "test/test_utils.h"

#include "sybok.hpp"

#include "pugl/pugl.h"
#include "pugl/pugl.hpp"
#include "pugl/vulkan.hpp"

#include <vulkan/vk_platform.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

constexpr uintptr_t resizeTimerId = 1u;

struct PhysicalDeviceSelection {
  sk::PhysicalDevice physicalDevice;
  uint32_t           graphicsFamilyIndex;
};

/// Basic Vulkan context associated with the window
struct VulkanContext {
  VkResult init(pugl::VulkanLoader& loader, const PuglTestOptions& opts);

  sk::VulkanApi              vk;
  sk::Instance               instance;
  sk::DebugReportCallbackEXT debugCallback;
};

/// Basic setup of graphics device
struct GraphicsDevice {
  VkResult init(const pugl::VulkanLoader& loader,
                const VulkanContext&      context,
                pugl::View&               view,
                const PuglTestOptions&    opts);

  sk::SurfaceKHR     surface;
  sk::PhysicalDevice physicalDevice{};
  uint32_t           graphicsIndex{};
  VkSurfaceFormatKHR surfaceFormat{};
  VkPresentModeKHR   presentMode{};
  VkPresentModeKHR   resizePresentMode{};
  sk::Device         device{};
  sk::Queue          graphicsQueue{};
  sk::CommandPool    commandPool{};
};

/// Buffer allocated on the GPU
struct Buffer {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                VkDeviceSize          size,
                VkBufferUsageFlags    usage,
                VkMemoryPropertyFlags properties);

  sk::Buffer       buffer;
  sk::DeviceMemory deviceMemory;
};

/// A set of frames that can be rendered concurrently
struct Swapchain {
  VkResult init(const sk::VulkanApi&     vk,
                const GraphicsDevice&    gpu,
                VkSurfaceCapabilitiesKHR capabilities,
                VkExtent2D               extent,
                VkSwapchainKHR           oldSwapchain,
                bool                     resizing);

  VkSurfaceCapabilitiesKHR   capabilities{};
  VkExtent2D                 extent{};
  sk::SwapchainKHR           swapchain{};
  std::vector<sk::ImageView> imageViews{};
};

/// A pass that renders to a target
struct RenderPass {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                const Swapchain&      swapchain);

  sk::RenderPass                                   renderPass;
  std::vector<sk::Framebuffer>                     framebuffers;
  sk::CommandBuffers<std::vector<VkCommandBuffer>> commandBuffers;
};

/// Uniform buffer for constant data used in shaders
struct UniformBufferObject {
  mat4 projection;
};

/// Rectangle data that does not depend on renderer configuration
struct RectData {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                size_t                nRects);

  sk::DescriptorSetLayout descriptorSetLayout{};
  Buffer                  uniformBuffer{};
  sk::MappedMemory        uniformData{};
  Buffer                  modelBuffer{};
  Buffer                  instanceBuffer{};
  sk::MappedMemory        vertexData{};
  size_t                  numRects{};
};

/// Shader modules for drawing rectangles
struct RectShaders {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                const std::string&    programPath);

  sk::ShaderModule vert{};
  sk::ShaderModule frag{};
};

/// A pipeline to render rectangles with our shaders
struct RectPipeline {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                const RectData&       rectData,
                const RectShaders&    shaders,
                const Swapchain&      swapchain,
                const RenderPass&     renderPass);

  sk::DescriptorPool                               descriptorPool{};
  sk::DescriptorSets<std::vector<VkDescriptorSet>> descriptorSets{};
  sk::PipelineLayout                               pipelineLayout{};
  std::array<sk::Pipeline, 1>                      pipelines{};
  uint32_t                                         numImages{};
};

/// Synchronization primitives used to coordinate drawing frames
struct RenderSync {
  VkResult init(const sk::VulkanApi& vk,
                const sk::Device&    device,
                uint32_t             numImages);

  std::vector<sk::Semaphore> imageAvailable{};
  std::vector<sk::Semaphore> renderFinished{};
  std::vector<sk::Fence>     inFlight{};
  size_t                     currentFrame{};
};

/// Renderer that owns the above and everything required to draw
struct Renderer {
  VkResult init(const sk::VulkanApi&  vk,
                const GraphicsDevice& gpu,
                const RectData&       rectData,
                const RectShaders&    rectShaders,
                VkExtent2D            extent,
                bool                  resizing);

  VkResult recreate(const sk::VulkanApi&  vk,
                    const sk::SurfaceKHR& surface,
                    const GraphicsDevice& gpu,
                    const RectData&       rectData,
                    const RectShaders&    rectShaders,
                    VkExtent2D            extent,
                    bool                  resizing);

  Swapchain    swapchain;
  RenderPass   renderPass;
  RectPipeline rectPipeline;
  RenderSync   sync;
};

VkResult
selectSurfaceFormat(const sk::VulkanApi&      vk,
                    const sk::PhysicalDevice& physicalDevice,
                    const sk::SurfaceKHR&     surface,
                    VkSurfaceFormatKHR&       surfaceFormat)
{
  std::vector<VkSurfaceFormatKHR> formats;
  if (VkResult r = vk.getPhysicalDeviceSurfaceFormatsKHR(
        physicalDevice, surface, formats)) {
    return r;
  }

  for (const auto& format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      surfaceFormat = format;
      return VK_SUCCESS;
    }
  }

  return VK_ERROR_FORMAT_NOT_SUPPORTED;
}

VkResult
selectPresentMode(const sk::VulkanApi&      vk,
                  const sk::PhysicalDevice& physicalDevice,
                  const sk::SurfaceKHR&     surface,
                  const bool                multiBuffer,
                  const bool                sync,
                  VkPresentModeKHR&         presentMode)
{
  // Map command line options to mode priorities
  static constexpr VkPresentModeKHR priorities[][2][4] = {
    {
      // No double buffer, no sync
      {VK_PRESENT_MODE_IMMEDIATE_KHR,
       VK_PRESENT_MODE_MAILBOX_KHR,
       VK_PRESENT_MODE_FIFO_RELAXED_KHR,
       VK_PRESENT_MODE_FIFO_KHR},

      // No double buffer, sync (nonsense, map to FIFO relaxed)
      {VK_PRESENT_MODE_FIFO_RELAXED_KHR,
       VK_PRESENT_MODE_FIFO_KHR,
       VK_PRESENT_MODE_MAILBOX_KHR,
       VK_PRESENT_MODE_IMMEDIATE_KHR},
    },
    {
      // Double buffer, no sync
      {
        VK_PRESENT_MODE_MAILBOX_KHR,
        VK_PRESENT_MODE_IMMEDIATE_KHR,
        VK_PRESENT_MODE_FIFO_RELAXED_KHR,
        VK_PRESENT_MODE_FIFO_KHR,
      },

      // Double buffer, sync
      {VK_PRESENT_MODE_FIFO_KHR,
       VK_PRESENT_MODE_FIFO_RELAXED_KHR,
       VK_PRESENT_MODE_MAILBOX_KHR,
       VK_PRESENT_MODE_IMMEDIATE_KHR},
    },
  };

  std::vector<VkPresentModeKHR> modes;
  if (VkResult r = vk.getPhysicalDeviceSurfacePresentModesKHR(
        physicalDevice, surface, modes)) {
    return r;
  }

  const auto& tryModes = priorities[bool(multiBuffer)][bool(sync)];
  for (const auto m : tryModes) {
    if (std::find(modes.begin(), modes.end(), m) != modes.end()) {
      presentMode = m;
      return VK_SUCCESS;
    }
  }

  return VK_ERROR_INCOMPATIBLE_DRIVER;
}

VkResult
openDevice(const sk::VulkanApi&      vk,
           const sk::PhysicalDevice& physicalDevice,
           const uint32_t            graphicsFamilyIndex,
           sk::Device&               device)
{
  const float       graphicsQueuePriority = 1.0f;
  const char* const swapchainName         = "VK_KHR_swapchain";

  const VkDeviceQueueCreateInfo queueCreateInfo{
    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    nullptr,
    0u,
    graphicsFamilyIndex,
    SK_COUNTED(1u, &graphicsQueuePriority),
  };

  const VkDeviceCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                      nullptr,
                                      0u,
                                      SK_COUNTED(1u, &queueCreateInfo),
                                      SK_COUNTED(0u, nullptr), // Deprecated
                                      SK_COUNTED(1u, &swapchainName),
                                      nullptr};

  return vk.createDevice(physicalDevice, createInfo, device);
}

/// Return whether the physical device supports the extensions we require
VkResult
deviceSupportsRequiredExtensions(const sk::VulkanApi&      vk,
                                 const sk::PhysicalDevice& device,
                                 bool&                     supported)
{
  VkResult r = VK_SUCCESS;

  std::vector<VkExtensionProperties> props;
  if ((r = vk.enumerateDeviceExtensionProperties(device, props))) {
    return r;
  }

  supported = std::any_of(
    props.begin(), props.end(), [&](const VkExtensionProperties& e) {
      return !strcmp(e.extensionName, "VK_KHR_swapchain");
    });

  return VK_SUCCESS;
}

/// Return the index of the graphics queue, if there is one
VkResult
findGraphicsQueue(const sk::VulkanApi&      vk,
                  const sk::SurfaceKHR&     surface,
                  const sk::PhysicalDevice& device,
                  uint32_t&                 queueIndex)
{
  VkResult r = VK_SUCCESS;

  std::vector<VkQueueFamilyProperties> queueProps;
  if ((r = vk.getPhysicalDeviceQueueFamilyProperties(device, queueProps))) {
    return r;
  }

  for (uint32_t q = 0u; q < queueProps.size(); ++q) {
    if (queueProps[q].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      bool supported = false;
      if ((r = vk.getPhysicalDeviceSurfaceSupportKHR(
             device, q, surface, supported))) {
        return r;
      }

      if (supported) {
        queueIndex = q;
        return VK_SUCCESS;
      }
    }
  }

  return VK_ERROR_FEATURE_NOT_PRESENT;
}

/// Select a physical graphics device to use (simply the first found)
VkResult
selectPhysicalDevice(const sk::VulkanApi&     vk,
                     const sk::Instance&      instance,
                     const sk::SurfaceKHR&    surface,
                     PhysicalDeviceSelection& selection)
{
  VkResult r = VK_SUCCESS;

  std::vector<sk::PhysicalDevice> devices;
  if ((r = vk.enumeratePhysicalDevices(instance, devices))) {
    return r;
  }

  for (const auto& device : devices) {
    auto supported = false;
    if ((r = deviceSupportsRequiredExtensions(vk, device, supported))) {
      return r;
    }

    if (supported) {
      auto queueIndex = 0u;
      if ((r = findGraphicsQueue(vk, surface, device, queueIndex))) {
        return r;
      }

      selection = PhysicalDeviceSelection{device, queueIndex};
      return VK_SUCCESS;
    }
  }

  return VK_ERROR_INCOMPATIBLE_DISPLAY_KHR;
}

VkResult
GraphicsDevice::init(const pugl::VulkanLoader& loader,
                     const VulkanContext&      context,
                     pugl::View&               view,
                     const PuglTestOptions&    opts)
{
  const auto& vk = context.vk;
  VkResult    r  = VK_SUCCESS;

  // Create a Vulkan surface for the window using the Pugl API
  VkSurfaceKHR surfaceHandle = {};
  if ((r = pugl::createSurface(loader.getInstanceProcAddrFunc(),
                               view,
                               context.instance,
                               nullptr,
                               &surfaceHandle))) {
    return r;
  }

  // Wrap surface in a safe RAII handle
  surface =
    sk::SurfaceKHR{surfaceHandle, {context.instance, vk.vkDestroySurfaceKHR}};

  PhysicalDeviceSelection physicalDeviceSelection = {};
  // Select a physical device to use
  if ((r = selectPhysicalDevice(
         vk, context.instance, surface, physicalDeviceSelection))) {
    return r;
  }

  physicalDevice = physicalDeviceSelection.physicalDevice;
  graphicsIndex  = physicalDeviceSelection.graphicsFamilyIndex;

  if ((r = selectSurfaceFormat(vk, physicalDevice, surface, surfaceFormat)) ||
      (r = selectPresentMode(vk,
                             physicalDevice,
                             surface,
                             opts.doubleBuffer,
                             opts.sync,
                             presentMode)) ||
      (r = selectPresentMode(
         vk, physicalDevice, surface, true, false, resizePresentMode)) ||
      (r = openDevice(vk, physicalDevice, graphicsIndex, device))) {
    return r;
  }

  const VkCommandPoolCreateInfo commandPoolInfo{
    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr, {}, graphicsIndex};

  if ((r = vk.createCommandPool(device, commandPoolInfo, commandPool))) {
    return r;
  }

  graphicsQueue = vk.getDeviceQueue(device, graphicsIndex, 0);
  return VK_SUCCESS;
}

uint32_t
findMemoryType(const sk::VulkanApi&         vk,
               const sk::PhysicalDevice&    physicalDevice,
               const uint32_t               typeFilter,
               const VkMemoryPropertyFlags& properties)
{
  VkPhysicalDeviceMemoryProperties memProperties =
    vk.getPhysicalDeviceMemoryProperties(physicalDevice);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  return UINT32_MAX;
}

VkResult
Buffer::init(const sk::VulkanApi&        vk,
             const GraphicsDevice&       gpu,
             const VkDeviceSize          size,
             const VkBufferUsageFlags    usage,
             const VkMemoryPropertyFlags properties)
{
  const VkBufferCreateInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                      nullptr,
                                      {},
                                      size,
                                      usage,
                                      VK_SHARING_MODE_EXCLUSIVE,
                                      SK_COUNTED(0, nullptr)};

  const auto& device = gpu.device;

  VkResult r = VK_SUCCESS;
  if ((r = vk.createBuffer(device, bufferInfo, buffer))) {
    return r;
  }

  const auto requirements    = vk.getBufferMemoryRequirements(device, buffer);
  const auto memoryTypeIndex = findMemoryType(
    vk, gpu.physicalDevice, requirements.memoryTypeBits, properties);

  if (memoryTypeIndex == UINT32_MAX) {
    return VK_ERROR_FEATURE_NOT_PRESENT;
  }

  const VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                       nullptr,
                                       requirements.size,
                                       memoryTypeIndex};

  if ((r = vk.allocateMemory(device, allocInfo, deviceMemory)) ||
      (r = vk.bindBufferMemory(device, buffer, deviceMemory, 0))) {
    return r;
  }

  return VK_SUCCESS;
}

VkResult
Swapchain::init(const sk::VulkanApi&           vk,
                const GraphicsDevice&          gpu,
                const VkSurfaceCapabilitiesKHR surfaceCapabilities,
                const VkExtent2D               surfaceExtent,
                VkSwapchainKHR                 oldSwapchain,
                bool                           resizing)
{
  capabilities = surfaceCapabilities;
  extent       = surfaceExtent;

  const auto minNumImages =
    (!capabilities.maxImageCount || capabilities.maxImageCount >= 3u)
      ? 3u
      : capabilities.maxImageCount;

  const VkSwapchainCreateInfoKHR swapchainCreateInfo{
    VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    nullptr,
    {},
    gpu.surface,
    minNumImages,
    gpu.surfaceFormat.format,
    gpu.surfaceFormat.colorSpace,
    surfaceExtent,
    1,
    (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT),
    VK_SHARING_MODE_EXCLUSIVE,
    SK_COUNTED(0, nullptr),
    capabilities.currentTransform,
    VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    resizing ? gpu.resizePresentMode : gpu.presentMode,
    VK_TRUE,
    oldSwapchain};

  VkResult             r = VK_SUCCESS;
  std::vector<VkImage> images;
  if ((r = vk.createSwapchainKHR(gpu.device, swapchainCreateInfo, swapchain)) ||
      (r = vk.getSwapchainImagesKHR(gpu.device, swapchain, images))) {
    return r;
  }

  imageViews = std::vector<sk::ImageView>(images.size());
  for (size_t i = 0; i < images.size(); ++i) {
    const VkImageViewCreateInfo imageViewCreateInfo{
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      nullptr,
      {},
      images[i],
      VK_IMAGE_VIEW_TYPE_2D,
      gpu.surfaceFormat.format,
      {},
      {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};

    if ((r = vk.createImageView(
           gpu.device, imageViewCreateInfo, imageViews[i]))) {
      return r;
    }
  }

  return VK_SUCCESS;
}

VkResult
RenderPass::init(const sk::VulkanApi&  vk,
                 const GraphicsDevice& gpu,
                 const Swapchain&      swapchain)
{
  const auto numImages = static_cast<uint32_t>(swapchain.imageViews.size());

  assert(numImages > 0);

  // Create command buffers
  const VkCommandBufferAllocateInfo commandBufferAllocateInfo{
    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    nullptr,
    gpu.commandPool,
    VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    numImages};

  VkResult r = VK_SUCCESS;
  if ((r = vk.allocateCommandBuffers(
         gpu.device, commandBufferAllocateInfo, commandBuffers))) {
    return r;
  }

  static constexpr VkAttachmentReference colorAttachmentRef{
    0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

  static constexpr VkSubpassDescription subpass{
    {},
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    SK_COUNTED(0, nullptr),
    SK_COUNTED(1, &colorAttachmentRef, nullptr, nullptr),
    SK_COUNTED(0u, nullptr)};

  static constexpr VkSubpassDependency dependency{
    VK_SUBPASS_EXTERNAL,
    0,
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    (VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
     VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT),
    {},
    {}};

  const VkAttachmentDescription colorAttachment{
    {},
    gpu.surfaceFormat.format,
    VK_SAMPLE_COUNT_1_BIT,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    VK_ATTACHMENT_STORE_OP_DONT_CARE,
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };

  const VkRenderPassCreateInfo renderPassCreateInfo{
    VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    nullptr,
    {},
    SK_COUNTED(1, &colorAttachment),
    SK_COUNTED(1, &subpass),
    SK_COUNTED(1, &dependency)};

  if ((r = vk.createRenderPass(gpu.device, renderPassCreateInfo, renderPass))) {
    return r;
  }

  // Create framebuffers
  framebuffers = std::vector<sk::Framebuffer>(numImages);
  for (uint32_t i = 0; i < numImages; ++i) {
    const VkFramebufferCreateInfo framebufferCreateInfo{
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      nullptr,
      {},
      renderPass,
      SK_COUNTED(1, &swapchain.imageViews[i].get()),
      swapchain.extent.width,
      swapchain.extent.height,
      1};

    if ((r = vk.createFramebuffer(
           gpu.device, framebufferCreateInfo, framebuffers[i]))) {
      return r;
    }
  }

  return VK_SUCCESS;
}

std::vector<uint32_t>
readFile(const char* const programPath, const std::string& filename)
{
  std::unique_ptr<char, decltype(&free)> path{
    resourcePath(programPath, filename.c_str()), &free};

  std::cerr << "Loading shader:           " << path.get() << std::endl;

  std::unique_ptr<FILE, decltype(&fclose)> file{fopen(path.get(), "rb"),
                                                &fclose};

  if (!file) {
    std::cerr << "Failed to open file '" << filename << "'\n";
    return {};
  }

  fseek(file.get(), 0, SEEK_END);
  const auto fileSize = static_cast<size_t>(ftell(file.get()));
  fseek(file.get(), 0, SEEK_SET);

  const auto            numWords = fileSize / sizeof(uint32_t);
  std::vector<uint32_t> buffer(numWords);

  fread(buffer.data(), sizeof(uint32_t), numWords, file.get());

  return buffer;
}

VkResult
createShaderModule(const sk::VulkanApi&         vk,
                   const GraphicsDevice&        gpu,
                   const std::vector<uint32_t>& code,
                   sk::ShaderModule&            shaderModule)
{
  const VkShaderModuleCreateInfo createInfo{
    VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    nullptr,
    {},
    code.size() * sizeof(uint32_t),
    code.data()};

  return vk.createShaderModule(gpu.device, createInfo, shaderModule);
}

VkResult
RectShaders::init(const sk::VulkanApi&  vk,
                  const GraphicsDevice& gpu,
                  const std::string&    programPath)
{
  auto vertShaderCode = readFile(programPath.c_str(), "shaders/rect.vert.spv");

  auto fragShaderCode = readFile(programPath.c_str(), "shaders/rect.frag.spv");

  if (vertShaderCode.empty() || fragShaderCode.empty()) {
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  VkResult r = VK_SUCCESS;
  if ((r = createShaderModule(vk, gpu, vertShaderCode, vert)) ||
      (r = createShaderModule(vk, gpu, fragShaderCode, frag))) {
    return r;
  }

  return VK_SUCCESS;
}

VkResult
RectPipeline::init(const sk::VulkanApi&  vk,
                   const GraphicsDevice& gpu,
                   const RectData&       rectData,
                   const RectShaders&    shaders,
                   const Swapchain&      swapchain,
                   const RenderPass&     renderPass)
{
  const auto oldNumImages = numImages;
  VkResult   r            = VK_SUCCESS;

  numImages      = static_cast<uint32_t>(swapchain.imageViews.size());
  pipelines      = {};
  pipelineLayout = {};
  descriptorSets = {};

  if (numImages != oldNumImages) {
    // Create layout descriptor pool

    const VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                        numImages};

    const VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
      VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
      nullptr,
      VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
      numImages,
      1u,
      &poolSize};
    if ((r = vk.createDescriptorPool(
           gpu.device, descriptorPoolCreateInfo, descriptorPool))) {
      return r;
    }
  }

  const std::vector<VkDescriptorSetLayout> layouts(
    numImages, rectData.descriptorSetLayout.get());

  const VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    nullptr,
    descriptorPool,
    numImages,
    layouts.data()};
  if ((r = vk.allocateDescriptorSets(
         gpu.device, descriptorSetAllocateInfo, descriptorSets))) {
    return r;
  }

  const VkDescriptorBufferInfo bufferInfo{
    rectData.uniformBuffer.buffer, 0, sizeof(UniformBufferObject)};

  const std::array<VkWriteDescriptorSet, 1> descriptorWrites{
    {{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      nullptr,
      descriptorSets[0],
      0,
      0,
      1,
      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      nullptr,
      &bufferInfo,
      nullptr}}};

  const std::array<VkCopyDescriptorSet, 0> descriptorCopies{};

  vk.updateDescriptorSets(gpu.device, descriptorWrites, descriptorCopies);

  static constexpr std::array<VkVertexInputAttributeDescription, 4>
    vertexAttributeDescriptions{
      {// Model
       {0u, 0u, VK_FORMAT_R32G32_SFLOAT, 0},

       // Rect instance attributes
       {1u, 1u, VK_FORMAT_R32G32_SFLOAT, offsetof(Rect, pos)},
       {2u, 1u, VK_FORMAT_R32G32_SFLOAT, offsetof(Rect, size)},
       {3u, 1u, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Rect, fillColor)}}};

  static constexpr std::array<VkVertexInputBindingDescription, 2>
    vertexBindingDescriptions{
      VkVertexInputBindingDescription{
        0, sizeof(vec2), VK_VERTEX_INPUT_RATE_VERTEX},
      VkVertexInputBindingDescription{
        1u, sizeof(Rect), VK_VERTEX_INPUT_RATE_INSTANCE}};

  static constexpr VkPipelineInputAssemblyStateCreateInfo inputAssembly{
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    nullptr,
    {},
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    false};

  static constexpr VkPipelineRasterizationStateCreateInfo rasterizer{
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    nullptr,
    {},
    0,
    0,
    VK_POLYGON_MODE_FILL,
    VK_CULL_MODE_BACK_BIT,
    VK_FRONT_FACE_CLOCKWISE,
    0,
    0,
    0,
    0,
    1.0f};

  static constexpr VkPipelineMultisampleStateCreateInfo multisampling{
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    nullptr,
    {},
    VK_SAMPLE_COUNT_1_BIT,
    false,
    0.0f,
    nullptr,
    false,
    false};

  static constexpr VkPipelineColorBlendAttachmentState colorBlendAttachment{
    true,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_OP_ADD,
    (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)};

  const VkPipelineShaderStageCreateInfo shaderStages[] = {
    {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
     nullptr,
     {},
     VK_SHADER_STAGE_VERTEX_BIT,
     shaders.vert.get(),
     "main",
     nullptr},
    {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
     nullptr,
     {},
     VK_SHADER_STAGE_FRAGMENT_BIT,
     shaders.frag.get(),
     "main",
     nullptr}};

  const VkPipelineVertexInputStateCreateInfo vertexInputInfo{
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    nullptr,
    {},
    SK_COUNTED(static_cast<uint32_t>(vertexBindingDescriptions.size()),
               vertexBindingDescriptions.data()),
    SK_COUNTED(static_cast<uint32_t>(vertexAttributeDescriptions.size()),
               vertexAttributeDescriptions.data())};

  const VkViewport viewport{0.0f,
                            0.0f,
                            float(swapchain.extent.width),
                            float(swapchain.extent.height),
                            0.0f,
                            1.0f};

  const VkRect2D scissor{{0, 0}, swapchain.extent};

  const VkPipelineViewportStateCreateInfo viewportState{
    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    nullptr,
    {},
    SK_COUNTED(1, &viewport),
    SK_COUNTED(1, &scissor)};

  const VkPipelineColorBlendStateCreateInfo colorBlending{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    {},
    false,
    VK_LOGIC_OP_COPY,
    SK_COUNTED(1, &colorBlendAttachment),
    {1.0f, 0.0f, 0.0f, 0.0f}};

  const VkPipelineLayoutCreateInfo layoutInfo{
    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    nullptr,
    {},
    SK_COUNTED(1, &rectData.descriptorSetLayout.get()),
    SK_COUNTED(0, nullptr)};

  if ((r = vk.createPipelineLayout(gpu.device, layoutInfo, pipelineLayout))) {
    return r;
  }

  const std::array<VkGraphicsPipelineCreateInfo, 1> pipelineInfos{
    {{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      nullptr,
      {},
      SK_COUNTED(2, shaderStages),
      &vertexInputInfo,
      &inputAssembly,
      nullptr,
      &viewportState,
      &rasterizer,
      &multisampling,
      nullptr,
      &colorBlending,
      nullptr,
      pipelineLayout,
      renderPass.renderPass,
      0u,
      {},
      0}}};

  if ((r = vk.createGraphicsPipelines(
         gpu.device, {}, pipelineInfos, pipelines))) {
    return r;
  }

  return VK_SUCCESS;
}

VkResult
RectData::init(const sk::VulkanApi&  vk,
               const GraphicsDevice& gpu,
               const size_t          nRects)
{
  numRects = nRects;

  static constexpr VkDescriptorSetLayoutBinding uboLayoutBinding{
    0,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    1,
    VK_SHADER_STAGE_VERTEX_BIT,
    nullptr};

  const VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{
    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    nullptr,
    {},
    1,
    &uboLayoutBinding};

  VkResult r = VK_SUCCESS;
  if ((r = vk.createDescriptorSetLayout(
         gpu.device, descriptorSetLayoutInfo, descriptorSetLayout)) ||
      (r = uniformBuffer.init(vk,
                              gpu,
                              sizeof(UniformBufferObject),
                              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) ||
      (r = vk.mapMemory(gpu.device,
                        uniformBuffer.deviceMemory,
                        0,
                        sizeof(UniformBufferObject),
                        {},
                        uniformData))) {
    return r;
  }

  const VkBufferUsageFlags usageFlags =
    (VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
     VK_BUFFER_USAGE_TRANSFER_DST_BIT);

  const VkMemoryPropertyFlags propertyFlags =
    (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if ((r = modelBuffer.init(
         vk, gpu, sizeof(rectVertices), usageFlags, propertyFlags))) {
    return r;
  }

  {
    // Copy model vertices (directly, we do this only once)
    sk::MappedMemory modelData;
    if ((r = vk.mapMemory(gpu.device,
                          modelBuffer.deviceMemory,
                          0,
                          static_cast<VkDeviceSize>(sizeof(rectVertices)),
                          {},
                          modelData))) {
      return r;
    }

    memcpy(modelData.get(), rectVertices, sizeof(rectVertices));
  }

  if ((r = instanceBuffer.init(
         vk, gpu, sizeof(Rect) * numRects, usageFlags, propertyFlags))) {
    return r;
  }

  // Map attribute vertices (we will update them every frame)
  const auto rectsSize = static_cast<VkDeviceSize>(sizeof(Rect) * numRects);
  if ((r = vk.mapMemory(gpu.device,
                        instanceBuffer.deviceMemory,
                        0,
                        rectsSize,
                        {},
                        vertexData))) {
    return r;
  }

  return VK_SUCCESS;
}

VkResult
RenderSync::init(const sk::VulkanApi& vk,
                 const sk::Device&    device,
                 const uint32_t       numImages)
{
  const auto maxInFlight = std::max(1u, numImages - 1u);
  VkResult   r           = VK_SUCCESS;

  imageAvailable = std::vector<sk::Semaphore>(numImages);
  renderFinished = std::vector<sk::Semaphore>(numImages);
  for (uint32_t i = 0; i < numImages; ++i) {
    static constexpr VkSemaphoreCreateInfo semaphoreInfo{
      VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, nullptr, {}};

    if ((r = vk.createSemaphore(device, semaphoreInfo, imageAvailable[i])) ||
        (r = vk.createSemaphore(device, semaphoreInfo, renderFinished[i]))) {
      return r;
    }
  }

  inFlight = std::vector<sk::Fence>(maxInFlight);
  for (uint32_t i = 0; i < maxInFlight; ++i) {
    static constexpr VkFenceCreateInfo fenceInfo{
      VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      nullptr,
      VK_FENCE_CREATE_SIGNALED_BIT};

    if ((r = vk.createFence(device, fenceInfo, inFlight[i]))) {
      return r;
    }
  }

  return VK_SUCCESS;
}

VkResult
Renderer::init(const sk::VulkanApi&  vk,
               const GraphicsDevice& gpu,
               const RectData&       rectData,
               const RectShaders&    rectShaders,
               const VkExtent2D      extent,
               bool                  resizing)
{
  VkResult                 r            = VK_SUCCESS;
  VkSurfaceCapabilitiesKHR capabilities = {};

  if ((r = vk.getPhysicalDeviceSurfaceCapabilitiesKHR(
         gpu.physicalDevice, gpu.surface, capabilities)) ||
      (r = swapchain.init(vk, gpu, capabilities, extent, {}, resizing)) ||
      (r = renderPass.init(vk, gpu, swapchain)) ||
      (r = rectPipeline.init(
         vk, gpu, rectData, rectShaders, swapchain, renderPass))) {
    return r;
  }

  const auto numFrames = static_cast<uint32_t>(swapchain.imageViews.size());
  return sync.init(vk, gpu.device, numFrames);
}

VkResult
Renderer::recreate(const sk::VulkanApi&  vk,
                   const sk::SurfaceKHR& surface,
                   const GraphicsDevice& gpu,
                   const RectData&       rectData,
                   const RectShaders&    rectShaders,
                   const VkExtent2D      extent,
                   bool                  resizing)
{
  VkResult   r            = VK_SUCCESS;
  const auto oldNumImages = swapchain.imageViews.size();

  VkSurfaceCapabilitiesKHR capabilities = {};
  if ((r = vk.getPhysicalDeviceSurfaceCapabilitiesKHR(
         gpu.physicalDevice, surface, capabilities)) ||
      (r = swapchain.init(
         vk, gpu, capabilities, extent, swapchain.swapchain, resizing)) ||
      (r = renderPass.init(vk, gpu, swapchain)) ||
      (r = rectPipeline.init(
         vk, gpu, rectData, rectShaders, swapchain, renderPass))) {
    return r;
  }

  const auto numFrames = static_cast<uint32_t>(swapchain.imageViews.size());
  if (swapchain.imageViews.size() != oldNumImages) {
    return sync.init(vk, gpu.device, numFrames);
  }

  return VK_SUCCESS;
}

VKAPI_ATTR
VkBool32 VKAPI_CALL
debugCallback(VkDebugReportFlagsEXT flags,
              VkDebugReportObjectTypeEXT,
              uint64_t,
              size_t,
              int32_t,
              const char* layerPrefix,
              const char* msg,
              void*)
{
  std::cerr << sk::string(static_cast<VkDebugReportFlagBitsEXT>(flags)) << ": "
            << layerPrefix << ": " << msg << std::endl;

  return VK_FALSE;
}

bool
hasExtension(const char*                               name,
             const std::vector<VkExtensionProperties>& properties)
{
  for (const auto& p : properties) {
    if (!strcmp(p.extensionName, name)) {
      return true;
    }
  }

  return false;
}

bool
hasLayer(const char* name, const std::vector<VkLayerProperties>& properties)
{
  for (const auto& p : properties) {
    if (!strcmp(p.layerName, name)) {
      return true;
    }
  }

  return false;
}

template<class Value>
void
logInfo(const char* heading, const Value& value)
{
  std::cout << std::setw(26) << std::left << (std::string(heading) + ":")
            << value << std::endl;
}

VkResult
createInstance(sk::VulkanInitApi&     initApi,
               const PuglTestOptions& opts,
               sk::Instance&          instance)
{
  VkResult r = VK_SUCCESS;

  std::vector<VkLayerProperties>     layerProps;
  std::vector<VkExtensionProperties> extProps;
  if ((r = initApi.enumerateInstanceLayerProperties(layerProps)) ||
      (r = initApi.enumerateInstanceExtensionProperties(extProps))) {
    return r;
  }

  const auto puglExtensions = pugl::getInstanceExtensions();
  auto       extensions =
    std::vector<const char*>(puglExtensions.begin(), puglExtensions.end());

  // Add extra extensions we want to use if they are supported
  if (hasExtension("VK_EXT_debug_report", extProps)) {
    extensions.push_back("VK_EXT_debug_report");
  }

  // Add validation layers if error checking is enabled
  std::vector<const char*> layers;
  if (opts.errorChecking) {
    for (const char* l : {"VK_LAYER_KHRONOS_validation",
                          "VK_LAYER_LUNARG_standard_validation"}) {
      if (hasLayer(l, layerProps)) {
        layers.push_back(l);
      }
    }
  }

  for (const auto& e : extensions) {
    logInfo("Using instance extension", e);
  }

  for (const auto& l : layers) {
    logInfo("Using instance layer", l);
  }

  static constexpr VkApplicationInfo appInfo{
    VK_STRUCTURE_TYPE_APPLICATION_INFO,
    nullptr,
    "Pugl Vulkan Demo",
    0,
    nullptr,
    0,
    VK_MAKE_VERSION(1, 0, 0),
  };

  const VkInstanceCreateInfo createInfo{
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    nullptr,
    VkInstanceCreateFlags{},
    &appInfo,
    SK_COUNTED(uint32_t(layers.size()), layers.data()),
    SK_COUNTED(uint32_t(extensions.size()), extensions.data())};

  return initApi.createInstance(createInfo, instance);
}

VkResult
getDebugReportCallback(sk::VulkanApi&              api,
                       sk::Instance&               instance,
                       const bool                  verbose,
                       sk::DebugReportCallbackEXT& callback)
{
  if (api.vkCreateDebugReportCallbackEXT) {
    VkDebugReportFlagsEXT flags = (VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                   VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                                   VK_DEBUG_REPORT_ERROR_BIT_EXT);

    if (verbose) {
      flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
      flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    }

    const VkDebugReportCallbackCreateInfoEXT createInfo{
      VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
      nullptr,
      flags,
      debugCallback,
      nullptr};

    return api.createDebugReportCallbackEXT(instance, createInfo, callback);
  }

  return VK_ERROR_FEATURE_NOT_PRESENT;
}

void
recordCommandBuffer(sk::CommandScope&   cmd,
                    const Swapchain&    swapchain,
                    const RenderPass&   renderPass,
                    const RectPipeline& rectPipeline,
                    const RectData&     rectData,
                    const size_t        imageIndex)
{
  const VkClearColorValue clearColorValue{{0.0f, 0.0f, 0.0f, 1.0f}};
  const VkClearValue      clearValue{clearColorValue};

  const VkRenderPassBeginInfo renderPassBegin{
    VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    nullptr,
    renderPass.renderPass,
    renderPass.framebuffers[imageIndex],
    VkRect2D{{0, 0}, swapchain.extent},
    SK_COUNTED(1, &clearValue)};

  auto pass = cmd.beginRenderPass(renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

  pass.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, rectPipeline.pipelines[0]);

  const std::array<VkDeviceSize, 1> offsets{0};
  pass.bindVertexBuffers(
    0u, SK_COUNTED(1u, &rectData.modelBuffer.buffer.get(), offsets.data()));

  pass.bindVertexBuffers(
    1u, SK_COUNTED(1u, &rectData.instanceBuffer.buffer.get(), offsets.data()));

  pass.bindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
                          rectPipeline.pipelineLayout,
                          0u,
                          SK_COUNTED(1u, rectPipeline.descriptorSets.get()),
                          0u,
                          nullptr);

  pass.draw(4u, static_cast<uint32_t>(rectData.numRects), 0u, 0u);
}

VkResult
recordCommandBuffers(const sk::VulkanApi& vk,
                     const Swapchain&     swapchain,
                     const RenderPass&    renderPass,
                     const RectPipeline&  rectPipeline,
                     const RectData&      rectData)
{
  VkResult r = VK_SUCCESS;

  for (size_t i = 0; i < swapchain.imageViews.size(); ++i) {
    const VkCommandBufferBeginInfo beginInfo{
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      nullptr,
      VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
      nullptr};

    auto* const commandBuffer = renderPass.commandBuffers[i];
    auto        cmd           = vk.beginCommandBuffer(commandBuffer, beginInfo);
    if (!cmd) {
      return cmd.error();
    }

    recordCommandBuffer(cmd, swapchain, renderPass, rectPipeline, rectData, i);

    if ((r = cmd.end())) {
      return r;
    }
  }

  return VK_SUCCESS;
}

class PuglVulkanDemo;

class View : public pugl::View
{
public:
  View(pugl::World& world, PuglVulkanDemo& app)
    : pugl::View{world}
    , _app{app}
  {
    setEventHandler(*this);
  }

  template<PuglEventType t, class Base>
  pugl::Status onEvent(const pugl::Event<t, Base>&) noexcept
  {
    return pugl::Status::success;
  }

  pugl::Status onEvent(const pugl::ConfigureEvent& event);
  pugl::Status onEvent(const pugl::UpdateEvent& event);
  pugl::Status onEvent(const pugl::ExposeEvent& event);
  pugl::Status onEvent(const pugl::LoopEnterEvent& event);
  pugl::Status onEvent(const pugl::TimerEvent& event);
  pugl::Status onEvent(const pugl::LoopLeaveEvent& event);
  pugl::Status onEvent(const pugl::KeyPressEvent& event);
  pugl::Status onEvent(const pugl::CloseEvent& event);

private:
  PuglVulkanDemo& _app;
};

class PuglVulkanDemo
{
public:
  PuglVulkanDemo(const char*            executablePath,
                 const PuglTestOptions& o,
                 size_t                 numRects);

  const char*        programPath;
  PuglTestOptions    opts;
  pugl::World        world;
  pugl::VulkanLoader loader;
  View               view;
  VulkanContext      vulkan;
  GraphicsDevice     gpu;
  Renderer           renderer;
  RectData           rectData;
  RectShaders        rectShaders;
  uint32_t           framesDrawn{0};
  VkExtent2D         extent{512u, 512u};
  std::vector<Rect>  rects;
  bool               resizing{false};
  bool               quit{false};
};

std::vector<Rect>
makeRects(const size_t numRects, const uint32_t windowWidth)
{
  std::vector<Rect> rects(numRects);
  for (size_t i = 0; i < numRects; ++i) {
    rects[i] = makeRect(i, static_cast<float>(windowWidth));
  }

  return rects;
}

PuglVulkanDemo::PuglVulkanDemo(const char* const      executablePath,
                               const PuglTestOptions& o,
                               const size_t           numRects)
  : programPath{executablePath}
  , opts{o}
  , world{pugl::WorldType::program, pugl::WorldFlag::threads}
  , loader{world}
  , view{world, *this}
  , rects{makeRects(numRects, extent.width)}
{}

VkResult
recreateRenderer(PuglVulkanDemo&       app,
                 const sk::VulkanApi&  vk,
                 const GraphicsDevice& gpu,
                 const VkExtent2D      extent,
                 const RectData&       rectData,
                 const RectShaders&    rectShaders)
{
  VkResult                 r            = VK_SUCCESS;
  VkSurfaceCapabilitiesKHR capabilities = {};
  if ((r = vk.getPhysicalDeviceSurfaceCapabilitiesKHR(
         gpu.physicalDevice, gpu.surface, capabilities))) {
    return r;
  }

  // There is a known race issue here, so we clamp and hope for the best
  const VkExtent2D clampedExtent{
    std::min(capabilities.maxImageExtent.width,
             std::max(capabilities.minImageExtent.width, extent.width)),
    std::min(capabilities.maxImageExtent.height,
             std::max(capabilities.minImageExtent.height, extent.height))};

  if ((r = vk.deviceWaitIdle(gpu.device)) ||
      (r = app.renderer.recreate(vk,
                                 gpu.surface,
                                 gpu,
                                 rectData,
                                 rectShaders,
                                 clampedExtent,
                                 app.resizing))) {
    return r;
  }

  // Reset current (initially signaled) fence because we already waited
  vk.resetFence(gpu.device,
                app.renderer.sync.inFlight[app.renderer.sync.currentFrame]);

  // Record new command buffers
  return recordCommandBuffers(vk,
                              app.renderer.swapchain,
                              app.renderer.renderPass,
                              app.renderer.rectPipeline,
                              rectData);
}

pugl::Status
View::onEvent(const pugl::ConfigureEvent& event)
{
  // We just record the size here and lazily resize the surface when exposed
  _app.extent = {static_cast<uint32_t>(event.width),
                 static_cast<uint32_t>(event.height)};

  return pugl::Status::success;
}

pugl::Status
View::onEvent(const pugl::UpdateEvent&)
{
  return postRedisplay();
}

VkResult
beginFrame(PuglVulkanDemo& app, const sk::Device& device, uint32_t& imageIndex)
{
  const auto& vk = app.vulkan.vk;

  VkResult r = VK_SUCCESS;

  // Wait until we can start rendering the next frame
  if ((r = vk.waitForFence(
         device, app.renderer.sync.inFlight[app.renderer.sync.currentFrame])) ||
      (r = vk.resetFence(
         device, app.renderer.sync.inFlight[app.renderer.sync.currentFrame]))) {
    return r;
  }

  // Rebuild the renderer first if the window size has changed
  if (app.extent.width != app.renderer.swapchain.extent.width ||
      app.extent.height != app.renderer.swapchain.extent.height) {
    if ((r = recreateRenderer(
           app, vk, app.gpu, app.extent, app.rectData, app.rectShaders))) {
      return r;
    }
  }

  // Acquire the next image to render, rebuilding if necessary
  while ((r = vk.acquireNextImageKHR(
            device,
            app.renderer.swapchain.swapchain,
            UINT64_MAX,
            app.renderer.sync.imageAvailable[app.renderer.sync.currentFrame],
            {},
            &imageIndex))) {
    switch (r) {
    case VK_SUBOPTIMAL_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR:
      if ((r = recreateRenderer(app,
                                vk,
                                app.gpu,
                                app.renderer.swapchain.extent,
                                app.rectData,
                                app.rectShaders))) {
        return r;
      }
      continue;
    default:
      return r;
    }
  }

  return VK_SUCCESS;
}

void
update(PuglVulkanDemo& app, const double time)
{
  // Animate rectangles
  for (size_t i = 0; i < app.rects.size(); ++i) {
    moveRect(&app.rects[i],
             i,
             app.rects.size(),
             static_cast<float>(app.extent.width),
             static_cast<float>(app.extent.height),
             time);
  }

  // Update vertex buffer
  memcpy(app.rectData.vertexData.get(),
         app.rects.data(),
         sizeof(Rect) * app.rects.size());

  // Update uniform buffer
  UniformBufferObject ubo = {{}};
  mat4Ortho(ubo.projection,
            0.0f,
            float(app.renderer.swapchain.extent.width),
            0.0f,
            float(app.renderer.swapchain.extent.height),
            -1.0f,
            1.0f);

  memcpy(app.rectData.uniformData.get(), &ubo, sizeof(ubo));
}

VkResult
endFrame(const sk::VulkanApi&  vk,
         const GraphicsDevice& gpu,
         const Renderer&       renderer,
         const uint32_t        imageIndex)
{
  const auto currentFrame = renderer.sync.currentFrame;
  VkResult   r            = VK_SUCCESS;

  static constexpr VkPipelineStageFlags waitStage =
    VK_PIPELINE_STAGE_TRANSFER_BIT;

  const VkSubmitInfo submitInfo{
    VK_STRUCTURE_TYPE_SUBMIT_INFO,
    nullptr,
    SK_COUNTED(1, &renderer.sync.imageAvailable[currentFrame].get()),
    &waitStage,
    SK_COUNTED(1, &renderer.renderPass.commandBuffers[imageIndex]),
    SK_COUNTED(1, &renderer.sync.renderFinished[imageIndex].get())};

  if ((r = vk.queueSubmit(gpu.graphicsQueue,
                          submitInfo,
                          renderer.sync.inFlight[currentFrame]))) {
    return r;
  }

  const VkPresentInfoKHR presentInfo{
    VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    nullptr,
    SK_COUNTED(1, &renderer.sync.renderFinished[imageIndex].get()),
    SK_COUNTED(1, &renderer.swapchain.swapchain.get(), &imageIndex),
    nullptr};

  switch ((r = vk.queuePresentKHR(gpu.graphicsQueue, presentInfo))) {
  case VK_SUCCESS:               // Sucessfully presented
  case VK_SUBOPTIMAL_KHR:        // Probably a resize race, ignore
  case VK_ERROR_OUT_OF_DATE_KHR: // Probably a resize race, ignore
    break;
  default:
    return r;
  }

  return VK_SUCCESS;
}

pugl::Status
View::onEvent(const pugl::ExposeEvent&)
{
  const auto& vk  = _app.vulkan.vk;
  const auto& gpu = _app.gpu;

  // Acquire the next image, waiting and/or rebuilding if necessary
  auto nextImageIndex = 0u;
  if (beginFrame(_app, gpu.device, nextImageIndex)) {
    return pugl::Status::unknownError;
  }

  // Ready to go, update the data to the current time
  update(_app, world().time());

  // Submit the frame to the queue and present it
  endFrame(vk, gpu, _app.renderer, nextImageIndex);

  ++_app.framesDrawn;
  ++_app.renderer.sync.currentFrame;
  _app.renderer.sync.currentFrame %= _app.renderer.sync.inFlight.size();

  return pugl::Status::success;
}

pugl::Status
View::onEvent(const pugl::LoopEnterEvent&)
{
  _app.resizing = true;
  startTimer(resizeTimerId,
             1.0 / static_cast<double>(getHint(pugl::ViewHint::refreshRate)));

  return pugl::Status::success;
}

pugl::Status
View::onEvent(const pugl::TimerEvent&)
{
  return postRedisplay();
}

pugl::Status
View::onEvent(const pugl::LoopLeaveEvent&)
{
  stopTimer(resizeTimerId);

  // Trigger a swapchain recreation with the normal present mode
  _app.renderer.swapchain.extent = {};
  _app.resizing                  = false;

  return pugl::Status::success;
}

pugl::Status
View::onEvent(const pugl::KeyPressEvent& event)
{
  if (event.key == PUGL_KEY_ESCAPE || event.key == 'q') {
    _app.quit = true;
  }

  return pugl::Status::success;
}

pugl::Status
View::onEvent(const pugl::CloseEvent&)
{
  _app.quit = true;

  return pugl::Status::success;
}

VkResult
VulkanContext::init(pugl::VulkanLoader& loader, const PuglTestOptions& opts)
{
  VkResult r = VK_SUCCESS;

  sk::VulkanInitApi initApi{};

  // Load Vulkan API and set up the fundamentals
  if ((r = initApi.init(loader.getInstanceProcAddrFunc())) ||
      (r = createInstance(initApi, opts, instance)) ||
      (r = vk.init(initApi, instance)) ||
      (r = getDebugReportCallback(vk, instance, opts.verbose, debugCallback))) {
    return r;
  }

  return VK_SUCCESS;
}

int
run(const char* const     programPath,
    const PuglTestOptions opts,
    const size_t          numRects)
{
  PuglVulkanDemo app{programPath, opts, numRects};

  VkResult   r      = VK_SUCCESS;
  const auto width  = static_cast<int>(app.extent.width);
  const auto height = static_cast<int>(app.extent.height);

  // Realize window so we can set up Vulkan
  app.world.setClassName("PuglVulkanDemo");
  app.view.setWindowTitle("Pugl Vulkan Demo");
  app.view.setAspectRatio(1, 1, 16, 9);
  app.view.setDefaultSize(width, height);
  app.view.setMinSize(width / 4, height / 4);
  app.view.setMaxSize(width * 4, height * 4);
  app.view.setBackend(pugl::vulkanBackend());
  app.view.setHint(pugl::ViewHint::resizable, opts.resizable);
  const pugl::Status st = app.view.realize();
  if (st != pugl::Status::success) {
    return logError("Failed to create window (%s)\n", pugl::strerror(st));
  }

  if (!app.loader) {
    return logError("Failed to load Vulkan library\n");
  }

  // Load Vulkan for the view
  if ((r = app.vulkan.init(app.loader, opts))) {
    return logError("Failed to set up Vulkan API (%s)\n", sk::string(r));
  }

  const auto& vk = app.vulkan.vk;

  // Set up the graphics device
  if ((r = app.gpu.init(app.loader, app.vulkan, app.view, opts))) {
    return logError("Failed to set up device (%s)\n", sk::string(r));
  }

  logInfo("Present mode", sk::string(app.gpu.presentMode));
  logInfo("Resize present mode", sk::string(app.gpu.resizePresentMode));

  // Set up the rectangle data we will render every frame
  if ((r = app.rectData.init(vk, app.gpu, app.rects.size()))) {
    return logError("Failed to allocate render data (%s)\n", sk::string(r));
  }

  // Load shader modules
  if ((r = app.rectShaders.init(vk, app.gpu, app.programPath))) {
    return logError("Failed to load shaders (%s)\n", sk::string(r));
  }

  if ((r = app.renderer.init(app.vulkan.vk,
                             app.gpu,
                             app.rectData,
                             app.rectShaders,
                             app.extent,
                             false))) {
    return logError("Failed to create renderer (%s)\n", sk::string(r));
  }

  logInfo("Swapchain frames",
          std::to_string(app.renderer.swapchain.imageViews.size()));
  logInfo("Frames in flight",
          std::to_string(app.renderer.sync.inFlight.size()));

  recordCommandBuffers(app.vulkan.vk,
                       app.renderer.swapchain,
                       app.renderer.renderPass,
                       app.renderer.rectPipeline,
                       app.rectData);

  const int    refreshRate   = app.view.getHint(pugl::ViewHint::refreshRate);
  const double frameDuration = 1.0 / static_cast<double>(refreshRate);
  const double timeout       = app.opts.sync ? frameDuration : 0.0;

  PuglFpsPrinter fpsPrinter = {app.world.time()};
  app.view.show();
  while (!app.quit) {
    app.world.update(timeout);
    puglPrintFps(app.world.cobj(), &fpsPrinter, &app.framesDrawn);
  }

  if ((r = app.vulkan.vk.deviceWaitIdle(app.gpu.device))) {
    return logError("Failed to wait for device idle (%s)\n", sk::string(r));
  }

  return 0;
}

} // namespace

int
main(int argc, char** argv)
{
  // Parse command line options
  const char* const     programPath = argv[0];
  const PuglTestOptions opts        = puglParseTestOptions(&argc, &argv);
  if (opts.help) {
    puglPrintTestUsage(programPath, "");
    return 0;
  }

  // Parse number of rectangles argument, if given
  int64_t numRects = 1000;
  if (argc >= 1) {
    char* endptr = nullptr;
    numRects     = strtol(argv[0], &endptr, 10);
    if (endptr != argv[0] + strlen(argv[0]) || numRects < 1) {
      logError("Invalid number of rectangles: %s\n", argv[0]);
      return 1;
    }
  }

  // Run application
  return run(programPath, opts, static_cast<size_t>(numRects));
}
