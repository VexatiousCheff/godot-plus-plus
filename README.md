# godot-opuslib

contains code for binding opus to gdscript.

can be used like so:

```
var opus_local_encoder: OpusEncoder
var opus_global_decoder: OpusDecoder
var _pcm_buffer : PackedByteArray = PackedByteArray() # persistent buffer across frames
const FRAME_SIZE : int = 960       # samples per channel (20 ms @ 48kHz)
const CHANNELS : int = 2
const SAMPLES_PER_FRAME : int = FRAME_SIZE * CHANNELS
const MAX_ENCODED_BYTES : int = 4000   # preallocated buffer for Opus


	var mic_bus = AudioServer.get_bus_index("Microphone")
	var capture = AudioServer.get_bus_effect(mic_bus, 0) as AudioEffectCapture
	var buf_size = capture.get_frames_available()
	
	if !capture.can_get_buffer(buf_size) or buf_size <= 0:
		return
	
	if buf_size <= FRAME_SIZE:
		return
	
	var _byte_size = FRAME_SIZE * CHANNELS * 2
	
	var _pcm = OpusLib.convert_signed_pcm(capture.get_buffer(buf_size))
	capture.clear_buffer()
	_pcm_buffer = _pcm_buffer + _pcm
	
	while _pcm_buffer.size() >= _byte_size:
		
		var _sample = _pcm_buffer.slice(0, _byte_size)
		_pcm_buffer = _pcm_buffer.slice(_byte_size)
	
		var bytes = opus_local_encoder.encode(_sample, FRAME_SIZE, MAX_ENCODED_BYTES)
	
		_voip_data(bytes)
```

this can then be decoded relatively the same way from the chunks:
```
	var decoded_data = opus_global_decoder.decode(data, FRAME_SIZE)
	var vdata: PackedVector2Array = OpusLib.convert_unsigned_pcm(decoded_data)
```