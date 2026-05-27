#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <map>

#define XR_USE_PLATFORM_ANDROID
#define XR_USE_GRAPHICS_API_VULKAN
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#include "../api/kel.hpp"
#include "../kel/ui/drawing.hpp"

#define LOG_TAG "KelUI_OpenXR"



extern void* patchPLTGOT(const char* soname, const char* symname, void* newfn);

struct XrGenericHeader {
    XrStructureType type;
    const XrGenericHeader* next;
};

struct Vk {
    VkDevice dev;
    VkInstance inst;
    VkPhysicalDevice phys;
    VkQueue q;
    uint32_t qfam;
};

static Vk vk{};
static bool init = false;
static VkCommandBuffer cmd{};
static VkFormat fmt = VK_FORMAT_R8G8B8A8_SRGB;

struct SC {
    std::vector<VkImage> imgs;
    std::vector<VkImageView> views;
    uint32_t w, h;
};

static std::map<XrSwapchain, SC> scs;

static PFN_xrCreateSession orig_cs;
static PFN_xrEndFrame orig_ef;
static PFN_xrEnumerateSwapchainImages orig_esi;
static PFN_xrCreateSwapchain orig_csc;

typedef VkResult (*pfn_vkBeginCommandBuffer)(VkCommandBuffer, const VkCommandBufferBeginInfo*);
static pfn_vkBeginCommandBuffer orig_bcb;

VkResult h_bcb(VkCommandBuffer c, const VkCommandBufferBeginInfo* i) {
    cmd = c;
    return orig_bcb(c, i);
}

XrResult h_csc(XrSession session, const XrSwapchainCreateInfo* ci, XrSwapchain* sc) {
    XrResult r = orig_csc(session, ci, sc);
    if (r == XR_SUCCESS) {
        fmt = (VkFormat)ci->format;
        auto& d = scs[*sc];
        d.w = ci->width;
        d.h = ci->height;
        
        if (!init && vk.dev && vk.q && vk.inst) {
            kel::getapi()->initvulkan(vk.inst, vk.phys, vk.dev, vk.q, vk.qfam, fmt);
            init = true;
        }
    }
    return r;
}

XrResult h_esi(XrSwapchain sc, uint32_t cap, uint32_t* cnt, XrSwapchainImageBaseHeader* imgs) {
    XrResult r = orig_esi(sc, cap, cnt, imgs);

    if (r == XR_SUCCESS && imgs && cap > 0 && vk.dev) {
        auto& d = scs[sc];
        d.imgs.clear();
        d.views.clear();

        auto* vi = (XrSwapchainImageVulkanKHR*)imgs;
        for (uint32_t i = 0; i < *cnt; i++) {
            d.imgs.push_back(vi[i].image);

            VkImageViewCreateInfo vci{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            vci.image = vi[i].image;
            vci.viewType = VK_IMAGE_VIEW_TYPE_2D;
            vci.format = fmt;
            vci.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
            VkImageView v;
            if (vkCreateImageView(vk.dev, &vci, nullptr, &v) == VK_SUCCESS) {
                d.views.push_back(v);
            }
        }
    }
    return r;
}

XrResult h_cs(XrInstance inst, const XrSessionCreateInfo* ci, XrSession* sess) {
    auto* next = (const XrGenericHeader*)ci->next;
    while (next) {
        if (next->type == XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR) {
            auto* b = (XrGraphicsBindingVulkanKHR*)next;
            vk.inst = b->instance;
            vk.phys = b->physicalDevice;
            vk.dev = b->device;
            vk.qfam = b->queueFamilyIndex;
            vkGetDeviceQueue(vk.dev, vk.qfam, b->queueIndex, &vk.q);
        }
        next = next->next;
    }
    return orig_cs(inst, ci, sess);
}

XrResult h_ef(XrSession sess, const XrFrameEndInfo* fi) {
    auto api = kel::getapi();

    for (uint32_t i = 0; i < fi->layerCount; i++) {
        auto* lyr = (XrCompositionLayerBaseHeader*)fi->layers[i];
        if (lyr->type == XR_TYPE_COMPOSITION_LAYER_PROJECTION) {
            auto* p = (XrCompositionLayerProjection*)lyr;
            for (uint32_t v = 0; v < p->viewCount; v++) {
                auto sc = p->views[v].subImage.swapchain;
                auto idx = p->views[v].subImage.imageArrayIndex;

                if (scs.count(sc) && idx < scs[sc].views.size() && cmd) {
                    auto& d = scs[sc];
                    kel::Drawing::set_resolution(d.w, d.h);

                    api->rect(50, 50, 350, 500, 0.05f, 0.05f, 0.05f, 0.95f);
                    api->text(70, 70, "KELUI SDK", 1, 1, 1, 1);

                    static bool mod1 = true;
                    api->checkbox("Auto-Aim", &mod1);

                    VkExtent2D e = { d.w, d.h };
                    api->rendervulkan(cmd, e, d.imgs[idx], d.views[idx]);
                }
            }
        }
    }

    return orig_ef(sess, fi);
}

extern "C" void init_kelui() {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    orig_cs = (PFN_xrCreateSession)patchPLTGOT("libopenxr_loader.so", "xrCreateSession", (void*)h_cs);
    orig_ef = (PFN_xrEndFrame)patchPLTGOT("libopenxr_loader.so", "xrEndFrame", (void*)h_ef);
    orig_csc = (PFN_xrCreateSwapchain)patchPLTGOT("libopenxr_loader.so", "xrCreateSwapchain", (void*)h_csc);
    orig_esi = (PFN_xrEnumerateSwapchainImages)patchPLTGOT("libopenxr_loader.so", "xrEnumerateSwapchainImagesVulkanKHR", (void*)h_esi);

    orig_bcb = (pfn_vkBeginCommandBuffer)patchPLTGOT("libvulkan.so", "vkBeginCommandBuffer", (void*)h_bcb);
}
