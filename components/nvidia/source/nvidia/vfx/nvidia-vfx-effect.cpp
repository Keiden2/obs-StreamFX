// AUTOGENERATED COPYRIGHT HEADER START
// Copyright (C) 2021-2023 Michael Fabian 'Xaymar' Dirks <info@xaymar.com>
// AUTOGENERATED COPYRIGHT HEADER END

#include "nvidia/vfx/nvidia-vfx-effect.hpp"
#include "obs/gs/gs-helper.hpp"
#include "util/util-logging.hpp"

#include "warning-disable.hpp"
#include <string_view>
#include "warning-enable.hpp"

#ifdef _DEBUG
#define ST_PREFIX "<%s> "
#define D_LOG_ERROR(x, ...) P_LOG_ERROR(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_WARNING(x, ...) P_LOG_WARN(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_INFO(x, ...) P_LOG_INFO(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#define D_LOG_DEBUG(x, ...) P_LOG_DEBUG(ST_PREFIX##x, __FUNCTION_SIG__, __VA_ARGS__)
#else
#define ST_PREFIX "<nvidia::vfx::feature> "
#define D_LOG_ERROR(...) P_LOG_ERROR(ST_PREFIX __VA_ARGS__)
#define D_LOG_WARNING(...) P_LOG_WARN(ST_PREFIX __VA_ARGS__)
#define D_LOG_INFO(...) P_LOG_INFO(ST_PREFIX __VA_ARGS__)
#define D_LOG_DEBUG(...) P_LOG_DEBUG(ST_PREFIX __VA_ARGS__)
#endif

using namespace ::streamfx::nvidia;

streamfx::nvidia::vfx::effect::~effect()
{
	auto gctx = ::streamfx::obs::gs::context();
	auto cctx = cuda::obs::get()->get_context()->enter();

	_fx.reset();
	_nvvfx.reset();
	_nvcvi.reset();
	_nvcuda.reset();
}

streamfx::nvidia::vfx::effect::effect(effect_t effect) : _nvcuda(cuda::obs::get()), _nvcvi(cv::cv::get()), _nvvfx(vfx::vfx::get()), _fx()
{
	auto gctx = ::streamfx::obs::gs::context();
	auto cctx = cuda::obs::get()->get_context()->enter();

	// Create the Effect/Feature.
	::vfx::handle_t handle;
	if (cv::result res = _nvvfx->NvVFX_CreateEffect(effect, &handle); res != cv::result::SUCCESS) {
		D_LOG_ERROR("Unable to create effect: %s", _nvcvi->NvCV_GetErrorStringFromCode(res));
		throw std::runtime_error("Unable to create effect.");
	}
	_fx = std::shared_ptr<void>(handle, [](::vfx::handle_t handle) { ::vfx::vfx::get()->NvVFX_DestroyEffect(handle); });

	// Assign CUDA Stream object.
	if (auto v = set_cuda_stream(PARAMETER_CUDA_STREAM, _nvcuda->get_stream()); v != cv::result::SUCCESS) {
		throw ::streamfx::nvidia::cv::exception(PARAMETER_CUDA_STREAM, v);
	}

	// Assign Model Directory.
	_model_path = _nvvfx->model_path().generic_u8string();
	if (auto v = set_string(PARAMETER_MODEL_DIRECTORY, _model_path); v != cv::result::SUCCESS) {
		throw ::streamfx::nvidia::cv::exception(PARAMETER_MODEL_DIRECTORY, v);
	}
}

cv::result streamfx::nvidia::vfx::effect::get_string(parameter_t param, std::string_view& value)
{
	const char* cvalue = nullptr;
	cv::result  res    = get_string(param, cvalue);
	if (res == cv::result::SUCCESS) {
		if (cvalue) {
			value = std::string_view(cvalue);
		} else {
			value = std::string_view();
		}
	}
	return res;
}

cv::result streamfx::nvidia::vfx::effect::get_string(parameter_t param, std::string& value)
{
	const char* cvalue = nullptr;
	cv::result  res    = get_string(param, cvalue);
	if (res == cv::result::SUCCESS) {
		if (cvalue) {
			value = cvalue;
		} else {
			value.clear();
		}
	}
	return res;
}
