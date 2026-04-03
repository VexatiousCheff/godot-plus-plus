#pragma once

#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "opus/opus.h"

namespace opuslib {
class OpusDecoder : public godot::Resource {
	GDCLASS(OpusDecoder, godot::Resource);

public:
	OpusDecoder();
	~OpusDecoder() override;

	static void _bind_methods();

    int setup(int sample_rate, int channels);
    godot::PackedByteArray decode(const godot::PackedByteArray &encoded_data, int frame_size);

private:
	::OpusDecoder *decoder;
};
} //namespace opuslib
