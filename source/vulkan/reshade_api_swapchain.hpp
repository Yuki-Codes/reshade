/*
 * Copyright (C) 2014 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#pragma once

#include "runtime.hpp"

namespace reshade::vulkan
{
	class device_impl;
	class command_queue_impl;

	class swapchain_impl : public api::api_object_impl<VkSwapchainKHR, runtime>
	{
		static const uint32_t NUM_QUERY_FRAMES = 4;
		static const uint32_t MAX_IMAGE_DESCRIPTOR_SETS = 128; // TODO: Check if these limits are enough
		static const uint32_t MAX_EFFECT_DESCRIPTOR_SETS = 50 * 2 * 4; // 50 resources, 4 passes

	public:
		swapchain_impl(device_impl *device, command_queue_impl *graphics_queue);
		~swapchain_impl();

		api::device *get_device() final;
		api::command_queue *get_command_queue() final;

		void get_current_back_buffer(api::resource *out) final;
		void get_current_back_buffer_target(bool srgb, api::resource_view *out) final;

		bool on_init(VkSwapchainKHR swapchain, const VkSwapchainCreateInfoKHR &desc, HWND hwnd);
		void on_reset();
		void on_present(VkQueue queue, const uint32_t swapchain_image_index, std::vector<VkSemaphore> &wait);
		bool on_layer_submit(uint32_t eye, VkImage source, const VkExtent2D &source_extent, VkFormat source_format, VkSampleCountFlags source_samples, uint32_t source_layer_index, const float bounds[4], VkImage *target_image);

	private:
		device_impl *const _device_impl;
		command_queue_impl *const _queue_impl;
		VkQueue  _queue = VK_NULL_HANDLE;
		uint32_t _queue_sync_index = 0;
		VkSemaphore _queue_sync_semaphores[NUM_QUERY_FRAMES] = {};

		uint32_t _swap_index = 0;
		std::vector<VkImage> _swapchain_images;
		std::vector<VkImageView> _swapchain_views;
	};
}
