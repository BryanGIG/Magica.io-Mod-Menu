import java.io.ByteArrayOutputStream

plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "com.polarmods"
    compileSdk = 30

    defaultConfig {
        applicationId = "com.polarmods"
        minSdk = 19
        targetSdk = 30
        multiDexEnabled = false
        versionCode = 1
        versionName = "1.0"
        ndk {
            abiFilters.addAll(arrayOf("arm64-v8a"))
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

    kotlinOptions.jvmTarget = "1.8"
}

afterEvaluate {
    android.applicationVariants.forEach { variant ->
        val variantCapped = variant.name.capitalize()
        val variantLowered = variant.name.toLowerCase()
        val buildTypeLowered = variant.buildType.name.toLowerCase()

        task<Exec>("push${variantCapped}") {
            dependsOn("assemble$variantCapped")
            workingDir = file("build/outputs/apk/${variantLowered}/")
            commandLine = mutableListOf(android.adbExecutable.path, "push", "app-${buildTypeLowered}.apk", "/sdcard/MT2/apks/")
        }

        variant.assembleProvider.get().finalizedBy("push${variantCapped}")
    }
}