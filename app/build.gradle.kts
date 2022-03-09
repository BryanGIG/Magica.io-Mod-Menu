plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.polarmods"
    compileSdk = 32

    defaultConfig {
        applicationId = "com.polarmods"
        minSdk = 19
        targetSdk = 30
        multiDexEnabled = false
        versionCode = 1
        versionName = "1.0"
        ndk {
            abiFilters.addAll(arrayOf("armeabi-v7a","arm64-v8a"))
        }
    }

    buildTypes {
        getByName("release") {
            isMinifyEnabled = true
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }

    kotlinOptions {
        jvmTarget = "1.8"
    }
    buildFeatures.prefab = true
}

dependencies {
    implementation("io.github.vvb2060.ndk:dobby:1.2")
}