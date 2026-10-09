// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/loader/cookie_jar.h"

#include "services/service_manager/public/cpp/interface_provider.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/platform/instrumentation/histogram.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"
#include "base/android/jni_android.h"

extern jobject xwalk_obj;
extern JavaVM* gJavaVM;
namespace blink {
int statusCookieJar;
JNIEnv *envCookieJar;
jclass interfaceClassCookieJar = NULL;
bool isAttachedCookieJar = false;

void detachCookieJar(){
    //env->DeleteLocalRef(interfaceClass);
    if(isAttachedCookieJar){
       gJavaVM->DetachCurrentThread();
       LOG(INFO) << "detach current thread";
    }
}

CookieJar::CookieJar(blink::Document* document) : document_(document) {}

CookieJar::~CookieJar() = default;

void CookieJar::SetCookie(const String& value) {
  KURL cookie_url = document_->CookieURL();
  if (cookie_url.IsEmpty())
    return;

  if(interfaceClassCookieJar == NULL){
    statusCookieJar = gJavaVM->GetEnv((void **) &envCookieJar, JNI_VERSION_1_4);
    if(statusCookieJar < 0) {
        LOG(INFO) <<"callback_handler: failed to get JNI environment, "
        "assuming native thread";
        statusCookieJar = gJavaVM->AttachCurrentThread(&envCookieJar, NULL);
        if(statusCookieJar < 0) {
            LOG(INFO) <<"callback_handler: failed to attach "
            "current thread";
            return;
        }
        isAttachedCookieJar = true;
    }
    /* Construct a Java string */
    interfaceClassCookieJar = envCookieJar->GetObjectClass(xwalk_obj);
    if(!interfaceClassCookieJar) {
        LOG(INFO) <<"callback_handler: failed to get class reference";
        if(isAttachedCookieJar) gJavaVM->DetachCurrentThread();
        return;
    }
  }
  jmethodID setCookieMethod = envCookieJar->GetMethodID(interfaceClassCookieJar, "setCookie", "(Ljava/lang/String;Ljava/lang/String;)V");
  if(!setCookieMethod) {
    LOG(INFO) <<"callback_handler: failed to get method ID";
    if(isAttachedCookieJar) gJavaVM->DetachCurrentThread();
      return;
    }
  // const char* urlStr = cookie_url.GetString().c_str();
  // jstring urlJs = envCookieJar->NewStringUTF(cookie_url.GetString().c_str());
  // const char* cookieStr = value.Utf8().c_str();
  // LOG(INFO)<<"----to jni---cookie---"<<value;
  // jstring cookieJs = envCookieJar->NewStringUTF(cookieStr);

  jchar* jurlcharArray = new jchar[cookie_url.GetString().length()];
  for (size_t i = 0; i < cookie_url.GetString().length(); ++i) {
      jurlcharArray[i] = static_cast<jchar>(cookie_url.GetString()[i]);
  }
 
  // 使用Java字符数组创建jstring
  jstring urlJs = envCookieJar->NewString(jurlcharArray, cookie_url.GetString().length());

  jchar* jcharArray = new jchar[value.length()];
  for (size_t i = 0; i < value.length(); ++i) {
      jcharArray[i] = static_cast<jchar>(value[i]);
  }
 
  // 使用Java字符数组创建jstring
  jstring cookieJs = envCookieJar->NewString(jcharArray, value.length());

  // 清理分配的字符数组
  delete[] jurlcharArray;
  delete[] jcharArray;


  envCookieJar->CallVoidMethod(xwalk_obj,setCookieMethod,urlJs,cookieJs);
  envCookieJar->DeleteLocalRef(urlJs);
  envCookieJar->DeleteLocalRef(cookieJs);
  detachCookieJar();

  RequestRestrictedCookieManagerIfNeeded();
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER("Blink.CookieJar.SyncCookiesSetTime");
  backend_->SetCookieFromString(cookie_url, document_->SiteForCookies(), value);
}

String CookieJar::Cookies() {
  KURL cookie_url = document_->CookieURL();
  if (cookie_url.IsEmpty())
    return String();

  SCOPED_BLINK_UMA_HISTOGRAM_TIMER("Blink.CookieJar.SyncCookiesTime");
  RequestRestrictedCookieManagerIfNeeded();
  String value;
  backend_->GetCookiesString(cookie_url, document_->SiteForCookies(), &value);
  return value;
}

bool CookieJar::CookiesEnabled() {
  KURL cookie_url = document_->CookieURL();
  if (cookie_url.IsEmpty())
    return false;

  RequestRestrictedCookieManagerIfNeeded();
  bool cookies_enabled = false;
  backend_->CookiesEnabledFor(cookie_url, document_->SiteForCookies(),
                              &cookies_enabled);
  return cookies_enabled;
}

void CookieJar::RequestRestrictedCookieManagerIfNeeded() {
  if (!backend_.is_bound() || backend_.encountered_error()) {
    document_->GetInterfaceProvider()->GetInterface(
        mojo::MakeRequest(&backend_));
  }
}

}  // namespace blink
