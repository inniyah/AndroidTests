#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#ifdef ANDROID
#include <android/log.h>
#define LOGV(...) (__android_log_print(ANDROID_LOG_VERBOSE, "SFML_Example", __VA_ARGS__))
#define LOGI(...) (__android_log_print(ANDROID_LOG_INFO,    "SFML_Example", __VA_ARGS__))
#define LOGW(...) (__android_log_print(ANDROID_LOG_WARN,    "SFML_Example", __VA_ARGS__))
#define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR,   "SFML_Example", __VA_ARGS__))
#define LOGF(...) (__android_log_print(ANDROID_LOG_FATAL,   "SFML_Example", __VA_ARGS__))
#else
#include <stdio.h>
#define LOGV(...) { printf("[DEBUG] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGI(...) { printf("[INFO]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGW(...) { printf("[WARN]  "); printf(__VA_ARGS__); printf("\n"); }
#define LOGE(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOGF(...) { printf("[FATAL] "); printf(__VA_ARGS__); printf("\n"); }
#endif

// Do we want to showcase direct JNI/NDK interaction?
// Undefine this to get real cross-platform code.
// Uncomment this to try JNI access; this seems to be broken in latest NDKs
//#define USE_JNI

#if defined(USE_JNI)
// These headers are only needed for direct NDK/JDK interaction
#include <jni.h>
#include <android/native_activity.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>

// NDK/JNI sub example - call Java code from native code
int vibrate(sf::Time duration)
{
    LOGI("Trying to vibrate");

    // First we'll need the native activity handle
    ANativeActivity *activity = sf::getNativeActivity();
    
    // Retrieve the JVM and JNI environment
    JavaVM* vm = activity->vm;
    JNIEnv* env = activity->env;

    // First, attach this thread to the main thread
    JavaVMAttachArgs attachargs;
    attachargs.version = JNI_VERSION_1_6;
    attachargs.name = "NativeThread";
    attachargs.group = NULL;
    jint res = vm->AttachCurrentThread(&env, &attachargs);

    if (res == JNI_ERR) {
        //~ LOGE("Failed to attach current thread");
        return EXIT_FAILURE;
    }

    // Retrieve class information
    jclass natact = env->FindClass("android/app/NativeActivity");
    jclass context = env->FindClass("android/content/Context");
    
    // Get the value of a constant
    jfieldID fid = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
    jobject svcstr = env->GetStaticObjectField(context, fid);

    //~ if (svcstr == 0) {
    //~     LOGE("Failed to obtain the vibration sevice");
    //~ }

    // Get the method 'getSystemService' and call it
    jmethodID getss = env->GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);

    //~ if (vib_obj == 0) {
    //~     LOGE("Failed to obtain the vibration sevice object");
    //~ }

    // Get the object's class and retrieve the member name
    jclass vib_cls = env->GetObjectClass(vib_obj);
    jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V"); 

    //~ if (vibrate == 0) {
    //~     LOGE("Failed to obtain the vibration method");
    //~ }

    // Determine the timeframe
    jlong length = duration.asMilliseconds();

    //~ LOGI("Vibrate!!");

    // Bzzz!
    env->CallVoidMethod(vib_obj, vibrate, length);

    //~ LOGI("Yay!! I did it!!");

    // Free references
    env->DeleteLocalRef(vib_obj);
    env->DeleteLocalRef(vib_cls);
    env->DeleteLocalRef(svcstr);
    env->DeleteLocalRef(context);
    env->DeleteLocalRef(natact);

    // Detach thread again
    vm->DetachCurrentThread();

    //~ LOGI("Successfully exiting vibration function");
    return EXIT_SUCCESS;
}
#endif

// This is the actual Android example. You don't have to write any platform
// specific code, unless you want to use things not directly exposed.
// ('vibrate()' in this example; undefine 'USE_JNI' above to disable it)
int main(int argc, char *argv[])
{
    sf::VideoMode screen(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    std::string texture_filename = "image.png";
    sf::Texture texture;
    if(!texture.loadFromFile(texture_filename)) {
        LOGE("Failed to load texture '%s' from file", texture_filename.c_str());
        return EXIT_FAILURE;
    } else {
        LOGV("Successfully loaded texture '%s' from file", texture_filename.c_str());
    }

    sf::Sprite image(texture);
    image.setPosition(screen.width / 2, screen.height / 2);
    image.setOrigin(texture.getSize().x/2, texture.getSize().y/2);

    sf::Font font;
    if (!font.loadFromFile("sansation.ttf"))
        return EXIT_FAILURE;

    sf::Text text("Tap anywhere to move the logo.", font, 64);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, 10);

    // Loading canary.wav fails for me for now; haven't had time to test why

    /*sf::Music music;
    if(!music.openFromFile("canary.wav"))
        return EXIT_FAILURE;

    music.play();*/

    sf::View view = window.getDefaultView();

    sf::Color background = sf::Color::White;

    // We shouldn't try drawing to the screen while in background
    // so we'll have to track that. You can do minor background
    // work, but keep battery life in mind.
    bool active = true;

    while (window.isOpen())
    {
        sf::Event event;

        while (active ? window.pollEvent(event) : window.waitEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width/2, event.size.height/2);
                    window.setView(view);
                    break;
                case sf::Event::LostFocus:
                    background = sf::Color::Black;
                    break;
                case sf::Event::GainedFocus:
                    background = sf::Color::White;
                    break;
                
                // On Android MouseLeft/MouseEntered are (for now) triggered,
                // whenever the app loses or gains focus.
                case sf::Event::MouseLeft:
                    active = false;
                    break;
                case sf::Event::MouseEntered:
                    active = true;
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0)
                    {
                        image.setPosition(event.touch.x, event.touch.y);
#if defined(USE_JNI)
                        vibrate(sf::milliseconds(10));
#endif
                    }
                    break;
            }
        }

        if (active)
        {
            window.clear(background);
            window.draw(image);
            window.draw(text);
            window.display();
        }
        else {
            sf::sleep(sf::milliseconds(100));
        }
    }
    return EXIT_SUCCESS;
}
