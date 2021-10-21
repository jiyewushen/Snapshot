package com.dayan.buildsrc

object Versions {

    const val compose_version = "1.0.0"

    const val appcompat_version = "1.3.1"
    const val core_version="1.5.0"
    const val activity_version = "1.3.0"
    const val annotation_version = "1.2.0"
    const val recyclerview_version = "1.2.0"
    const val recyclerview_selection_version = "1.1.0"
    const val vectordrawable_animated_version = "1.1.0"
    const val fragment_version = "1.3.6"
    const val kotlin_version = "1.5.21"
    const val kotlinx_coroutines_version = "1.5.1"
    const val core_ktx_version = "1.3.2"
    const val mmkv_version = "1.2.2"
    const val gradle_version = "4.2.2"
    const val room_version = "2.3.0"
    const val lifecycle_version = "2.4.0-alpha03"
    const val nav_version = "2.3.5"
    const val paging_version = "3.1.0-alpha03"
    const val glide_version = "4.12.0"
    const val android_svg_version = "1.4"
    const val constraintlayout_version = "2.1.0"
    const val viewpager2_version = "1.1.0-beta01"
    const val coordinatorlayout_version = "1.1.0"
    const val dynamicanimation_version = "1.0.0"
    const val relinker_version = "1.3.1"
    const val ARTIFACT_VERSION = "1.0.1"

    const val ads_version = "13.4.29.301"
    const val exo_player = "2.15.1"
}


object Config {
    const val compile_sdk_version = 30
    const val min_sdk_version = 21
    const val target_sdk_version = 30

    //当前NDK版本为r21e,如果修改，需要使用对应版本编译的ffmpeg动态库，防止一些未知崩溃
    const val ndk_version = "21.4.7075529"
    const val androidGradlePlugin = "com.android.tools.build:gradle:${Versions.gradle_version}"
    const val kotlinGradlePlugin =
        "org.jetbrains.kotlin:kotlin-gradle-plugin:${Versions.kotlin_version}"
    const val navigationSafeArgsGradlePlugin =
        "androidx.navigation:navigation-safe-args-gradle-plugin:${Versions.nav_version}"
}


object Kotlin {
    private const val version = Versions.kotlin_version
    const val stdlib = "org.jetbrains.kotlin:kotlin-stdlib-jdk8:$version"
    const val gradlePlugin = "org.jetbrains.kotlin:kotlin-gradle-plugin:$version"
    const val extensions = "org.jetbrains.kotlin:kotlin-android-extensions:$version"
    const val annotationsJvm = "org.jetbrains.kotlin:kotlin-annotations-jvm:$version"

    object Coroutines {
        private const val version = Versions.kotlinx_coroutines_version
        const val android = "org.jetbrains.kotlinx:kotlinx-coroutines-android:$version"
        const val test = "org.jetbrains.kotlinx:kotlinx-coroutines-test:$version"
    }
}

object ThirdLib {
    const val mmkv = "com.tencent:mmkv-static:${Versions.mmkv_version}"
    const val xCrash = "com.iqiyi.xcrash:xcrash-android-lib:3.0.0"
    const val boostMultidex =
        "com.bytedance.boost_multidex:boost_multidex:${Versions.ARTIFACT_VERSION}"
    const val relinker = "pl.droidsonroids:relinker:${Versions.relinker_version}"
    const val guava = "com.google.guava:guava:30.1-android"
    const val material = "com.google.android.material:material:1.4.0"
    const val compress = "org.apache.commons:commons-compress:1.20"
    const val svg = "com.caverock:androidsvg-aar:${Versions.android_svg_version}"
    const val checkQual = "org.checkerframework:checker-qual:3.3.0"
    const val checkerCompatQual = "org.checkerframework:checker-compat-qual:2.5.5"
    const val lottie = "com.airbnb.android:lottie:4.1.0"
    const val javapoet = "com.squareup:javapoet:1.13.0"

    object Exoplayer {
        const val core = "com.google.android.exoplayer:exoplayer-core:${Versions.exo_player}"
        const val dash = "com.google.android.exoplayer:exoplayer-dash:${Versions.exo_player}"
        const val ui = "com.google.android.exoplayer:exoplayer-ui:${Versions.exo_player}"
    }

    object Glide {
        const val glide = "com.github.bumptech.glide:glide:${Versions.glide_version}"
        const val compiler = "com.github.bumptech.glide:compiler:${Versions.glide_version}"
    }

    object Coil {
        const val coil = "io.coil-kt:coil:1.2.1"
        const val svg = "io.coil-kt:coil-svg:1.2.1"
    }

    object AutoService {
        const val autoService = "com.google.auto.service:auto-service:1.0"
//        const val autoService = "com.google.auto.service:auto-service:1.0-rc7"

        const val autoServiceAnnotation = "com.google.auto.service:auto-service-annotations:1.0"
    }
}

object AndroidX {

    const val appcompat = "androidx.appcompat:appcompat:${Versions.appcompat_version}"
    const val core="androidx.core:core:${Versions.core_version}"
    const val collection="androidx.collection:collection:1.1.0"
    const val viewpager2 = "androidx.viewpager2:viewpager2:${Versions.viewpager2_version}"
    const val constraintlayout =
        "androidx.constraintlayout:constraintlayout:${Versions.constraintlayout_version}"
    const val annotation = "androidx.annotation:annotation:${Versions.annotation_version}"
    const val exifinterface = "androidx.exifinterface:exifinterface:1.3.3"
    const val fragment = "androidx.fragment:fragment:${Versions.fragment_version}"
    const val vectorDrawableAnimated = "androidx.vectordrawable:vectordrawable-animated:1.1.0"
    const val activity = "androidx.activity:activity:${Versions.activity_version}"
    const val swiperefreshlayout = "androidx.swiperefreshlayout:swiperefreshlayout:1.1.0"

    object Compose {
        const val snapshot = ""
        private const val version = Versions.compose_version
        const val activity = "androidx.activity:activity-compose:1.3.0-rc01"

        // Integration with ViewModels
        const val viewmodel = "androidx.lifecycle:lifecycle-viewmodel-compose:1.0.0-alpha07"

        // Integration with observables
        const val livedata = "androidx.compose.runtime:runtime-livedata:$version"
        const val runtime = "androidx.compose.runtime:runtime:$version"
        const val runtimeLivedata = "androidx.compose.runtime:runtime-livedata:$version"
        const val material = "androidx.compose.material:material:$version"
        const val foundation = "androidx.compose.foundation:foundation:$version"
        const val materialIconsCore = "androidx.compose.material:material-icons-core:$version"
        const val materialIconsExtended =
            "androidx.compose.material:material-icons-extended:$version"
        const val layout = "androidx.compose.foundation:foundation-layout:$version"
        const val tooling = "androidx.compose.ui:ui-tooling:$version"
        const val ui = "androidx.compose.ui:ui:$version"
        const val animation = "androidx.compose.animation:animation:$version"
        const val uiTest = "androidx.compose.ui:ui-test-junit4:$version"
    }

    object Lifecycle {
        private const val version = Versions.lifecycle_version
        const val viewModelKtx = "androidx.lifecycle:lifecycle-viewmodel-ktx:$version"
        const val lifecycleRuntimeKtx="androidx.lifecycle:lifecycle-runtime-ktx:$version"
    }

    object Room {
        private const val version = Versions.room_version
        const val roomKtx = "androidx.room:room-ktx:$version"
        const val roomCompiler = "androidx.room:room-compiler:$version"

    }

    object RecyclerView {
        private const val version = Versions.recyclerview_version
        const val recyclerView = "androidx.recyclerview:recyclerview:$version"
    }

    object Navigation {
        private const val version = Versions.nav_version
        const val navigationFragmentKtx = "androidx.navigation:navigation-fragment-ktx:$version"
        const val navigationUiKtx = "androidx.navigation:navigation-ui-ktx:$version"
    }

    object Paging {
        private const val version = Versions.paging_version
        const val pagingRuntimeKtx = "androidx.paging:paging-runtime-ktx:$version"

    }

    object Test {
        private const val version = "1.3.0"
        const val core = "androidx.test:core:$version"
        const val rules = "androidx.test:rules:$version"

        object Ext {
            private const val version = "1.1.2"
            const val junit = "androidx.test.ext:junit-ktx:$version"
        }

        const val espressoCore = "androidx.test.espresso:espresso-core:3.3.0"
    }
}

object Hilt {
    private const val version = "2.31.2-alpha"

    const val gradlePlugin = "com.google.dagger:hilt-android-gradle-plugin:$version"
    const val android = "com.google.dagger:hilt-android:$version"
    const val compiler = "com.google.dagger:hilt-compiler:$version"
    const val testing = "com.google.dagger:hilt-android-testing:$version"
}

object JUnit {
    private const val version = "4.13.12"
    const val junit = "junit:junit:$version"
}
