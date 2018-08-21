#pragma once



// Helper class for callbacks from DirectXTK
class IVoiceNotify
{
public:
	virtual ~IVoiceNotify() = default;

	IVoiceNotify(const IVoiceNotify&) = delete;
	IVoiceNotify& operator=(const IVoiceNotify&) = delete;

	IVoiceNotify(IVoiceNotify&&) = delete;
	IVoiceNotify& operator=(IVoiceNotify&&) = delete;

	virtual void __cdecl OnBufferEnd() = 0;
	// Notfication that a voice buffer has finished
	// Note this is called from XAudio2's worker thread, so it should perform very minimal and thread-safe operations

	virtual void __cdecl OnCriticalError() = 0;
	// Notification that the audio engine encountered a critical error

	virtual void __cdecl OnReset() = 0;
	// Notification of an audio engine reset

	virtual void __cdecl OnUpdate() = 0;
	// Notification of an audio engine per-frame update (opt-in)

	virtual void __cdecl OnDestroyEngine() = 0;
	// Notification that the audio engine is being destroyed

	virtual void __cdecl OnTrim() = 0;
	// Notification of a request to trim the voice pool


protected:
	IVoiceNotify() = default;
};