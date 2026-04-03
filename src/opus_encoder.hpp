#pragma once

#include "godot_cpp/classes/resource.hpp"

#include "godot_cpp/variant/packed_byte_array.hpp"
#include "opus/opus.h"

namespace opuslib {
class OpusEncoder : public godot::Resource {
	GDCLASS(OpusEncoder, godot::Resource);

public:
	OpusEncoder();
	~OpusEncoder() override;

	static void _bind_methods();

    int setup(int sample_rate, int channels);
    int set_bitrate(int bitrate);
    godot::PackedByteArray encode(const godot::PackedByteArray &pcm, int frame_size, int max_data_bytes);
private:
	::OpusEncoder *encoder;
};
} //namespace opuslib
