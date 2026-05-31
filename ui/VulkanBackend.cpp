#include "VulkanBackend.hpp"
#include <cstring>
namespace kel {


static const uint32_t vertShaderCode[] = {
    0x07230203,0x00010000,0x0008000a,0x00000021,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0008000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000b,0x00000010,
    0x00000015,0x0000001b,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00050005,0x00000009,0x6f6c5f67,0x505f6e6f,0x74697369,0x6e6f006e,0x00060005,
    0x0000000b,0x505f6e69,0x69736f6f,0x6e6f6974,0x00000000,0x00050005,0x00000010,0x61726667,
    0x6c6f4367,0x0000726f,0x00050005,0x00000015,0x436e6900,0x726f6c6f,0x00000000,0x00040005,
    0x0000001b,0x6f4c7655,0x00000063,0x00040005,0x0000001d,0x6e695556,0x00000000,0x00050048,
    0x00000009,0x00000000,0x0000000b,0x00000000,0x00030047,0x00000009,0x00000002,0x00040047,
    0x0000000b,0x0000001e,0x00000000,0x00040047,0x00000010,0x0000001e,0x00000001,0x00040047,
    0x00000015,0x0000001e,0x00000002,0x00040047,0x0000001b,0x0000001e,0x00000000,0x00040047,
    0x0000001d,0x0000001e,0x00000001,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
    0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040018,
    0x00000008,0x00000007,0x00000001,0x00040020,0x0000000a,0x00000003,0x00000008,0x0004003b,
    0x0000000a,0x00000009,0x00000003,0x00040017,0x0000000c,0x00000006,0x00000002,0x00040020,
    0x0000000d,0x00000001,0x0004003b,0x0000000d,0x0000000b,0x00000001,0x00040017,0x0000000f,
    0x00000006,0x00000004,0x00040020,0x0000011,0x00000003,0x0000000f,0x0004003b,0x0000011,
    0x00000010,0x00000003,0x0004003b,0x0000011,0x00000015,0x00000001,0x00040017,0x00000019,
    0x00000006,0x00000002,0x00040020,0x000001a,0x00000003,0x0000019,0x0004003b,0x000001a,
    0x000001b,0x00000003,0x0004003b,0x000001a,0x000001d,0x00000001,0x00040015,0x000001e,
    0x00000020,0x00000001,0x0004002b,0x000001e,0x000001f,0x00000000,0x00040020,0x00000020,
    0x00000003,0x00000007,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
    0x00000005,0x0004003d,0x0000000c,0x0000000e,0x0000000b,0x00050041,0x00000006,0x00000012,
    0x0000000e,0x0000001f,0x00050041,0x00000006,0x00000013,0x0000000e,0x00000001,0x00050051,
    0x00000006,0x00000014,0x00000012,0x00000000,0x00050051,0x00000006,0x00000015,0x00000013,
    0x00000000,0x00070050,0x00000007,0x0000016,0x0000014,0x0000015,0x0004002b,0x00000006,
    0x0000017,0x00000000,0x0004002b,0x00000006,0x0000018,0x3f800000,0x00050041,0x0000020,
    0x0000019,0x00000009,0x000001f,0x0003003e,0x0000019,0x0000016,0x0004003d,0x0000000f,
    0x000001c,0x0000015,0x0003003e,0x0000010,0x000001c,0x0004003d,0x0000019,0x000001e,
    0x000001d,0x0003003e,0x000001b,0x000001e,0x000100fd,0x00010038
};


static const uint32_t fragShaderCode[] = {
    0x07230203,0x00010000,0x00080007,0x00000014,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0005000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x00030003,0x00000002,
    0x000001c2,0x00040005,0x00000004,0x6e69616d,0x00000000,0x00050005,0x00000009,0x74756f43,
    0x6f6c6f72,0x00000000,0x00050005,0x0000000a,0x61726667,0x6c6f4367,0x0000726f,0x00040005,
    0x0000000c,0x666f6e74,0x00000000,0x00050005,0x0000010,0x61726667,0x55560000,0x00040047,
    0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000a,0x0000001e,0x00000000,0x00040047,
    0x0000000c,0x00000022,0x00000000,0x00040047,0x0000000c,0x00000021,0x00000000,0x00040047,
    0x0000010,0x0000001e,0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
    0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,
    0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040020,
    0x0000000b,0x00000001,0x00000007,0x0004003b,0x0000000b,0x0000000a,0x00000001,0x00030019,
    0x0000000d,0x00000006,0x0003001b,0x0000000e,0x0000000d,0x00040020,0x0000000f,0x00000000,
    0x0000000e,0x0004003b,0x0000000f,0x0000000c,0x00000000,0x00040017,0x00000011,0x00000006,
    0x00000002,0x00040020,0x00000012,0x00000001,0x0000011,0x0004003b,0x0000012,0x0000010,
    0x00000001,0x0004002b,0x00000006,0x0000013,0xbf800000,0x00050036,0x00000002,0x00000004,
    0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,0x0000011,0x0000015,0x0000010,
    0x00050041,0x00000006,0x0000016,0x0000015,0x00000000,0x0004003d,0x00000006,0x0000017,
    0x0000016,0x000600a7,0x00000006,0x0000018,0x0000017,0x0000013,0x00000001,0x0003003e,
    0x00000009,0x0000000a,0x000300f9,0x0000018,0x00000007,0x0004003d,0x0000000e,0x0000019,
    0x0000000c,0x0004003d,0x0000011,0x000001a,0x0000010,0x00050057,0x00000007,0x000001b,
    0x0000019,0x000001a,0x00050081,0x00000007,0x000001c,0x0000000a,0x000001b,0x0003003e,
    0x00000009,0x000001c,0x000100fa,0x00010038
};

void VulkanBackend::init(const Context& context) {
    ctx = context;
    createfonttexture();
    createdescriptors();
    createrenderpass();
    creategraphicspipeline();
    createvertexbuffer(4096 * sizeof(Vertex));
}

void VulkanBackend::createfonttexture() {
    VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.extent = {128, 128, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vkCreateImage(ctx.device, &imageInfo, nullptr, &fontImage);

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(ctx.device, fontImage, &memReqs);
    VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, memReqs.size, findmemorytype(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)};
    vkAllocateMemory(ctx.device, &allocInfo, nullptr, &fontImageMemory);
    vkBindImageMemory(ctx.device, fontImage, fontImageMemory, 0);

    VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0, fontImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, {}, {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};
    vkCreateImageView(ctx.device, &viewInfo, nullptr, &fontImageView);

    VkSamplerCreateInfo samplerInfo{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE};
    vkCreateSampler(ctx.device, &samplerInfo, nullptr, &fontSampler);
}

void VulkanBackend::createdescriptors() {
    VkDescriptorSetLayoutBinding binding{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    VkDescriptorSetLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr, 0, 1, &binding};
    vkCreateDescriptorSetLayout(ctx.device, &layoutInfo, nullptr, &descriptorSetLayout);

    VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1};
    VkDescriptorPoolCreateInfo poolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, nullptr, 0, 1, 1, &poolSize};
    vkCreateDescriptorPool(ctx.device, &poolInfo, nullptr, &descriptorPool);

    VkDescriptorSetAllocateInfo allocInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr, descriptorPool, 1, &descriptorSetLayout};
    vkAllocateDescriptorSets(ctx.device, &allocInfo, &descriptorSet);

    VkDescriptorImageInfo imageInfo{fontSampler, fontImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, descriptorSet, 0, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &imageInfo, nullptr, nullptr};
    vkUpdateDescriptorSets(ctx.device, 1, &write, 0, nullptr);
}

void VulkanBackend::createrenderpass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = ctx.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkSubpassDescription subpass{0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, nullptr};
    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr, 0, 1, &colorAttachment, 1, &subpass, 0, nullptr};
    vkCreateRenderPass(ctx.device, &renderPassInfo, nullptr, &renderPass);
}

void VulkanBackend::creategraphicspipeline() {
    VkShaderModule vertModule = createshadermodule(vertShaderCode, sizeof(vertShaderCode));
    VkShaderModule fragModule = createshadermodule(fragShaderCode, sizeof(fragShaderCode));

    VkPipelineShaderStageCreateInfo stages[2] = {
        {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_VERTEX_BIT, vertModule, "main", nullptr},
        {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FRAGMENT_BIT, fragModule, "main", nullptr}
    };

    VkVertexInputBindingDescription binding{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX};
    VkVertexInputAttributeDescription attrs[3] = {
        {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, x)},
        {1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, u)},
        {2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, r)}
    };

    VkPipelineVertexInputStateCreateInfo vInput{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, nullptr, 0, 1, &binding, 3, attrs};
    VkPipelineInputAssemblyStateCreateInfo inputAsm{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE};
    VkPipelineViewportStateCreateInfo viewportState{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr, 0, 1, nullptr, 1, nullptr};
    VkPipelineRasterizationStateCreateInfo rasterizer{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE, VK_FALSE, 0, 0, 0, 1.0f};
    VkPipelineMultisampleStateCreateInfo multisample{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr, 0, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0, nullptr, VK_FALSE, VK_FALSE};
    VkPipelineColorBlendAttachmentState colorBlend{VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD, VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
    VkPipelineColorBlendStateCreateInfo colorBlending{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, nullptr, 0, VK_FALSE, VK_LOGIC_OP_COPY, 1, &colorBlend, {0,0,0,0}};
    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0, 2, dynamicStates};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, 0, 1, &descriptorSetLayout, 0, nullptr};
    vkCreatePipelineLayout(ctx.device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, nullptr, 0, 2, stages, &vInput, &inputAsm, nullptr, &viewportState, &rasterizer, &multisample, nullptr, &colorBlending, &dynamicState, pipelineLayout, renderPass, 0, VK_NULL_HANDLE, 0};
    vkCreateGraphicsPipelines(ctx.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

    vkDestroyShaderModule(ctx.device, vertModule, nullptr);
    vkDestroyShaderModule(ctx.device, fragModule, nullptr);
}

void VulkanBackend::createvertexbuffer(size_t size) {
    VkBufferCreateInfo bufferInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr, 0, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr};
    vkCreateBuffer(ctx.device, &bufferInfo, nullptr, &vertexBuffer);
    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(ctx.device, vertexBuffer, &memReqs);
    VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, memReqs.size, findmemorytype(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)};
    vkAllocateMemory(ctx.device, &allocInfo, nullptr, &vertexBufferMemory);
    vkBindBufferMemory(ctx.device, vertexBuffer, vertexBufferMemory, 0);
    currentVertexBufferSize = size;
}

void VulkanBackend::updatevertexbuffer(const std::vector<Vertex>& vertices) {
    size_t size = vertices.size() * sizeof(Vertex);
    if (size > currentVertexBufferSize) createvertexbuffer(size * 2);
    void* data;
    vkMapMemory(ctx.device, vertexBufferMemory, 0, size, 0, &data);
    memcpy(data, vertices.data(), size);
    vkUnmapMemory(ctx.device, vertexBufferMemory);
}

void VulkanBackend::rendermenu(VkCommandBuffer cmd, VkExtent2D extent, VkImage targetImage, VkImageView targetView) {
    const auto& vertices = Drawing::getvertices();
    if (vertices.empty()) return;
    updatevertexbuffer(vertices);

    // Image layout transition (Old layout set to UNDEFINED for safety)
    VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = targetImage;
    barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    VkFramebufferCreateInfo fbInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr, 0, renderPass, 1, &targetView, extent.width, extent.height, 1};
    VkFramebuffer fb;
    vkCreateFramebuffer(ctx.device, &fbInfo, nullptr, &fb);

    VkRenderPassBeginInfo rpBegin{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, nullptr, renderPass, fb, {{0,0}, extent}, 0, nullptr};
    vkCmdBeginRenderPass(cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport vp{0, 0, (float)extent.width, (float)extent.height, 0, 1};
    vkCmdSetViewport(cmd, 0, 1, &vp);
    VkRect2D sc{{0,0}, extent};
    vkCmdSetScissor(cmd, 0, 1, &sc);

    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer, &offset);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
    vkCmdDraw(cmd, (uint32_t)vertices.size(), 1, 0, 0);

    vkCmdEndRenderPass(cmd);
    vkDestroyFramebuffer(ctx.device, fb, nullptr);
}

void VulkanBackend::cleanup() {
    vkDestroyDescriptorPool(ctx.device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(ctx.device, descriptorSetLayout, nullptr);
    vkDestroySampler(ctx.device, fontSampler, nullptr);
    vkDestroyImageView(ctx.device, fontImageView, nullptr);
    vkDestroyImage(ctx.device, fontImage, nullptr);
    vkFreeMemory(ctx.device, fontImageMemory, nullptr);
}

uint32_t VulkanBackend::findmemorytype(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(ctx.physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
    }
    return 0;
}

VkShaderModule VulkanBackend::createshadermodule(const uint32_t* code, size_t size) {
    VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, 0, size, code};
    VkShaderModule module;
    vkCreateShaderModule(ctx.device, &createInfo, nullptr, &module);
    return module;
}

}
