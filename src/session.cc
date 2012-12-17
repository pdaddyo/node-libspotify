/*
 * =====================================================================================
 *
 *       Filename:  session.cc
 *
 *    Description:  Bindings for the sp_session subsystem
 *
 *        Version:  1.0
 *        Created:  16/12/2012 21:38:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Florent Jaby (FJ), florent.jaby@gmail.com
 *        Company:  Florent Jaby
 *
 * =====================================================================================
 */

#include "session.h"

using namespace v8;
using namespace nsp;
static unsigned int scount = 0;

void Session::onMainThreadNotified(sp_session* spsession) {
}

void Session::onLoggedIn(sp_session* spsession, sp_error error) {
    if(error != SP_ERROR_OK) {
        fprintf(stderr, "Error %s\n", sp_error_message(error));
        exit(1);
    }
    printf("LOGGED IN!\n");
}

void Session::processEvents() {
    int next_timeout = 0;
    sp_session_process_events(this->spsession, &next_timeout);
}

static void process_timer(uv_timer_t* handle, int status) {
    Session* session = static_cast<Session*>(handle->data);
    fprintf(stderr, "3 seconds after init of session %d\n", session->number);
    session->processEvents();
}

Session::Session() {
    uv_timer_init(uv_default_loop(), &timer_handle);
    timer_handle.data = this;
    uv_timer_start(&timer_handle, process_timer, 3000, 0);

    this->number = ++scount;


    this->initAppKey();
    this->initCallbacks();
    this->initConfig();

    sp_error error;
    error = sp_session_create(&spconfig, &spsession);
    if(error != SP_ERROR_OK) {
        fprintf(stderr, "Error: %s\n", sp_error_message(error));
        exit(1);
    }
    fprintf(stderr, "Coucou\n");
    sp_session_login(spsession, "FlobyIV", "mlkPOI5555Spotify", 0, NULL);
};

Session::~Session() {
};

static v8::Handle<v8::Value> bidule(const v8::Arguments& args) {
    printf("chose\n");
    return v8::Null();
}
void Session::Init(Handle<Object> target) {
    // prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(Session::New);
    tpl->SetClassName(String::NewSymbol("Session"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // prototype
    tpl->PrototypeTemplate()->Set(
            String::NewSymbol("bidule"),
            FunctionTemplate::New(bidule)->GetFunction()
    );

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol("Session"), constructor);
}

Handle<Value> Session::New(const Arguments& args) {
    HandleScope scope;
    Session* session = new Session();
    session->Wrap(args.This());


    return args.This();
}

void Session::initCallbacks() {
    memset(&spcallbacks, 0, sizeof(sp_session_callbacks));
    spcallbacks.logged_in = onLoggedIn;
    spcallbacks.notify_main_thread = onMainThreadNotified;
}


void Session::initConfig() {
    memset(&spconfig, 0, sizeof(sp_session_config));
    spconfig.api_version = SPOTIFY_API_VERSION;
    spconfig.cache_location = "tmp";
    spconfig.settings_location = "tmp";
    spconfig.application_key = g_appkey;
    spconfig.application_key_size = g_appkey_size;
    spconfig.user_agent = "node-libspotify";
    spconfig.userdata = this;
    spconfig.callbacks = &spcallbacks;
}




static uint8_t appkey[] = {
    0x01, 0xD6, 0x13, 0x0A, 0x4B, 0x53, 0xB7, 0xB5, 0x34, 0xA8, 0xFE, 0xB4, 0x96, 0xFF, 0x62, 0x41,
    0x24, 0x03, 0x33, 0xAA, 0xA7, 0x17, 0xDA, 0xAD, 0x18, 0x10, 0x1C, 0x95, 0x67, 0x7A, 0x1F, 0x25,
    0xD9, 0x1D, 0xAA, 0x66, 0x84, 0x50, 0x04, 0xF3, 0xD1, 0x07, 0x7A, 0x20, 0x37, 0xB0, 0xF4, 0xA5,
    0xAE, 0xF7, 0x65, 0x9B, 0xE2, 0xAA, 0x72, 0x21, 0x12, 0x96, 0x8B, 0xD8, 0x34, 0x10, 0xA5, 0xF3,
    0x58, 0x9A, 0x6F, 0xA9, 0x7C, 0xC6, 0x16, 0x42, 0x2E, 0xBF, 0x02, 0xFD, 0x4F, 0x3E, 0xF7, 0x16,
    0x6D, 0x89, 0xB4, 0xDC, 0x61, 0x86, 0x9C, 0xC2, 0x7C, 0xBC, 0xE1, 0xE2, 0x1B, 0x6B, 0x30, 0xF8,
    0x8D, 0x9D, 0xE4, 0x6B, 0x3B, 0xCD, 0xE6, 0xA7, 0xA2, 0x32, 0xD2, 0x0D, 0x7E, 0x69, 0xFE, 0x16,
    0x81, 0xE2, 0xE2, 0x08, 0x8E, 0x47, 0xC8, 0xE5, 0x35, 0xA5, 0x64, 0x79, 0x7F, 0x74, 0xE9, 0x43,
    0x02, 0x1B, 0x4E, 0x74, 0x58, 0xEC, 0xE6, 0xDA, 0xD3, 0x24, 0xC5, 0x2A, 0x04, 0xD5, 0x61, 0x72,
    0xD5, 0xFC, 0xE1, 0x9E, 0xD3, 0xCB, 0xF7, 0xF4, 0x19, 0xD6, 0x38, 0x8A, 0xC4, 0x96, 0xEA, 0x8B,
    0x7D, 0x6C, 0x5A, 0x5B, 0x2E, 0x6F, 0x59, 0xEF, 0x03, 0x2B, 0xDB, 0x8C, 0xEC, 0xDB, 0x74, 0x27,
    0x24, 0x17, 0x83, 0x20, 0x07, 0x76, 0xC5, 0x98, 0xAE, 0xFF, 0x29, 0xB1, 0xBA, 0x98, 0x6B, 0x90,
    0x76, 0x2B, 0xBC, 0xC9, 0x6C, 0x32, 0xC9, 0xFD, 0xD4, 0x50, 0x15, 0xF5, 0x39, 0x75, 0x8C, 0x70,
    0x7E, 0x8F, 0x3D, 0x0E, 0x8F, 0x9E, 0x58, 0xFE, 0x87, 0x8C, 0x89, 0x83, 0x03, 0xCD, 0xCF, 0x92,
    0xC6, 0x55, 0xDE, 0x29, 0x05, 0xFC, 0x6F, 0x5C, 0xB7, 0x3D, 0x9E, 0x43, 0xF7, 0x2F, 0x0C, 0x8B,
    0x7A, 0x34, 0x7B, 0x2E, 0xCA, 0x2B, 0x53, 0xAD, 0x8C, 0xC2, 0x77, 0xEB, 0x8F, 0xB2, 0xE4, 0x33,
    0x43, 0x9B, 0xE8, 0x05, 0x8F, 0x9D, 0xED, 0xB5, 0xAA, 0x27, 0x0F, 0xEE, 0x70, 0x6F, 0xEB, 0xCF,
    0x6C, 0x6F, 0x27, 0x73, 0x7F, 0x02, 0x34, 0xE4, 0xBC, 0x3E, 0x04, 0x59, 0xDF, 0xDC, 0x9F, 0xC9,
    0xBF, 0xF8, 0xBC, 0xDA, 0xD1, 0x4D, 0x3A, 0x2F, 0xA4, 0x7B, 0x6B, 0xC4, 0x40, 0x2E, 0xDF, 0xEB,
    0xA4, 0x93, 0x22, 0xA6, 0x53, 0xE5, 0x60, 0x5B, 0x82, 0x5E, 0x7F, 0x28, 0x82, 0xE1, 0x52, 0xF9,
    0xF2,
};

void Session::initAppKey() {
    g_appkey_size = sizeof(appkey);   
    g_appkey = appkey;
}
