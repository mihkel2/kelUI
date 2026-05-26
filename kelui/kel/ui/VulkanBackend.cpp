#include "VulkanBackend.hpp"
#include "drawing.hpp"
#include <android/log.h>

namespace kel {

void VulkanBackend::init(const Context& context) {
    ctx = context;
    createrenderpass();
    creategraphicspipeline();
}

void VulkanBackend::createrenderpass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = ctx.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vkCreateRenderPass(ctx.device, &renderPassInfo, nullptr, &renderPass);
}

void VulkanBackend::creategraphicspipeline() {
    // Pipeline setup for rendering vertices (triangles)
}

void VulkanBackend::rendermenu(VkCommandBuffer cmd, VkExtent2D extent, VkImage targetImage, VkImageView targetView) {
    const auto& vertices = Drawing::get_vertices();
    if (vertices.empty()) return;

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &targetView;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    VkFramebuffer framebuffer;
    vkCreateFramebuffer(ctx.device, &framebufferInfo, nullptr, &framebuffer);

    renderPassInfo.framebuffer = framebuffer;

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // 1. Bind Graphics Pipeline
    // vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    // 2. Upload/Bind Vertices and Draw
    // For now, we provide the vertices to the backend.
    // You would typically use a staging buffer or push constants for small UI data.
    vkCmdDraw(cmd, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

    vkCmdEndRenderPass(cmd);
    vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
}

void VulkanBackend::cleanup() {
    vkDestroyRenderPass(ctx.device, renderPass, nullptr);
}

}
