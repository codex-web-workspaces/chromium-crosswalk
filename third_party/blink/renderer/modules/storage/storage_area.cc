/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/modules/storage/storage_area.h"

#include "base/feature_list.h"
#include "base/memory/scoped_refptr.h"
#include "third_party/blink/public/common/features.h"
#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/modules/storage/dom_window_storage.h"
#include "third_party/blink/renderer/modules/storage/inspector_dom_storage_agent.h"
#include "third_party/blink/renderer/modules/storage/storage_controller.h"
#include "third_party/blink/renderer/modules/storage/storage_event.h"
#include "third_party/blink/renderer/modules/storage/storage_namespace.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/weborigin/security_origin.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "base/android/jni_android.h"
#include "v8/src/objects/objectsutil.h"


extern jobject xwalk_obj;
extern JavaVM* gJavaVM;
namespace blink {

StorageArea* StorageArea::Create(LocalFrame* frame,
                                 scoped_refptr<CachedStorageArea> storage_area,
                                 StorageType storage_type) {
  return MakeGarbageCollected<StorageArea>(frame, std::move(storage_area),
                                           storage_type,
                                           /* should_enqueue_events */ true);
}

StorageArea* StorageArea::CreateForInspectorAgent(
    LocalFrame* frame,
    scoped_refptr<CachedStorageArea> storage_area,
    StorageType storage_type) {
  return MakeGarbageCollected<StorageArea>(frame, std::move(storage_area),
                                           storage_type,
                                           /* should_enqueue_events */ false);
}

StorageArea::StorageArea(LocalFrame* frame,
                         scoped_refptr<CachedStorageArea> storage_area,
                         StorageType storage_type,
                         bool should_enqueue_events)
    : ContextClient(frame),
      cached_area_(std::move(storage_area)),
      storage_type_(storage_type),
      should_enqueue_events_(should_enqueue_events) {
  DCHECK(frame);
  DCHECK(cached_area_);
  cached_area_->RegisterSource(this);
}

unsigned StorageArea::length(ExceptionState& exception_state) const {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return 0;
  }
  return cached_area_->GetLength();
}

String StorageArea::key(unsigned index, ExceptionState& exception_state) const {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return String();
  }
  return cached_area_->GetKey(index);
}

String StorageArea::getItem(const String& key,
                            ExceptionState& exception_state) const {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return String();
  }
  String value = cached_area_->GetItem(key);
  if(value.IsEmpty() && v8::storageUseJni){
    JNIEnv *env;
    bool isAttached = false;
    int status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
    if(status < 0) {
        LOG(INFO) <<"callback_handler: failed to get JNI environment, "
        "assuming native thread";
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to attach "
            "current thread";
            return value;
        }
        isAttached = true;
    }
    /* Construct a Java string */
    jclass interfaceClass = env->GetObjectClass(xwalk_obj);
    if(!interfaceClass) {
        LOG(INFO) <<"callback_handler: failed to get class reference";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return value;
    }

    jmethodID jniMethod = env->GetMethodID(interfaceClass, "getStorage", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if(!jniMethod) {
        LOG(INFO) <<"callback_handler: failed to get method ID";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return value;
    }
    jstring urlStr = env->NewStringUTF(GetPageUrl().GetString().Utf8().c_str());
    jstring keyStr = env->NewStringUTF(key.Utf8().c_str());
    String type = "1";
    if(storage_type_ == StorageType::kLocalStorage){
      type = "0";
    }
    jstring js = env->NewStringUTF(type.Utf8().c_str());
    jstring rvalue = (jstring) env->CallObjectMethod(xwalk_obj, jniMethod,urlStr,keyStr,js);
    
    String wtfString = String();
    if(rvalue != nullptr){
      const char *utf8Chars = env->GetStringUTFChars(rvalue, nullptr);  
      wtfString = (String)utf8Chars;
      // std::string utf8String(utf8Chars);
      env->ReleaseStringUTFChars(rvalue, utf8Chars);
      // wtfString = String::fromUTF8(utf8String);
    }
    env->DeleteLocalRef(urlStr);
    env->DeleteLocalRef(keyStr);
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(rvalue);
    env->DeleteLocalRef(interfaceClass);
    if(isAttached) {
        gJavaVM->DetachCurrentThread();
    }
    // if(!value.IsEmpty()){
    //   // setItem(key,value,exception_state);
    //   cached_area_->SetItem(key, value, this);
    // }
    return wtfString;
  }
  return value;

}

bool StorageArea::setItem(const String& key,
                          const String& value,
                          ExceptionState& exception_state) {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return true;
  }
  if (!cached_area_->SetItem(key, value, this)) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kQuotaExceededError,
        "Setting the value of '" + key + "' exceeded the quota.");
  }

  if(v8::storageUseJni){
    JNIEnv *env;
    bool isAttached = false;
    int status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
    if(status < 0) {
        LOG(INFO) <<"callback_handler: failed to get JNI environment, "
        "assuming native thread";
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to attach "
            "current thread";
            return true;
        }
        isAttached = true;
    }
    /* Construct a Java string */
    jclass interfaceClass = env->GetObjectClass(xwalk_obj);
    if(!interfaceClass) {
        LOG(INFO) <<"callback_handler: failed to get class reference";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return true;
    }

    jmethodID jniMethod = env->GetMethodID(interfaceClass, "setStorage", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(!jniMethod) {
        LOG(INFO) <<"callback_handler: failed to get method ID";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return true;
    }
    jstring urlStr = env->NewStringUTF(GetPageUrl().GetString().Utf8().c_str());
    jstring keyStr = env->NewStringUTF(key.Utf8().c_str());
    jstring valueStr = env->NewStringUTF(value.Utf8().c_str());
    String type = "1";
    if(storage_type_ == StorageType::kLocalStorage){
      type = "0";
    }
    jstring js = env->NewStringUTF(type.Utf8().c_str());
    env->CallVoidMethod(xwalk_obj, jniMethod,urlStr,keyStr,valueStr,js);
    env->DeleteLocalRef(urlStr);
    env->DeleteLocalRef(keyStr);
    env->DeleteLocalRef(valueStr);
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(interfaceClass);
    if(isAttached) {
        gJavaVM->DetachCurrentThread();
    }
  }

  return true;
}

DeleteResult StorageArea::removeItem(const String& key,
                                     ExceptionState& exception_state) {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return kDeleteSuccess;
  }
  cached_area_->RemoveItem(key, this);

  if(v8::storageUseJni){
    JNIEnv *env;
    bool isAttached = false;
    int status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
    if(status < 0) {
        LOG(INFO) <<"callback_handler: failed to get JNI environment, "
        "assuming native thread";
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to attach "
            "current thread";
            return kDeleteSuccess;
        }
        isAttached = true;
    }
    /* Construct a Java string */
    jclass interfaceClass = env->GetObjectClass(xwalk_obj);
    if(!interfaceClass) {
        LOG(INFO) <<"callback_handler: failed to get class reference";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return kDeleteSuccess;
    }

    jmethodID jniMethod = env->GetMethodID(interfaceClass, "removeStorage", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(!jniMethod) {
        LOG(INFO) <<"callback_handler: failed to get method ID";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return kDeleteSuccess;
    }
    jstring urlStr = env->NewStringUTF(GetPageUrl().GetString().Utf8().c_str());
    jstring keyStr = env->NewStringUTF(key.Utf8().c_str());
    String type = "1";
    if(storage_type_ == StorageType::kLocalStorage){
      type = "0";
    }
    jstring js = env->NewStringUTF(type.Utf8().c_str());
    env->CallVoidMethod(xwalk_obj, jniMethod,urlStr,keyStr,js);
    env->DeleteLocalRef(urlStr);
    env->DeleteLocalRef(keyStr);
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(interfaceClass);
    if(isAttached) {
        gJavaVM->DetachCurrentThread();
    }
  }
  return kDeleteSuccess;
}

void StorageArea::clear(ExceptionState& exception_state) {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return;
  }
  cached_area_->Clear(this);

  if(v8::storageUseJni){
    JNIEnv *env;
    bool isAttached = false;
    int status = gJavaVM->GetEnv((void **) &env, JNI_VERSION_1_4);
    if(status < 0) {
        LOG(INFO) <<"callback_handler: failed to get JNI environment, "
        "assuming native thread";
        status = gJavaVM->AttachCurrentThread(&env, NULL);
        if(status < 0) {
            LOG(INFO) <<"callback_handler: failed to attach "
            "current thread";
            return ;
        }
        isAttached = true;
    }
    /* Construct a Java string */
    jclass interfaceClass = env->GetObjectClass(xwalk_obj);
    if(!interfaceClass) {
        LOG(INFO) <<"callback_handler: failed to get class reference";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return ;
    }

    jmethodID jniMethod = env->GetMethodID(interfaceClass, "clearStorage", "(Ljava/lang/String;Ljava/lang/String;)V");
    if(!jniMethod) {
        LOG(INFO) <<"callback_handler: failed to get method ID";
        if(isAttached) gJavaVM->DetachCurrentThread();
        return ;
    }
    jstring urlStr = env->NewStringUTF(GetPageUrl().GetString().Utf8().c_str());
    String type = "1";
    if(storage_type_ == StorageType::kLocalStorage){
      type = "0";
    }
    jstring js = env->NewStringUTF(type.Utf8().c_str());
    env->CallVoidMethod(xwalk_obj, jniMethod,urlStr,js);
    env->DeleteLocalRef(urlStr);
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(interfaceClass);
    if(isAttached) {
        gJavaVM->DetachCurrentThread();
    }
  }
}

bool StorageArea::Contains(const String& key,
                           ExceptionState& exception_state) const {
  if (!CanAccessStorage()) {
    exception_state.ThrowSecurityError("access is denied for this document.");
    return false;
  }
  return !cached_area_->GetItem(key).IsNull();
}

void StorageArea::NamedPropertyEnumerator(Vector<String>& names,
                                          ExceptionState& exception_state) {
  unsigned length = this->length(exception_state);
  if (exception_state.HadException())
    return;
  names.resize(length);
  for (unsigned i = 0; i < length; ++i) {
    String key = this->key(i, exception_state);
    if (exception_state.HadException())
      return;
    DCHECK(!key.IsNull());
    String val = getItem(key, exception_state);
    if (exception_state.HadException())
      return;
    names[i] = key;
  }
}

bool StorageArea::NamedPropertyQuery(const AtomicString& name,
                                     ExceptionState& exception_state) {
  if (name == "length")
    return false;
  bool found = Contains(name, exception_state);
  return found && !exception_state.HadException();
}

void StorageArea::Trace(blink::Visitor* visitor) {
  ScriptWrappable::Trace(visitor);
  ContextClient::Trace(visitor);
}

bool StorageArea::CanAccessStorage() const {
  LocalFrame* frame = GetFrame();
  if (!frame || !frame->GetPage())
    return false;

  if (did_check_can_access_storage_)
    return can_access_storage_cached_result_;
  can_access_storage_cached_result_ =
      StorageController::CanAccessStorageArea(frame, storage_type_);
  did_check_can_access_storage_ = true;
  return can_access_storage_cached_result_;
}

KURL StorageArea::GetPageUrl() const {
  LocalFrame* frame = GetFrame();
  if (!frame)
    return KURL();
  return GetFrame()->GetDocument()->Url();
}

bool StorageArea::EnqueueStorageEvent(const String& key,
                                      const String& old_value,
                                      const String& new_value,
                                      const String& url) {
  if (!should_enqueue_events_)
    return true;
  if (!GetExecutionContext())
    return false;
  LocalFrame* frame = GetFrame();
  if (!frame)
    return true;
  frame->DomWindow()->EnqueueWindowEvent(
      *StorageEvent::Create(event_type_names::kStorage, key, old_value,
                            new_value, url, this),
      TaskType::kDOMManipulation);
  return true;
}

blink::WebScopedVirtualTimePauser StorageArea::CreateWebScopedVirtualTimePauser(
    const char* name,
    WebScopedVirtualTimePauser::VirtualTaskDuration duration) {
  LocalFrame* frame = GetFrame();
  if (!frame)
    return blink::WebScopedVirtualTimePauser();
  return frame->GetFrameScheduler()->CreateWebScopedVirtualTimePauser(name,
                                                                      duration);
}

}  // namespace blink
