/*
  Copyright 2021 David Robillard <d@drobilla.net>

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

// Tests basic Vulkan support

#undef NDEBUG

#include "test_utils.h"

#include "pugl/pugl.h"
#include "pugl/vulkan.h"

#include <vulkan/vulkan_core.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Vulkan allocation callbacks which can be used for debugging
#define ALLOC_VK NULL

// Helper macro for allocating arrays by type, with C++ compatible cast
#define AALLOC(size, Type) ((Type*)calloc(size, sizeof(Type)))

// Helper macro for counted array arguments to make clang-format behave
#define COUNTED(count, ...) count, __VA_ARGS__

typedef struct {
  PuglWorld*      world;
  PuglView*       view;
  VkInstance      instance;
  VkSurfaceKHR    surface;
  PuglTestOptions opts;
  bool            exposed;
} PuglTest;

static bool
hasExtension(const char* const                  name,
             const VkExtensionProperties* const properties,
             const uint32_t                     count)
{
  for (uint32_t i = 0; i < count; ++i) {
    if (!strcmp(properties[i].extensionName, name)) {
      return true;
    }
  }

  return false;
}

static void
pushString(const char*** const array,
           uint32_t* const     count,
           const char* const   string)
{
  *array = (const char**)realloc(*array, (*count + 1) * sizeof(const char*));
  (*array)[*count] = string;
  ++*count;
}

static void
onExpose(PuglView* const view, const PuglExposeEvent* const event)
{
  (void)view;
  (void)event;

  /* ... */
}

static PuglStatus
onEvent(PuglView* const view, const PuglEvent* const event)
{
  PuglTest* const test = (PuglTest*)puglGetHandle(view);

  if (test->opts.verbose) {
    printEvent(event, "Event: ", true);
  }

  if (event->type == PUGL_EXPOSE) {
    onExpose(view, &event->expose);
    test->exposed = true;
  }

  return PUGL_SUCCESS;
}

static VkResult
createInstance(PuglTest* const test)
{
  const VkApplicationInfo appInfo = {
    VK_STRUCTURE_TYPE_APPLICATION_INFO,
    NULL,
    "Pugl Vulkan Test",
    VK_MAKE_VERSION(0, 1, 0),
    "Pugl Vulkan Test Engine",
    VK_MAKE_VERSION(0, 1, 0),
    VK_MAKE_VERSION(1, 0, 0),
  };

  // Get the number of supported extensions and layers
  VkResult vr          = VK_SUCCESS;
  uint32_t nExtProps   = 0;
  uint32_t nLayerProps = 0;
  if ((vr = vkEnumerateInstanceLayerProperties(&nLayerProps, NULL)) ||
      (vr = vkEnumerateInstanceExtensionProperties(NULL, &nExtProps, NULL))) {
    return vr;
  }

  // Get properties of supported extensions
  VkExtensionProperties* extProps = AALLOC(nExtProps, VkExtensionProperties);
  vkEnumerateInstanceExtensionProperties(NULL, &nExtProps, extProps);

  uint32_t     nExtensions = 0;
  const char** extensions  = NULL;

  // Add extensions required by pugl
  uint32_t           nPuglExts = 0;
  const char* const* puglExts  = puglGetInstanceExtensions(&nPuglExts);
  for (uint32_t i = 0; i < nPuglExts; ++i) {
    pushString(&extensions, &nExtensions, puglExts[i]);
  }

  // Add extra extensions we want to use if they are supported
  if (hasExtension("VK_EXT_debug_report", extProps, nExtProps)) {
    pushString(&extensions, &nExtensions, "VK_EXT_debug_report");
  }

  // Get properties of supported layers
  VkLayerProperties* layerProps = AALLOC(nLayerProps, VkLayerProperties);
  vkEnumerateInstanceLayerProperties(&nLayerProps, layerProps);

  const VkInstanceCreateInfo createInfo = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    NULL,
    0,
    &appInfo,
    COUNTED(0, NULL),
    COUNTED(nExtensions, extensions),
  };

  if ((vr = vkCreateInstance(&createInfo, ALLOC_VK, &test->instance))) {
    logError("Could not create Vulkan Instance: %d\n", vr);
  }

  free(extensions);
  free(layerProps);
  free(extProps);

  return vr;
}

int
main(int argc, char** argv)
{
  PuglWorld* const world = puglNewWorld(PUGL_PROGRAM, PUGL_WORLD_THREADS);

  PuglView* const         view   = puglNewView(world);
  PuglVulkanLoader* const loader = puglNewVulkanLoader(world);
  const PuglTestOptions   opts   = puglParseTestOptions(&argc, &argv);

  PuglTest test = {world, view, VK_NULL_HANDLE, VK_NULL_HANDLE, opts, false};

  // Create Vulkan instance
  assert(!createInstance(&test));

  // Create window
  puglSetClassName(test.world, "Pugl Vulkan Test");
  puglSetHandle(test.view, &test);
  puglSetBackend(test.view, puglVulkanBackend());
  puglSetEventFunc(test.view, onEvent);
  puglSetDefaultSize(test.view, 512, 512);
  assert(!puglRealize(test.view));

  // Create Vulkan surface for window
  assert(!puglCreateSurface(puglGetInstanceProcAddrFunc(loader),
                            test.view,
                            test.instance,
                            ALLOC_VK,
                            &test.surface));

  // Check that loading functions are available
  assert(puglGetInstanceProcAddrFunc(loader));
  assert(puglGetDeviceProcAddrFunc(loader));

  // Show view and drive event loop until the view gets exposed
  puglShow(test.view);
  while (!test.exposed) {
    puglUpdate(test.world, -1.0);
  }

  // Tear down
  puglFreeVulkanLoader(loader);
  puglFreeView(test.view);
  puglFreeWorld(test.world);

  return 0;
}
