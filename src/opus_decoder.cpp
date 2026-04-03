#include "opus_decoder.hpp"
#include "opuslib.hpp"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"

namespace opuslib {
OpusDecoder::OpusDecoder() {
    decoder = nullptr;
}

OpusDecoder::~OpusDecoder() {
    if (decoder) {
        opus_decoder_destroy(decoder);
        decoder = nullptr;
    }
}

void OpusDecoder::_bind_methods() {
    // Bind your methods here, for example:
    // ClassDB::bind_method(D_METHOD("your_method_name"), &OpusDecoder::your_method);
    godot::ClassDB::bind_method(godot::D_METHOD("setup", "sample_rate", "channels"), &OpusDecoder::setup);
    godot::ClassDB::bind_method(godot::D_METHOD("decode", "encoded_data", "frame_size"), &OpusDecoder::decode);
}

int OpusDecoder::setup(int sample_rate, int channels) {
    int err;
    decoder = opus_decoder_create(sample_rate, channels, &err);
    return err;
}

godot::PackedByteArray OpusDecoder::decode(const godot::PackedByteArray &encoded_data, int frame_size) {
    if (!decoder) {
        godot::print_error("Decoder not initialized. Call setup() first.", __FILE__, __LINE__);
        return godot::PackedByteArray();
    }

    const uint8_t *data = encoded_data.ptr();
    int data_size = encoded_data.size();

    // Allocate a buffer for the decoded PCM data
    godot::PackedByteArray pcm_data;
    pcm_data.resize(frame_size * 2 * sizeof(int16_t)); // 2 channels * 2 bytes per sample

    int16_t *pcm_buffer = reinterpret_cast<int16_t *>(pcm_data.ptrw());

    int decoded_samples = opus_decode(decoder, data, data_size, pcm_buffer, frame_size, 0);

    if (decoded_samples < 0) {
        // Handle decoding error
        godot::print_error("Decoding failed: " + OpusLib::str_error(decoded_samples), __FILE__, __LINE__);
        return godot::PackedByteArray();
    }

    // Resize the pcm_data to the actual size of the decoded PCM data
    pcm_data.resize(decoded_samples * 2 * sizeof(int16_t)); // 2 channels * 2 bytes per sample

    return pcm_data;
}

}
