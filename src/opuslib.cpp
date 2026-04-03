#include "opuslib.hpp"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"
#include "opus/opus.h"
#include "opus_defines.h"

void OpusLib::_bind_methods() {
	// No methods to bind

	godot::ClassDB::bind_static_method("OpusLib", godot::D_METHOD("str_error", "err"), &OpusLib::str_error);
	godot::ClassDB::bind_static_method("OpusLib", godot::D_METHOD("convert_signed_pcm", "pcm"), &OpusLib::convert_signed_pcm);
	godot::ClassDB::bind_static_method("OpusLib", godot::D_METHOD("convert_unsigned_pcm", "pcm"), &OpusLib::convert_unsigned_pcm);
}

godot::String OpusLib::str_error(int err) {
	return godot::String(opus_strerror(err));
}

godot::PackedByteArray OpusLib::convert_signed_pcm(const godot::PackedVector2Array &pcm) {
	godot::PackedByteArray arr;

	arr.resize(pcm.size() * 2 * sizeof(int16_t)); // 2 bytes per sample (2 channels * 2 bytes per sample)

	uint8_t *data = arr.ptrw();

	for (size_t i = 0; i < pcm.size(); i++) {
		int16_t left_sample = static_cast<int16_t>(pcm[i].x * 32767.0f);
		int16_t right_sample = static_cast<int16_t>(pcm[i].y * 32767.0f);

		memcpy(data, &left_sample, sizeof(int16_t));
		memcpy(data + sizeof(int16_t), &right_sample, sizeof(int16_t));

		data += sizeof(int16_t) * 2; // Move to the next sample (2 channels)
	}

	return arr;
}

godot::PackedVector2Array OpusLib::convert_unsigned_pcm(const godot::PackedByteArray &pcm) {
	godot::PackedVector2Array arr;

	size_t num_samples = pcm.size() / (2 * sizeof(int16_t)); // 2 bytes per sample (2 channels * 2 bytes per sample)
	arr.resize(num_samples);

	const uint8_t *data = pcm.ptr();

	for (size_t i = 0; i < num_samples; i++) {
		int16_t left_sample;
		int16_t right_sample;

        memcpy(&left_sample, data, sizeof(int16_t));
        memcpy(&right_sample, data + sizeof(int16_t), sizeof(int16_t));
        
		arr[i].x = static_cast<float>(left_sample) / 32767.0f;
		arr[i].y = static_cast<float>(right_sample) / 32767.0f;

		data += sizeof(int16_t) * 2; // Move to the next sample (2 channels)
	}

	return arr;
}
