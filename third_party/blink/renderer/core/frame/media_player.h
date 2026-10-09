

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_MEDIA_PLAYER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_MEDIA_PLAYER_H_

#include "base/android/jni_android.h"
#include <string>
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "v8/include/v8.h"
namespace blink {
	class MediaPlayer : public ScriptWrappable{
		DEFINE_WRAPPERTYPEINFO();
		public:
			~MediaPlayer() override = default;
		    static MediaPlayer* Create(); 
		    static MediaPlayer* Create(const ExceptionState& state); 
		    static MediaPlayer* Create(const ExecutionContext* context, const ExceptionState& state); 
		    static MediaPlayer* Create(const AtomicString& type);
		    static MediaPlayer* Create(const ExecutionContext* context, const AtomicString& type, const ExceptionState& state); 
		    static MediaPlayer* Create(const AtomicString& type, const ExceptionState& state); 
		    MediaPlayer();
		    MediaPlayer(const AtomicString& type); 

		    int getNativePlayerInstanceID(void) const;
    		int prepare(const int& iFlag);
    		int prepare(void) const;
		    void setSingleOrPlaylistMode(const int& mode);
		    int getSingleOrPlaylistMode(void) const;
		    void setVideoDisplayMode(const int& videoDisplayMode);
		    int getVideoDisplayMode(void) const;
		    void setVideoZoomMode(const int& videoZoomMode);
    		int getVideoZoomMode(void) const;
		    void setVideoDisplayArea(const int& left,const int& top,const int& width,const int& height);
			int getVideoDisplayLeft(void) const;
		    int getVideoDisplayTop(void) const;
		    int getVideoDisplayWidth(void) const;
		    int getVideoDisplayHeight(void) const;
		    void setMuteFlag(const int& muteFlag);
		    int getMuteFlag(void) const;
		    void setMacrovisionFlag(const int& macrovisionFlag);
		    void setHDCPFlag(const int& HDCPFlag);
		    void setCGMSAFlag(const int& CGMSAFlag);
		    void setTimeShiftLength(const int& bufferLength);
		    void setCCFlag(const int& CCFlag);
    		int getCCFlag(void) const;
    		void setIPQAMResource(const int& IPQAMResource);
		    void clearIPQAMResource(const int& clearFlag);
		    int setCapability (const String& capabilityString);
		    String getCapability(const String& index);
		    int enableSQMMonitor(void) const;
		    int disableSQMMonitor(void) const;
		    void setVRFlag(const int& VRFlag);

		    void setNativeUIFlag(const int& nativeUIFlag);
		    int getNativeUIFlag(void) const;
		    void setMuteUIFlag(const int& muteUIFlag);
		    int getMuteUIFlag(void) const;
		    void setAudioVolumeUIFlag(const int& audioVolumeUIFlag);
		    int getAudioVolumeUIFlag(void) const;
		    void setAudioTrackUIFlag(const int& audioTrackUIFlag);
		    int getAudioTrackUIFlag(void) const;
		    void setProgressBarUIFlag(const int& progressBarUIFlag);
		    int getProgressBarUIFlag(void) const;
		    void setChannelNoUIFlag(const int& channelNoUIFlag);
		    int getChannelNoUIFlag(void) const;
		    void setSubtitileFlag(const int& subtitileFlag);
		    int getSubtitileFlag(void) const;
		    void setVideoAlpha(const int& videoAlpha);
		    int getVideoAlpha(void) const;
		    void setAllowTrickmodeFlag(const int& allowTrickmodeFlag);
		    int getAllowTrickmodeFlag(void) const;
		    void setCycleFlag(const int& cycleFlag);
		    int getCycleFlag(void) const;
		    void setRandomFlag(const int& randomFlag);
		    int getRandomFlag(void) const;
		    void setVendorSpecificAttr(const String& vendorSpecificAttr);
		    String getVendorSpecificAttr(void) const;
		    String getMediaCode(void) const;
		    int getMediaDuration(void) const;
		    String getCurrentPlayTime(void) const;
		    String getPlaybackMode(void) const;
		    int getChannelNum(void) const;
                    String getCurrentAudioChannel(void) const;
		    String getAudioTrack(void) const;
		    String getSubtitle(void) const;
		    int getMediaCount(void) const;
		    int getCurrentIndex(void) const;
		    String getEntryID(void) const;
		    String getPlaylist(void) const;
		    int bindNativePlayerInstance(const int& nativePlayerInstanceID);
		    void initMediaPlayer(const int& nativePlayerinstanceID,const int& playlistFlag,const int& videoDisplayMode,const int& height,const int& width,const int& left,const int& top,const int& muteFlag,const int& useNativeUIFlag,const int& subtitleFlag,const int& videoAlpha,const int& cycleFlag,const int& randomFlag);
		    void initMediaPlayer(const int& nativePlayerinstanceID,const int& playlistFlag,const int& videoDisplayMode,const int& height,const int& width,const int& left,const int& top,const int& muteFlag,const int& useNativeUIFlag,const int& subtitleFlag,const int& videoAlpha,const int& cycleFlag,const int& randomFlag,const int& autoDelFlag);
		    int releaseMediaPlayer(const int& nativePlayerInstanceID);
		    void setSingleMedia(const String& mediaStr);
		    int joinChannel(const int& userchannelid);
		    int leaveChannel(void) const;
		    void addBatchMedia(const String& batchMediaStr);
		    void clearAllMedia(void) const;
		    void moveMediaByIndex(const int& entryID,const int& toIndex);
		    void moveMediaByOffset(const int& entryID,const int& offset);
		    void moveMediaByIndex1(const int& index,const int& toIndex);
		    void moveMediaByOffset1(const int& index,const int& offset);
		    void moveMediaToNext(const int& entryID);
		    void moveMediaToPrevious(const int& entryID);
		    void moveMediaToFirst(const int& entryID);
		    void moveMediaToLast(const int& entryID);
		    void moveMediaToNext1(const int& index);
		    void moveMediaToPrevious1(const int& index);
		    void moveMediaToFirst1(const int& index);
		    void moveMediaToLast1(const int& index);
		    void selectMediaByIndex(const int& index);
		    void selectMediaByOffset(const int& offset);
		    void selectNext(void) const;
		    void selectPrevious(void) const;
		    void selectFirst(void) const;
		    void selectLast(void) const;
		    void SelectMediaByEntryID(const int& entryID);
    		void selectAudio(const String& param);
		    void playFromStart(void) const;
		    void playByTime(const int& type,const String& timestamp,const int& speed);
		    void playByTime(const int& type,const String& timestamp);
		    void playByEventId(const int& type,const String& timestamp,const int& eventId);
		    void startMediaChange(void) const;
		    int prepareByTime(const int& type,const String& timestamp);
		    void pause(void) const;
		    void fastForward(const int& speed);
		    void fastRewind(const int& speed);
		    void resume(void) const;
		    void gotoEnd(void) const;
		    void gotoStart(void) const;
		    void stop(void) const;
		    void refreshVideoDisplay(void) const;
		    void switchAudioChannel(void) const;
		    void switchAudioChannel(const String& type);
		    String checkTeletextAvailability(void) const;
		    void teletextSurfaceSwitch(const int& teletextSwitch);
		    void switchAudioTrack(void) const;
		    void switchSubtitle(void) const;
		    void sendVendorSpecificCommand(const String& xml);
		    int getAudioPID(void) const;
		    String getAudioPIDs(void) const;
		    void setAudioPID(void) const;
		    int getSubtitlePID(void) const;
		    String getSubtitlePIDs(void) const;
		    void setSubtitlePID(void) const;
		    void setVolume(const int& intvolume);
		    int getVolume(void) const;
			String getAllVideoTrackInfo(void) const;
			String getCurrentVideoTrackInfo(void) const;
			String getAllAudioTrackInfo(void) const;
			String getAllSubtitleInfo(void) const;
			String getAllCCInfo(void) const;
			String getCurrentAudioTrackInfo(void) const;
			String getCurrentSubtitleInfo(void) const;
			String getCurrentCCInfo(void) const;
			void selectSubtitle(const int& pid);
			void selectDefaultAudio(const int& pid);
			void selectCC(const int& serviceNumber);
			String getBufferInfo(void) const;
			int setVideoOutput(const int& value);
			int setMaxBandwidth(const int& value);
			void setVRRotate(const int& direction,const String& degree,const int& flag);
			void clearScreen(void) const;
		    String get(const String& ioStr);
		    void set(const String& ioStr,const String& wrStr);
		    void addSingleMedia(const int& index,const String& mediaStr);
		    void release(void) const;
		    int getUuid(){
		    	return uuid;
		    }
		private:
		    int uuid = 0;
	};
}
#endif // MediaPlayer_H

