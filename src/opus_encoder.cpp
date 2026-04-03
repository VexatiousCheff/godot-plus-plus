#include "opus_encoder.hpp"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "opus.h"
#include "opuslib.hpp"

namespace opuslib {
OpusEncoder::OpusEncoder() {
	encoder = nullptr;
}

OpusEncoder::~OpusEncoder() {
	if (encoder) {
		opus_encoder_destroy(encoder);
		encoder = nullptr;
	}
}

void OpusEncoder::_bind_methods() {
	godot::ClassDB::bind_method(godot::D_METHOD("setup", "sample_rate", "channels"), &OpusEncoder::setup);
	godot::ClassDB::bind_method(godot::D_METHOD("set_bitrate", "bitrate"), &OpusEncoder::set_bitrate);
	godot::ClassDB::bind_method(godot::D_METHOD("encode", "pcm", "frame_size", "max_data_bytes"), &OpusEncoder::encode);
}

int OpusEncoder::setup(int sample_rate, int channels) {
	int err;
	encoder = opus_encoder_create(sample_rate, channels, OPUS_APPLICATION_AUDIO, &err);
	return err;
}

int OpusEncoder::set_bitrate(int bitrate) {
	if (!encoder) {
		return OPUS_BAD_ARG;
	}
	return opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));
}

godot::PackedByteArray OpusEncoder::encode(const godot::PackedByteArray &pcm, int frame_size, int max_data_bytes) {
	if (!encoder) {
        godot::print_error("Encoder not initialized. Call setup() first.", __FILE__, __LINE__);
		return godot::PackedByteArray();
	}

	// pcm is PackedVector2Array converted to PackedByteArray of int16_t samples
	const int16_t *pcm_data = reinterpret_cast<const int16_t *>(pcm.ptr());

	// Allocate a buffer for the encoded data
	godot::PackedByteArray encoded_data;

	encoded_data.resize(max_data_bytes); // Adjust size as needed for your application

	uint8_t *data = encoded_data.ptrw();

	int encoded_bytes = opus_encode(encoder, pcm_data, frame_size, data, max_data_bytes);

	if (encoded_bytes < 0) {
        godot::print_error("Encoding failed: " + OpusLib::str_error(encoded_bytes), __FILE__, __LINE__);
		return godot::PackedByteArray();
	}

	// Resize the encoded_data to the actual size of the encoded data
	encoded_data.resize(encoded_bytes);

	return encoded_data;
}

} //namespace opuslib
