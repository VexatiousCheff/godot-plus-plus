#pragma once

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/packed_vector2_array.hpp"
#include "godot_cpp/variant/string.hpp"

class OpusLib : public godot::Object {
    GDCLASS(OpusLib, godot::Object);
public:
    static void _bind_methods();

    static godot::String str_error(int err);

    static godot::PackedByteArray convert_signed_pcm(const godot::PackedVector2Array &pcm);
    static godot::PackedVector2Array convert_unsigned_pcm(const godot::PackedByteArray &pcm);
};
