#pragma once

namespace cgb
{
	/** Represents data for a vulkan ray tracing pipeline */
	class ray_tracing_pipeline_t
	{
	public:
		ray_tracing_pipeline_t() = default;
		ray_tracing_pipeline_t(ray_tracing_pipeline_t&&) noexcept = default;
		ray_tracing_pipeline_t(const ray_tracing_pipeline_t&) = delete;
		ray_tracing_pipeline_t& operator=(ray_tracing_pipeline_t&&) noexcept = default;
		ray_tracing_pipeline_t& operator=(const ray_tracing_pipeline_t&) = delete;
		~ray_tracing_pipeline_t() = default;

		std::tuple<const ray_tracing_pipeline_t*, const set_of_descriptor_set_layouts*> layout() const { return std::make_tuple(this, &mAllDescriptorSetLayouts); }
		const auto& layout_handle() const { return mPipelineLayout.get(); }
		const auto& handle() const { return mPipeline.get(); }
		auto table_entry_size() const { return mShaderGroupHandleSize; }
		const auto& shader_binding_table_handle() const { return mShaderBindingTable->buffer_handle(); }

		static owning_resource<ray_tracing_pipeline_t> create(ray_tracing_pipeline_config _Config, cgb::context_specific_function<void(ray_tracing_pipeline_t&)> _AlterConfigBeforeCreation = {});

	private:
		// TODO: What to do with flags?
		vk::PipelineCreateFlags mPipelineCreateFlags;

		// Our precious GPU shader programs:
		std::vector<shader> mShaders;
		std::vector<vk::PipelineShaderStageCreateInfo> mShaderStageCreateInfos;

		// Shader table a.k.a. shader groups:
		std::vector<vk::RayTracingShaderGroupCreateInfoNV> mShaderGroupCreateInfos;

		// Maximum recursion depth:
		uint32_t mMaxRecursionDepth;

		// TODO: What to do with the base pipeline index?
		int32_t mBasePipelineIndex;

		// Pipeline layout, i.e. resource bindings
		set_of_descriptor_set_layouts mAllDescriptorSetLayouts;
		std::vector<vk::PushConstantRange> mPushConstantRanges;
		vk::PipelineLayoutCreateInfo mPipelineLayoutCreateInfo;

		// Handles:
		vk::UniquePipelineLayout mPipelineLayout;
		vk::ResultValueType<vk::UniqueHandle<vk::Pipeline, vk::DispatchLoaderDynamic>>::type mPipeline;

		size_t mShaderGroupHandleSize;
		cgb::generic_buffer mShaderBindingTable; // TODO: support more than one shader binding table

		context_tracker<ray_tracing_pipeline_t> mTracker;
	};

	//template <>
	//void command_buffer_t::bind_descriptors<ray_tracing_pipeline_t>(std::tuple<const ray_tracing_pipeline_t*, const set_of_descriptor_set_layouts*> aPipelineLayout, std::initializer_list<binding_data> aBindings, descriptor_cache_interface* aDescriptorCache)
	//{
	//	command_buffer_t::bind_descriptors(vk::PipelineBindPoint::eRayTracingNV, std::get<const ray_tracing_pipeline_t*>(aPipelineLayout)->layout_handle(), std::move(aBindings), aDescriptorCache);
	//}

	using ray_tracing_pipeline = owning_resource<ray_tracing_pipeline_t>;

}
