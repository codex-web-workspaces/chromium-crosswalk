#include "third_party/blink/renderer/core/frame/media_player.h"
#include "base/android/jni_android.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include <algorithm>

extern jobject java_obj;
extern JavaVM* gJavaVM;
namespace blink {
    int status;
    JNIEnv *env;
    jclass interfaceClass = NULL;
    bool isAttached = false;

    void detach(){
        //env->DeleteLocalRef(interfaceClass);
        if(isAttached){
           gJavaVM->DetachCurrentThread();
           LOG(INFO) << "detach current thread";
        }
    }


    MediaPlayer* MediaPlayer::Create() {
        return MakeGarbageCollected<MediaPlayer>();
    }

    MediaPlayer* MediaPlayer::Create(const ExecutionContext* context,
                                 const ExceptionState& state) {
        return MakeGarbageCollected<MediaPlayer>();
    }
    MediaPlayer* MediaPlayer::Create(const ExceptionState& state) {
        return MakeGarbageCollected<MediaPlayer>();
    }
    MediaPlayer* MediaPlayer::Create(const AtomicString& type) {
        return MakeGarbageCollected<MediaPlayer>(type);
    }

    MediaPlayer* MediaPlayer::Create(const ExecutionContext* context,
                                 const AtomicString& type,
                                 const ExceptionState& state) {
        return MakeGarbageCollected<MediaPlayer>(type);
    }
    MediaPlayer* MediaPlayer::Create(const AtomicString& type,
                                 const ExceptionState& state) {
        return MakeGarbageCollected<MediaPlayer>(type);
    }


	MediaPlayer::MediaPlayer(){
        if(interfaceClass == NULL){
        status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to get JNI environment, "
            "assuming native thread";
            status = gJavaVM->AttachCurrentThread(&env, NULL);
            if(status < 0) {
                LOG(INFO) <<"callback_handler: failed to attach "
                "current thread";
                return;
            }
            isAttached = true;
        }
        /* Construct a Java string */
        interfaceClass = env->GetObjectClass(java_obj);
        if(!interfaceClass) {
            LOG(INFO) <<"callback_handler: failed to get class reference";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        }


        jmethodID createMethod = env->GetMethodID(
        interfaceClass, "createMediaPlayer", "()I");
        if(!createMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        jint juuid = (jint) env->CallIntMethod(java_obj, createMethod);
        uuid = (int)juuid;
        detach();
	}

    MediaPlayer::MediaPlayer(const AtomicString& type){
        if(interfaceClass == NULL){
        status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to get JNI environment, "
            "assuming native thread";
            status = gJavaVM->AttachCurrentThread(&env, NULL);
            if(status < 0) {
                LOG(INFO) <<"callback_handler: failed to attach "
                "current thread";
                return;
            }
            isAttached = true;
        }
        /* Construct a Java string */
        interfaceClass = env->GetObjectClass(java_obj);
        if(!interfaceClass) {
            LOG(INFO) <<"callback_handler: failed to get class reference";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        }


        jmethodID createMethod = env->GetMethodID(
        interfaceClass, "createMediaPlayer", "(Ljava/lang/String;)I");
        if(!createMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = type.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        jint juuid = (jint) env->CallIntMethod(java_obj, createMethod,js);
        uuid = (int)juuid;
        env->DeleteLocalRef(js);
        detach();
    }


    int MediaPlayer::getNativePlayerInstanceID() const{
        LOG(INFO)<<"native getNativePlayerInstanceID:"<< uuid;
        return uuid;
        // jmethodID method = env->GetMethodID(
        // interfaceClass, "getNativePlayerInstanceID", "()I");
        // if(!method) {
        //     LOG(INFO) <<"callback_handler: failed to get method ID";
        //     if(isAttached) gJavaVM->DetachCurrentThread();
        //     return -1;
        // }
        // jint inId = (jint) env->CallIntMethod(java_obj, method);
        // return (int)inId;
    }
    
    int MediaPlayer::prepare(const int& iFlag){
        jmethodID slectNextMethod = env->GetMethodID(
        interfaceClass, "prepare", "(II)I");
        if(!slectNextMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint rVale = (jint) env->CallIntMethod(java_obj, slectNextMethod,(jint)iFlag,(jint)uuid);
        detach();
        return (int)rVale;
    }
    int MediaPlayer::prepare() const{
        jmethodID prepareMethod = env->GetMethodID(
        interfaceClass, "prepare", "(I)I");
        if(!prepareMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint rVale = (jint) env->CallIntMethod(java_obj, prepareMethod,(jint)uuid);
        detach();
        return (int)rVale;
    }
    
    void MediaPlayer::setSingleOrPlaylistMode(const int& mode){
        jmethodID singleMethod = env->GetMethodID(
        interfaceClass, "setSingleOrPlaylistMode", "(II)V");
        if(!singleMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, singleMethod,(jint)mode,(jint)uuid);
        detach();
    }
    int MediaPlayer::getSingleOrPlaylistMode() const{
        jmethodID getSinglemethod = env->GetMethodID(
        interfaceClass, "getSingleOrPlaylistMode", "(I)I");
        if(!getSinglemethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint mode = (jint) env->CallIntMethod(java_obj, getSinglemethod,(jint)uuid);
        detach();
        return (int)mode;
    }
    void MediaPlayer::setVideoDisplayMode(const int& videoDisplayMode){
        jmethodID vPlayerModemethod = env->GetMethodID(
        interfaceClass, "setVideoDisplayMode", "(II)V");
        if(!vPlayerModemethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, vPlayerModemethod,(jint)videoDisplayMode, (jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getVideoDisplayMode() const{
        jmethodID getDisPlayModemethod = env->GetMethodID(
        interfaceClass, "getVideoDisplayMode", "(I)I");
        if(!getDisPlayModemethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint mode = (jint) env->CallIntMethod(java_obj, getDisPlayModemethod, (jint)uuid);
        detach();
        return (int)mode;
    }
    void MediaPlayer::setVideoZoomMode(const int& videoZoomMode){
        jmethodID vVideoZoomModemethod = env->GetMethodID(
        interfaceClass, "setVideoZoomMode", "(II)V");
        if(!vVideoZoomModemethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, vVideoZoomModemethod,(jint)videoZoomMode, (jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getVideoZoomMode() const{
        jmethodID getZoomModemethod = env->GetMethodID(
        interfaceClass, "getVideoZoomMode", "(I)I");
        if(!getZoomModemethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint mode = (jint) env->CallIntMethod(java_obj, getZoomModemethod, (jint)uuid);
        detach();
        return (int)mode;
    }

    void MediaPlayer::setVideoDisplayArea(const int& left,const int& top,const int& width,const int& height){
        LOG(INFO) <<"setVideoDisplayArea: left>>"<<left<<";top>>"<<top<<";width>>"<<width<<";height>>"<<height;
        jmethodID areamethod = env->GetMethodID(
        interfaceClass, "setVideoDisplayArea", "(IIIII)V");
        if(!areamethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, areamethod,(jint)left,(jint)top,(jint)width,(jint)height,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getVideoDisplayLeft() const{
        jmethodID leftMthod = env->GetMethodID(
        interfaceClass, "getVideoDisplayLeft", "(I)I");
        if(!leftMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint left = (jint) env->CallIntMethod(java_obj, leftMthod,(jint)uuid);
        detach();
        return (int)left;
    }
    int MediaPlayer::getVideoDisplayTop() const{
        jmethodID topMthod = env->GetMethodID(
        interfaceClass, "getVideoDisplayTop", "(I)I");
        if(!topMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint top = (jint) env->CallIntMethod(java_obj, topMthod,(jint)uuid);
        detach();
        return (int)top;
    }
    int MediaPlayer::getVideoDisplayWidth() const{
        jmethodID widthMthod = env->GetMethodID(
        interfaceClass, "getVideoDisplayWidth", "(I)I");
        if(!widthMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint width = (jint) env->CallIntMethod(java_obj, widthMthod,(jint)uuid);
        detach();
        return (int)width;
    }
    int MediaPlayer::getVideoDisplayHeight() const{
        jmethodID heigthMthod = env->GetMethodID(
        interfaceClass, "getVideoDisplayHeight", "(I)I");
        if(!heigthMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint height = (jint) env->CallIntMethod(java_obj, heigthMthod,(jint)uuid);
        detach();
        return (int)height;
    }
    void MediaPlayer::setMuteFlag(const int& muteFlag){
        jmethodID mflatMethod = env->GetMethodID(
        interfaceClass, "setMuteFlag", "(II)V");
        if(!mflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, mflatMethod,(jint)muteFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getMuteFlag() const{
        jmethodID mflagMthod = env->GetMethodID(
        interfaceClass, "getMuteFlag", "(I)I");
        if(!mflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint mFlag = (jint) env->CallIntMethod(java_obj, mflagMthod,(jint)uuid);
        detach();
        return (int)mFlag;
    }
    void MediaPlayer::setMacrovisionFlag(const int& macrovisionFlag){
        jmethodID macrovisionMethod = env->GetMethodID(
        interfaceClass, "setMacrovisionFlag", "(II)V");
        if(!macrovisionMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, macrovisionMethod,(jint)macrovisionFlag,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setHDCPFlag(const int& HDCPFlag){
        jmethodID setHDCPFlagMethod = env->GetMethodID(
        interfaceClass, "setHDCPFlag", "(II)V");
        if(!setHDCPFlagMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setHDCPFlagMethod,(jint)HDCPFlag,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setCGMSAFlag(const int& CGMSAFlag){
        jmethodID setCGMSAFlagMethod = env->GetMethodID(
        interfaceClass, "setCGMSAFlag", "(II)V");
        if(!setCGMSAFlagMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setCGMSAFlagMethod,(jint)CGMSAFlag,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setTimeShiftLength(const int& bufferLength){
        jmethodID setTimeShiftLengthMethod = env->GetMethodID(
        interfaceClass, "setTimeShiftLength", "(II)V");
        if(!setTimeShiftLengthMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setTimeShiftLengthMethod,(jint)bufferLength,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setCCFlag(const int& CCFlag){
        jmethodID setCCFlagMethod = env->GetMethodID(
        interfaceClass, "setCCFlag", "(II)V");
        if(!setCCFlagMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setCCFlagMethod,(jint)CCFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getCCFlag() const{
        jmethodID getCCFlagMthod = env->GetMethodID(
        interfaceClass, "getCCFlag", "(I)I");
        if(!getCCFlagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint ccFlag = (jint) env->CallIntMethod(java_obj, getCCFlagMthod,(jint)uuid);
        detach();
        return (int)ccFlag;
    }


    void MediaPlayer::setIPQAMResource(const int& IPQAMResource){
        jmethodID setIPQAMResourceMethod = env->GetMethodID(
        interfaceClass, "setIPQAMResource", "(II)V");
        if(!setIPQAMResourceMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setIPQAMResourceMethod,(jint)IPQAMResource,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::clearIPQAMResource(const int& clearFlag){
        jmethodID clearIPQAMResourceMethod = env->GetMethodID(
        interfaceClass, "clearIPQAMResource", "(II)V");
        if(!clearIPQAMResourceMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, clearIPQAMResourceMethod,(jint)clearFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::setCapability (const String& capabilityString){
        jmethodID setCapabilityMethod = env->GetMethodID(
        interfaceClass, "setCapability", "(Ljava/lang/String;I)I");
        if(!setCapabilityMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return 0;
        }
        std::string utf8Str = capabilityString.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        jint reint = (jint)env->CallIntMethod(java_obj, setCapabilityMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return (int)reint;
    }
    String MediaPlayer::getCapability(const String& index){
        jmethodID getCapabilityMthod = env->GetMethodID(
        interfaceClass, "getCapability", "(Ljava/lang/String;I)Ljava/lang/String;");
        if(!getCapabilityMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        std::string utf8Str = index.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        jstring gvsa = (jstring) env->CallObjectMethod(java_obj, getCapabilityMthod,js,(jint)uuid);
        const char *str = env->GetStringUTFChars(gvsa, NULL);
        env->ReleaseStringUTFChars(gvsa, str);
        env->DeleteLocalRef(js);
        env->DeleteLocalRef(gvsa);
        detach();
        return (String)str;
    }
    int MediaPlayer::enableSQMMonitor() const{
        jmethodID enableSQMMonitorMthod = env->GetMethodID(
        interfaceClass, "enableSQMMonitor", "(I)I");
        if(!enableSQMMonitorMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint enableFlat = (jint) env->CallIntMethod(java_obj, enableSQMMonitorMthod,(jint)uuid);
        detach();
        return (int)enableFlat;
    }
    int MediaPlayer::disableSQMMonitor() const{
        jmethodID disableSQMMonitorMthod = env->GetMethodID(
        interfaceClass, "disableSQMMonitor", "(I)I");
        if(!disableSQMMonitorMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint disableFlat = (jint) env->CallIntMethod(java_obj, disableSQMMonitorMthod,(jint)uuid);
        detach();
        return (int)disableFlat;
    }
    void MediaPlayer::setVRFlag(const int& VRFlag){
        jmethodID setVRFlagMethod = env->GetMethodID(
        interfaceClass, "setVRFlag", "(II)V");
        if(!setVRFlagMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setVRFlagMethod,(jint)VRFlag,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setNativeUIFlag(const int& nativeUIFlag){
        jmethodID uiflatMethod = env->GetMethodID(
        interfaceClass, "setNativeUIFlag", "(II)V");
        if(!uiflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, uiflatMethod,(jint)nativeUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getNativeUIFlag() const{
        jmethodID nuflagMthod = env->GetMethodID(
        interfaceClass, "getNativeUIFlag", "(I)I");
        if(!nuflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint nuFlag = (jint) env->CallIntMethod(java_obj, nuflagMthod,(jint)uuid);
        detach();
        return (int)nuFlag;
    }
    void MediaPlayer::setMuteUIFlag(const int& muteUIFlag){
        jmethodID muiflatMethod = env->GetMethodID(
        interfaceClass, "setMuteUIFlag", "(II)V");
        if(!muiflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, muiflatMethod,(jint)muteUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getMuteUIFlag() const{
        jmethodID muflagMthod = env->GetMethodID(
        interfaceClass, "getMuteUIFlag", "(I)I");
        if(!muflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint muFlag = (jint) env->CallIntMethod(java_obj, muflagMthod,(jint)uuid);
        detach();
        return (int)muFlag;
    }
    void MediaPlayer::setAudioVolumeUIFlag(const int& audioVolumeUIFlag){
        jmethodID auvflatMethod = env->GetMethodID(
        interfaceClass, "setAudioVolumeUIFlag", "(II)V");
        if(!auvflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, auvflatMethod,(jint)audioVolumeUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getAudioVolumeUIFlag() const{
        jmethodID avflagMthod = env->GetMethodID(
        interfaceClass, "getAudioVolumeUIFlag", "(I)I");
        if(!avflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint avFlag = (jint) env->CallIntMethod(java_obj, avflagMthod,(jint)uuid);
        detach();
        return (int)avFlag;
    }
    void MediaPlayer::setAudioTrackUIFlag(const int& audioTrackUIFlag){
        jmethodID atuflatMethod = env->GetMethodID(
        interfaceClass, "setAudioTrackUIFlag", "(II)V");
        if(!atuflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, atuflatMethod,(jint)audioTrackUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getAudioTrackUIFlag() const{
        jmethodID autuflagMthod = env->GetMethodID(
        interfaceClass, "getAudioTrackUIFlag", "(I)I");
        if(!autuflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint autuFlag = (jint) env->CallIntMethod(java_obj, autuflagMthod,(jint)uuid);
        detach();
        return (int)autuFlag;
    }
    void MediaPlayer::setProgressBarUIFlag(const int& progressBarUIFlag){
        jmethodID pbuflatMethod = env->GetMethodID(
        interfaceClass, "setProgressBarUIFlag", "(II)V");
        if(!pbuflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, pbuflatMethod,(jint)progressBarUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getProgressBarUIFlag() const{
        jmethodID pbuiflagMthod = env->GetMethodID(
        interfaceClass, "getProgressBarUIFlag", "(I)I");
        if(!pbuiflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint pbuFlag = (jint) env->CallIntMethod(java_obj, pbuiflagMthod,(jint)uuid);
        detach();
        return (int)pbuFlag;
    }
    void MediaPlayer::setChannelNoUIFlag(const int& channelNoUIFlag){
        jmethodID cnuflatMethod = env->GetMethodID(
        interfaceClass, "setChannelNoUIFlag", "(II)V");
        if(!cnuflatMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, cnuflatMethod,(jint)channelNoUIFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getChannelNoUIFlag() const{
        jmethodID cnouiflagMthod = env->GetMethodID(
        interfaceClass, "getChannelNoUIFlag", "(I)I");
        if(!cnouiflagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint cnouiFlag = (jint) env->CallIntMethod(java_obj, cnouiflagMthod,(jint)uuid);
        detach();
        return (int)cnouiFlag;
    }
    void MediaPlayer::setSubtitileFlag(const int& subtitileFlag){
        jmethodID stFlagMethod = env->GetMethodID(
        interfaceClass, "setSubtitileFlag", "(II)V");
        if(!stFlagMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, stFlagMethod,(jint)subtitileFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getSubtitileFlag() const{
        jmethodID subtFlagMthod = env->GetMethodID(
        interfaceClass, "getSubtitileFlag", "(I)I");
        if(!subtFlagMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint subtFlag = (jint) env->CallIntMethod(java_obj, subtFlagMthod,(jint)uuid);
        detach();
        return (int)subtFlag;
    }
    void MediaPlayer::setVideoAlpha(const int& videoAlpha){
        jmethodID vAlphaMethod = env->GetMethodID(
        interfaceClass, "setVideoAlpha", "(II)V");
        if(!vAlphaMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, vAlphaMethod,(jint)videoAlpha,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getVideoAlpha() const{
        jmethodID videoAMthod = env->GetMethodID(
        interfaceClass, "getVideoAlpha", "(I)I");
        if(!videoAMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint subtFlag = (jint) env->CallIntMethod(java_obj, videoAMthod,(jint)uuid);
        detach();
        return (int)subtFlag;
    }
    void MediaPlayer::setAllowTrickmodeFlag(const int& allowTrickmodeFlag){
        jmethodID atmMethod = env->GetMethodID(
        interfaceClass, "setAllowTrickmodeFlag", "(II)V");
        if(!atmMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, atmMethod,(jint)allowTrickmodeFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getAllowTrickmodeFlag() const{
        jmethodID atmMthod = env->GetMethodID(
        interfaceClass, "getAllowTrickmodeFlag", "(I)I");
        if(!atmMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint atmFlag = (jint) env->CallIntMethod(java_obj, atmMthod,(jint)uuid);
        detach();
        return (int)atmFlag;
    }
    void MediaPlayer::setCycleFlag(const int& cycleFlag){
        jmethodID scfMethod = env->GetMethodID(
        interfaceClass, "setCycleFlag", "(II)V");
        if(!scfMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, scfMethod,(jint)cycleFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getCycleFlag() const{
        jmethodID gcfMthod = env->GetMethodID(
        interfaceClass, "getCycleFlag", "(I)I");
        if(!gcfMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint getcFlag = (jint) env->CallIntMethod(java_obj, gcfMthod,(jint)uuid);
        detach();
        return (int)getcFlag;
    }
    void MediaPlayer::setRandomFlag(const int& randomFlag){
        jmethodID srdfMethod = env->GetMethodID(
        interfaceClass, "setRandomFlag", "(II)V");
        if(!srdfMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, srdfMethod,(jint)randomFlag,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getRandomFlag() const{
        jmethodID grdMthod = env->GetMethodID(
        interfaceClass, "getRandomFlag", "(I)I");
        if(!grdMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint grdFlag = (jint) env->CallIntMethod(java_obj, grdMthod,(jint)uuid);
        detach();
        return (int)grdFlag;
    }
    void MediaPlayer::setVendorSpecificAttr(const String& vendorSpecificAttr){
        jmethodID svsaMethod = env->GetMethodID(
        interfaceClass, "setVendorSpecificAttr", "(Ljava/lang/String;I)V");
        if(!svsaMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = vendorSpecificAttr.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, svsaMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    String MediaPlayer::getVendorSpecificAttr() const{
        jmethodID gvsaMthod = env->GetMethodID(
        interfaceClass, "getVendorSpecificAttr", "(I)Ljava/lang/String;");
        if(!gvsaMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring gvsa = (jstring) env->CallObjectMethod(java_obj, gvsaMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(gvsa, NULL);
        env->ReleaseStringUTFChars(gvsa, str);
        env->DeleteLocalRef(gvsa);
        detach();
        return (String)str;
    }
    String MediaPlayer::getMediaCode() const{
        jmethodID gmCodeMthod = env->GetMethodID(
        interfaceClass, "getMediaCode", "(I)Ljava/lang/String;");
        if(!gmCodeMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring gmCode = (jstring) env->CallObjectMethod(java_obj, gmCodeMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(gmCode, NULL);
        env->ReleaseStringUTFChars(gmCode, str);
        env->DeleteLocalRef(gmCode);
        detach();
        return (String)str;
    }
    int MediaPlayer::getMediaDuration() const{
        jmethodID gmDurMthod = env->GetMethodID(
        interfaceClass, "getMediaDuration", "(I)I");
        if(!gmDurMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return 0;
        }
        jint gmDur = env->CallIntMethod(java_obj, gmDurMthod,(jint)uuid);
        detach();
        return (int)gmDur;
    }
    String MediaPlayer::getCurrentPlayTime() const{
        jmethodID gcpTimeMthod = env->GetMethodID(
        interfaceClass, "getCurrentPlayTime", "(I)Ljava/lang/String;");
        if(!gcpTimeMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring gcpTime = (jstring) env->CallObjectMethod(java_obj, gcpTimeMthod,(jint)uuid);
        const char *str = (char *)env->GetStringUTFChars(gcpTime, NULL);
        String returnStr = str;
        env->ReleaseStringUTFChars(gcpTime, str);
        env->DeleteLocalRef(gcpTime);
        detach();
        LOG(INFO) << "Xwalk-MediaPlayer-CurrentPlayTime+++:" <<  returnStr;
        return returnStr;
    }

    String MediaPlayer::getPlaybackMode() const{
        jmethodID gpbModeMthod = env->GetMethodID(
        interfaceClass, "getPlaybackMode", "(I)Ljava/lang/String;");
        if(!gpbModeMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring gpbMode = (jstring) env->CallObjectMethod(java_obj, gpbModeMthod,(jint)uuid);
        const char *str = (char *)env->GetStringUTFChars(gpbMode, NULL);
        env->ReleaseStringUTFChars(gpbMode, str);
        env->DeleteLocalRef(gpbMode);
        detach();
        return (String)str;
    }
    int MediaPlayer::getChannelNum() const{
        jmethodID gchannelNumMthod = env->GetMethodID(
        interfaceClass, "getChannelNum", "(I)I");
        if(!gchannelNumMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint gchannelNum = (jint) env->CallIntMethod(java_obj, gchannelNumMthod,(jint)uuid);
        detach();
        return (int)gchannelNum;
    }
    String MediaPlayer::getCurrentAudioChannel() const{
        jmethodID getcacMthod = env->GetMethodID(
        interfaceClass, "getCurrentAudioChannel", "(I)Ljava/lang/String;");
        if(!getcacMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring cac = (jstring) env->CallObjectMethod(java_obj, getcacMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(cac, NULL);
        env->ReleaseStringUTFChars(cac, str);
        env->DeleteLocalRef(cac);
        detach();
        return (String)str;
    }
    String MediaPlayer::getAudioTrack() const{
        jmethodID getATMthod = env->GetMethodID(
        interfaceClass, "getAudioTrack", "(I)Ljava/lang/String;");
        if(!getATMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring audioTrack = (jstring) env->CallObjectMethod(java_obj, getATMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(audioTrack, NULL);
        env->ReleaseStringUTFChars(audioTrack, str);
        env->DeleteLocalRef(audioTrack);
        detach();
        return (String)str;
    }
    String MediaPlayer::getSubtitle() const{
        jmethodID getSubtitMthod = env->GetMethodID(
        interfaceClass, "getSubtitle", "(I)Ljava/lang/String;");
        if(!getSubtitMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring subTitle = (jstring) env->CallObjectMethod(java_obj, getSubtitMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(subTitle, NULL);
        env->ReleaseStringUTFChars(subTitle, str);
        env->DeleteLocalRef(subTitle);
        detach();
        return (String)str;
    }
    int MediaPlayer::getMediaCount() const{
        jmethodID gMCountMthod = env->GetMethodID(
        interfaceClass, "getMediaCount", "(I)I");
        if(!gMCountMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint mediaCount = (jint) env->CallIntMethod(java_obj, gMCountMthod,(jint)uuid);
        detach();
        return (int)mediaCount;
    }
    int MediaPlayer::getCurrentIndex() const{
        jmethodID gcIndexMthod = env->GetMethodID(
        interfaceClass, "getCurrentIndex", "(I)I");
        if(!gcIndexMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint currentIndex = (jint) env->CallIntMethod(java_obj, gcIndexMthod,(jint)uuid);
        detach();
        return (int)currentIndex;
    }
    String MediaPlayer::getEntryID() const{
        jmethodID getEntryIdtMthod = env->GetMethodID(
        interfaceClass, "getEntryID", "(I)Ljava/lang/String;");
        if(!getEntryIdtMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring entryId = (jstring) env->CallObjectMethod(java_obj, getEntryIdtMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(entryId, NULL);
        env->ReleaseStringUTFChars(entryId, str);
        env->DeleteLocalRef(entryId);
        detach();
        return (String)str;
    }
    String MediaPlayer::getPlaylist() const{
        jmethodID getPlayListMthod = env->GetMethodID(
        interfaceClass, "getPlaylist", "(I)Ljava/lang/String;");
        if(!getPlayListMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring playList = (jstring) env->CallObjectMethod(java_obj, getPlayListMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(playList, NULL);
        env->ReleaseStringUTFChars(playList, str);
        env->DeleteLocalRef(playList);
        detach();
        return (String)str;
    }
    int MediaPlayer::bindNativePlayerInstance(const int& nativePlayerInstanceID){
        jmethodID bindMthod = env->GetMethodID(
        interfaceClass, "bindNativePlayerInstance", "(I)I");
        if(!bindMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        uuid = nativePlayerInstanceID;
        jint bindStatus = (jint) env->CallIntMethod(java_obj, bindMthod,nativePlayerInstanceID);
        detach();
        return (int)bindStatus;
    }
    void MediaPlayer::initMediaPlayer(const int& nativePlayerinstanceID,const int& playlistFlag,const int& videoDisplayMode,const int& height,const int& width,const int& left,const int& top,const int& muteFlag,const int& useNativeUIFlag,const int& subtitleFlag,const int& videoAlpha,const int& cycleFlag,const int& randomFlag){
        jmethodID initMethod = env->GetMethodID(
        interfaceClass, "initMediaPlayer", "(IIIIIIIIIIIIII)V");
        if(!initMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, initMethod,(jint)nativePlayerinstanceID,(jint)playlistFlag,(jint)videoDisplayMode,(jint)height,(jint)width,(jint)left,(jint)top,(jint)muteFlag,(jint)useNativeUIFlag,(jint)subtitleFlag,(jint)videoAlpha,(jint)cycleFlag,(jint)randomFlag,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::initMediaPlayer(const int& nativePlayerinstanceID,const int& playlistFlag,const int& videoDisplayMode,const int& height,const int& width,const int& left,const int& top,const int& muteFlag,const int& useNativeUIFlag,const int& subtitleFlag,const int& videoAlpha,const int& cycleFlag,const int& randomFlag,const int& autoDelFlag){
        jmethodID initMethod = env->GetMethodID(interfaceClass, "initMediaPlayer", "(IIIIIIIIIIIIIII)V");
        if(!initMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, initMethod,(jint)nativePlayerinstanceID,(jint)playlistFlag,(jint)videoDisplayMode,(jint)height,(jint)width,(jint)left,(jint)top,(jint)muteFlag,(jint)useNativeUIFlag,(jint)subtitleFlag,(jint)videoAlpha,(jint)cycleFlag,(jint)randomFlag,(jint)autoDelFlag,(jint)uuid);
        detach();
        return;
        
    }
    int MediaPlayer::releaseMediaPlayer(const int& nativePlayerInstanceID){
        jmethodID releaseMthod = env->GetMethodID(
        interfaceClass, "releaseMediaPlayer", "(II)I");
        if(!releaseMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint releaseStatus = (jint) env->CallIntMethod(java_obj, releaseMthod,(jint)nativePlayerInstanceID,(jint)uuid);
        detach();
        return (int)releaseStatus;
    }
    void MediaPlayer::setSingleMedia(const String& mediaStr){
        jmethodID setMediaMethod = env->GetMethodID(
        interfaceClass, "setSingleMedia", "(Ljava/lang/String;I)V");
        if(!setMediaMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = mediaStr.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, setMediaMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    int MediaPlayer::joinChannel(const int& userchannelid){
        jmethodID joinChannelMthod = env->GetMethodID(
        interfaceClass, "joinChannel", "(II)I");
        if(!joinChannelMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint joinStatus = (jint) env->CallIntMethod(java_obj, joinChannelMthod,userchannelid,(jint)uuid);
        detach();
        return (int)joinStatus;
    }
    int MediaPlayer::leaveChannel() const{
        jmethodID leaveChannelMthod = env->GetMethodID(
        interfaceClass, "leaveChannel", "(I)I");
        if(!leaveChannelMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint leaveStatus = (jint) env->CallIntMethod(java_obj, leaveChannelMthod,(jint)uuid);
        detach();
        return (int)leaveStatus;
    }
    
    void MediaPlayer::addBatchMedia(const String& batchMediaStr){
        jmethodID addBatchMethod = env->GetMethodID(
        interfaceClass, "addBatchMedia", "(Ljava/lang/String;I)V");
        if(!addBatchMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }

        std::string utf8Str = batchMediaStr.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, addBatchMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::clearAllMedia() const{
        jmethodID clearMethod = env->GetMethodID(
        interfaceClass, "clearAllMedia", "()V");
        if(!clearMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, clearMethod);
        detach();
        return;
    }
    void MediaPlayer::moveMediaByIndex(const int& entryID,const int& toIndex){
        jmethodID moveByiMethod = env->GetMethodID(
        interfaceClass, "moveMediaByIndex", "(III)V");
        if(!moveByiMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveByiMethod,(jint)entryID,(jint)toIndex,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaByOffset(const int& entryID,const int& offset){
        jmethodID moveByOMethod = env->GetMethodID(
        interfaceClass, "moveMediaByOffset", "(III)V");
        if(!moveByOMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveByOMethod,(jint)entryID,(jint)offset,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaByIndex1(const int& index,const int& toIndex){
        jmethodID moveByI1Method = env->GetMethodID(
        interfaceClass, "moveMediaByIndex1", "(III)V");
        if(!moveByI1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveByI1Method,(jint)index,(jint)toIndex,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaByOffset1(const int& index,const int& offset){
        jmethodID moveByO1Method = env->GetMethodID(
        interfaceClass, "moveMediaByOffset1", "(III)V");
        if(!moveByO1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveByO1Method,(jint)index,(jint)offset,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToNext(const int& entryID){
        jmethodID moveToNMethod = env->GetMethodID(
        interfaceClass, "moveMediaToNext", "(II)V");
        if(!moveToNMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToNMethod,(jint)entryID,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToPrevious(const int& entryID){
        jmethodID moveToPMethod = env->GetMethodID(
        interfaceClass, "moveMediaToPrevious", "(II)V");
        if(!moveToPMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToPMethod,(jint)entryID,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToFirst(const int& entryID){
        jmethodID moveToFMethod = env->GetMethodID(
        interfaceClass, "moveMediaToFirst", "(II)V");
        if(!moveToFMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToFMethod,(jint)entryID,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToLast(const int& entryID){
        jmethodID moveToLMethod = env->GetMethodID(
        interfaceClass, "moveMediaToLast", "(II)V");
        if(!moveToLMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToLMethod,(jint)entryID,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToNext1(const int& index){
        jmethodID moveToN1Method = env->GetMethodID(
        interfaceClass, "moveMediaToNext1", "(II)V");
        if(!moveToN1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToN1Method,(jint)index,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToPrevious1(const int& index){
        jmethodID moveToP1Method = env->GetMethodID(
        interfaceClass, "moveMediaToPrevious1", "(II)V");
        if(!moveToP1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToP1Method,(jint)index,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToFirst1(const int& index){
        jmethodID moveToF1Method = env->GetMethodID(
        interfaceClass, "moveMediaToFirst1", "(II)V");
        if(!moveToF1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToF1Method,(jint)index,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::moveMediaToLast1(const int& index){
        jmethodID moveToL1Method = env->GetMethodID(
        interfaceClass, "moveMediaToLast1", "(II)V");
        if(!moveToL1Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, moveToL1Method,(jint)index,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectMediaByIndex(const int& index){
        jmethodID slectByIndexMethod = env->GetMethodID(
        interfaceClass, "selectMediaByIndex", "(II)V");
        if(!slectByIndexMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectByIndexMethod,(jint)index,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectMediaByOffset(const int& offset){
        jmethodID slectByOMethod = env->GetMethodID(
        interfaceClass, "selectMediaByOffset", "(II)V");
        if(!slectByOMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectByOMethod,(jint)offset,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectNext() const{
        jmethodID slectNextMethod = env->GetMethodID(
        interfaceClass, "selectNext", "(I)V");
        if(!slectNextMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectNextMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectPrevious() const{
        jmethodID slectPMethod = env->GetMethodID(
        interfaceClass, "selectPrevious", "(I)V");
        if(!slectPMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectPMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectFirst() const{
        jmethodID slectFMethod = env->GetMethodID(
        interfaceClass, "selectFirst", "(I)V");
        if(!slectFMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectFMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectLast() const{
        jmethodID slectLMethod = env->GetMethodID(
        interfaceClass, "selectLast", "(I)V");
        if(!slectLMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectLMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::SelectMediaByEntryID(const int& entryID){
        jmethodID slectByEIdMethod = env->GetMethodID(
        interfaceClass, "SelectMediaByEntryID", "(II)V");
        if(!slectByEIdMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, slectByEIdMethod,(jint)entryID,(jint)uuid);
        detach();
        return;
    }


    void MediaPlayer::selectAudio(const String& param){
        jmethodID selectAudioMethod = env->GetMethodID(
        interfaceClass, "selectAudio", "(Ljava/lang/String;I)V");
        if(!selectAudioMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = param.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, selectAudioMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }


    void MediaPlayer::playFromStart() const{
        jmethodID playFromStartMethod = env->GetMethodID(
        interfaceClass, "playFromStart", "(I)V");
        if(!playFromStartMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, playFromStartMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::playByTime(const int& type,const String& timestamp,const int& speed){
        jmethodID playByTimeMethod = env->GetMethodID(
        interfaceClass, "playByTime", "(ILjava/lang/String;II)V");
        if(!playByTimeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = timestamp.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, playByTimeMethod,(jint)type,js,(jint)speed,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::playByTime(const int& type,const String& timestamp){
        jmethodID playByTimeMethod = env->GetMethodID(
        interfaceClass, "playByTime", "(ILjava/lang/String;II)V");
        if(!playByTimeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = timestamp.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, playByTimeMethod,(jint)type,js,1,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::playByEventId(const int& type,const String& timestamp,const int& eventId){
        jmethodID playByEventIdMethod = env->GetMethodID(
        interfaceClass, "playByEventId", "(ILjava/lang/String;II)V");
        if(!playByEventIdMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = timestamp.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, playByEventIdMethod,(jint)type,js,(jint)eventId,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::startMediaChange() const{
        jmethodID startMediaChangeMethod = env->GetMethodID(
        interfaceClass, "startMediaChange", "(I)V");
        if(!startMediaChangeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, startMediaChangeMethod,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::prepareByTime(const int& type,const String& timestamp){
        jmethodID prepareByTimeMethod = env->GetMethodID(
        interfaceClass, "prepareByTime", "(ILjava/lang/String;I)I");
        if(!prepareByTimeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return 0;
        }
        std::string utf8Str = timestamp.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        jint rVale = (jint) env->CallIntMethod(java_obj, prepareByTimeMethod,(jint)type,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return (int)rVale;
    }
    void MediaPlayer::pause() const{
        jmethodID pauseMethod = env->GetMethodID(
        interfaceClass, "pause", "(I)V");
        if(!pauseMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, pauseMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::fastForward(const int& speed){
        jmethodID fastForwardMethod = env->GetMethodID(
        interfaceClass, "fastForward", "(II)V");
        if(!fastForwardMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, fastForwardMethod,(jint)speed,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::fastRewind(const int& speed){
        jmethodID fastRewindMethod = env->GetMethodID(
        interfaceClass, "fastRewind", "(II)V");
        if(!fastRewindMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, fastRewindMethod,(jint)speed,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::resume() const{
        jmethodID resumeMethod = env->GetMethodID(
        interfaceClass, "resume", "(I)V");
        if(!resumeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, resumeMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::gotoEnd() const{
        jmethodID gotoEndMethod = env->GetMethodID(
        interfaceClass, "gotoEnd", "(I)V");
        if(!gotoEndMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, gotoEndMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::gotoStart() const{
        jmethodID gotoStartMethod = env->GetMethodID(
        interfaceClass, "gotoStart", "(I)V");
        if(!gotoStartMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, gotoStartMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::stop() const{
        jmethodID stopMethod = env->GetMethodID(
        interfaceClass, "stop", "(I)V");
        if(!stopMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, stopMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::refreshVideoDisplay() const{
        jmethodID refreshVideoDisplayMethod = env->GetMethodID(
        interfaceClass, "refreshVideoDisplay", "(I)V");
        if(!refreshVideoDisplayMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, refreshVideoDisplayMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::switchAudioChannel() const{
        jmethodID switchAudioChannelMethod = env->GetMethodID(
        interfaceClass, "switchAudioChannel", "(I)V");
        if(!switchAudioChannelMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, switchAudioChannelMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::switchAudioChannel(const String& type){
        jmethodID switchAudioChannel2Method = env->GetMethodID(
        interfaceClass, "switchAudioChannel", "(I)V");
        if(!switchAudioChannel2Method) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = type.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, switchAudioChannel2Method,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    String MediaPlayer::checkTeletextAvailability() const{
        jmethodID checkTeletextAvailabilityMthod = env->GetMethodID(
        interfaceClass, "checkTeletextAvailability", "(I)Ljava/lang/String;");
        if(!checkTeletextAvailabilityMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring pids = (jstring) env->CallObjectMethod(java_obj, checkTeletextAvailabilityMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(pids, NULL);
        env->ReleaseStringUTFChars(pids, str);
        env->DeleteLocalRef(pids);
        detach();
        return (String)str;
    }
    void MediaPlayer::teletextSurfaceSwitch(const int& teletextSwitch){
        jmethodID teletextSurfaceSwitchMethod = env->GetMethodID(
        interfaceClass, "teletextSurfaceSwitch", "(II)V");
        if(!teletextSurfaceSwitchMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, teletextSurfaceSwitchMethod,(jint)teletextSwitch,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::switchAudioTrack() const{
        jmethodID switchAudioTrackMethod = env->GetMethodID(
        interfaceClass, "switchAudioTrack", "(I)V");
        if(!switchAudioTrackMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, switchAudioTrackMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::switchSubtitle() const{
        jmethodID switchSubtitleMethod = env->GetMethodID(
        interfaceClass, "switchSubtitle", "(I)V");
        if(!switchSubtitleMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, switchSubtitleMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::sendVendorSpecificCommand(const String& xml){
        jmethodID svscMethod = env->GetMethodID(
        interfaceClass, "sendVendorSpecificCommand", "(Ljava/lang/String;I)V");
        if(!svscMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }

        std::string utf8Str = xml.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, svscMethod,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    int MediaPlayer::getAudioPID() const{
        jmethodID getAudioPIDMthod = env->GetMethodID(
        interfaceClass, "getAudioPID", "(I)I");
        if(!getAudioPIDMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint audioPid = (jint) env->CallIntMethod(java_obj, getAudioPIDMthod,(jint)uuid);
        detach();
        return (int)audioPid;
    }
    String MediaPlayer::getAudioPIDs() const{
        jmethodID getAudioPIDsMthod = env->GetMethodID(
        interfaceClass, "getAudioPIDs", "(I)Ljava/lang/String;");
        if(!getAudioPIDsMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring pids = (jstring) env->CallObjectMethod(java_obj, getAudioPIDsMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(pids, NULL);
        env->ReleaseStringUTFChars(pids, str);
        env->DeleteLocalRef(pids);
        detach();
        return (String)str;
    }
    void MediaPlayer::setAudioPID() const{
        jmethodID setAudioPIDMethod = env->GetMethodID(
        interfaceClass, "setAudioPID", "(I)V");
        if(!setAudioPIDMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setAudioPIDMethod,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getSubtitlePID() const{
        jmethodID getSubtitlePIDMthod = env->GetMethodID(
        interfaceClass, "getSubtitlePID", "(I)I");
        if(!getSubtitlePIDMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint subtitlePid = (jint) env->CallIntMethod(java_obj, getSubtitlePIDMthod,(jint)uuid);
        detach();
        return (int)subtitlePid;
    }
    String MediaPlayer::getSubtitlePIDs() const{
        jmethodID getSubtitlePIDsMthod = env->GetMethodID(
        interfaceClass, "getSubtitlePIDs", "(I)Ljava/lang/String;");
        if(!getSubtitlePIDsMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring subpids = (jstring) env->CallObjectMethod(java_obj, getSubtitlePIDsMthod,(jint)uuid);
        const char *str = env->GetStringUTFChars(subpids, NULL);
        env->ReleaseStringUTFChars(subpids, str);
        env->DeleteLocalRef(subpids);
        detach();
        return (String)str;
    }
    void MediaPlayer::setSubtitlePID() const{
        jmethodID setSubtitlePIDMethod = env->GetMethodID(
        interfaceClass, "setSubtitlePID", "(I)V");
        if(!setSubtitlePIDMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setSubtitlePIDMethod,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::setVolume(const int& intvolume){
        jmethodID setVolumeMethod = env->GetMethodID(
        interfaceClass, "setVolume", "(II)V");
        if(!setVolumeMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, setVolumeMethod,(jint)intvolume,(jint)uuid);
        detach();
        return;
    }
    int MediaPlayer::getVolume() const{
        jmethodID getVolumeMthod = env->GetMethodID(
        interfaceClass, "getVolume", "(I)I");
        if(!getVolumeMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint volume = (jint) env->CallIntMethod(java_obj, getVolumeMthod,(jint)uuid);
        detach();
        return (int)volume;
    }
    String MediaPlayer::getAllVideoTrackInfo() const{
        jmethodID getAllVideoTrackInfoMethod = env->GetMethodID(
        interfaceClass, "getAllVideoTrackInfo", "(I)Ljava/lang/String;");
        if(!getAllVideoTrackInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getAllVideoTrackInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getCurrentVideoTrackInfo() const{
        jmethodID getCurrentVideoTrackInfoMethod = env->GetMethodID(
        interfaceClass, "getCurrentVideoTrackInfo", "(I)Ljava/lang/String;");
        if(!getCurrentVideoTrackInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getCurrentVideoTrackInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getAllAudioTrackInfo() const{
        jmethodID getAllAudioTrackInfoMethod = env->GetMethodID(
        interfaceClass, "getAllAudioTrackInfo", "(I)Ljava/lang/String;");
        if(!getAllAudioTrackInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getAllAudioTrackInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getAllSubtitleInfo() const{
        jmethodID getAllSubtitleInfoMethod = env->GetMethodID(
        interfaceClass, "getAllSubtitleInfo", "(I)Ljava/lang/String;");
        if(!getAllSubtitleInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getAllSubtitleInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getAllCCInfo() const{
        jmethodID getAllCCInfoMethod = env->GetMethodID(
        interfaceClass, "getAllCCInfo", "(I)Ljava/lang/String;");
        if(!getAllCCInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getAllCCInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getCurrentAudioTrackInfo() const{
        jmethodID getCurrentAudioTrackInfoMethod = env->GetMethodID(
        interfaceClass, "getCurrentAudioTrackInfo", "(I)Ljava/lang/String;");
        if(!getCurrentAudioTrackInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getCurrentAudioTrackInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getCurrentSubtitleInfo() const{
        jmethodID getCurrentSubtitleInfoMethod = env->GetMethodID(
        interfaceClass, "getCurrentSubtitleInfo", "(I)Ljava/lang/String;");
        if(!getCurrentSubtitleInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getCurrentSubtitleInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    String MediaPlayer::getCurrentCCInfo() const{
        jmethodID getCurrentCCInfoMethod = env->GetMethodID(
        interfaceClass, "getCurrentCCInfo", "(I)Ljava/lang/String;");
        if(!getCurrentCCInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getCurrentCCInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    void MediaPlayer::selectSubtitle(const int& pid){
        jmethodID selectSubtitleMthod = env->GetMethodID(
        interfaceClass, "selectSubtitle", "(II)V");
        if(!selectSubtitleMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, selectSubtitleMthod,(jint)pid,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectDefaultAudio(const int& pid){
        jmethodID selectDefaultAudioMthod = env->GetMethodID(
        interfaceClass, "selectDefaultAudio", "(II)V");
        if(!selectDefaultAudioMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, selectDefaultAudioMthod,(jint)pid,(jint)uuid);
        detach();
        return;
    }
    void MediaPlayer::selectCC(const int& serviceNumber){
        jmethodID selectCCMthod = env->GetMethodID(
        interfaceClass, "selectCC", "(II)V");
        if(!selectCCMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, selectCCMthod,(jint)serviceNumber,(jint)uuid);
        detach();
        return;
    }
    String MediaPlayer::getBufferInfo() const{
        jmethodID getBufferInfoMethod = env->GetMethodID(
        interfaceClass, "getBufferInfo", "(I)Ljava/lang/String;");
        if(!getBufferInfoMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getBufferInfoMethod,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(greturn);
        detach();
        return (String)newStr;
    }
    int MediaPlayer::setVideoOutput(const int& value){
        jmethodID setVideoOutputMthod = env->GetMethodID(
        interfaceClass, "setVideoOutput", "(II)I");
        if(!setVideoOutputMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint volume = (jint) env->CallIntMethod(java_obj, setVideoOutputMthod,(jint)value,(jint)uuid);
        detach();
        return (int)volume;
    }
    int MediaPlayer::setMaxBandwidth(const int& value){
        jmethodID setMaxBandwidthMthod = env->GetMethodID(
        interfaceClass, "setMaxBandwidth", "(II)I");
        if(!setMaxBandwidthMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return -1;
        }
        jint volume = (jint) env->CallIntMethod(java_obj, setMaxBandwidthMthod,(jint)value,(jint)uuid);
        detach();
        return (int)volume;
    }
    void MediaPlayer::setVRRotate(const int& direction,const String& degree,const int& flag){
        jmethodID setVRRotateMthod = env->GetMethodID(
        interfaceClass, "setVRRotate", "(ILjava/lang/String;II)V");
        if(!setVRRotateMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = degree.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, setVRRotateMthod,(jint)direction,js,(jint)flag,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::clearScreen() const{
        jmethodID clearScreenMthod = env->GetMethodID(
        interfaceClass, "clearScreen", "(I)V");
        if(!clearScreenMthod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, clearScreenMthod,(jint)uuid);
        detach();
        return;
    }
    String MediaPlayer::get(const String& ioStr){
        jmethodID getMethod = env->GetMethodID(
        interfaceClass, "get", "(Ljava/lang/String;I)Ljava/lang/String;");
        if(!getMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return "";
        }
        std::string utf8Str = ioStr.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        jstring greturn = (jstring)env->CallObjectMethod(java_obj, getMethod,js,(jint)uuid);
        const char *newStr = env->GetStringUTFChars(greturn,NULL);
        env->ReleaseStringUTFChars(greturn, newStr);
        env->DeleteLocalRef(js);
        env->DeleteLocalRef(greturn);
        String rstr = newStr;
        detach();
        return rstr;
    }

    void MediaPlayer::set(const String& ioStr,const String& wrStr){
        jmethodID setMethod = env->GetMethodID(
        interfaceClass, "set", "(Ljava/lang/String;Ljava/lang/String;I)V");
        if(!setMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = ioStr.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        std::string utf8Str2 = wrStr.Utf8().c_str();
        jstring js2 = env->NewStringUTF(utf8Str2.c_str());
        env->CallVoidMethod(java_obj, setMethod,js,js2,(jint)uuid);
        env->DeleteLocalRef(js);
        env->DeleteLocalRef(js2);
        detach();
        return;
    }
    void MediaPlayer::addSingleMedia(const int& index,const String& mediaStrs){
        jmethodID addSingleMediaMethod = env->GetMethodID(
        interfaceClass, "addSingleMedia", "(ILjava/lang/String;I)V");
        if(!addSingleMediaMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        std::string utf8Str = mediaStrs.Utf8().c_str();
        jstring js = env->NewStringUTF(utf8Str.c_str());
        env->CallVoidMethod(java_obj, addSingleMediaMethod,(jint)index,js,(jint)uuid);
        env->DeleteLocalRef(js);
        detach();
        return;
    }
    void MediaPlayer::release() const{
        jmethodID releaseMethod = env->GetMethodID(
        interfaceClass, "release", "(I)V");
        if(!releaseMethod) {
            LOG(INFO) <<"callback_handler: failed to get method ID";
            if(isAttached) gJavaVM->DetachCurrentThread();
            return;
        }
        env->CallVoidMethod(java_obj, releaseMethod,(jint)uuid);
        detach();
        return;
    }

} // namespace blink


