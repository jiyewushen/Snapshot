import org.gradle.kotlin.dsl.`kotlin-dsl`

plugins {
    `kotlin-dsl`
}

repositories {
    google()
    mavenCentral()
}

dependencies {
    //引用gradle api
    implementation(gradleApi())
    //引用android gradle api, 里面包含transform api
    implementation("com.android.tools.build:gradle:7.0.0")
    implementation("commons-io:commons-io:2.11.0")

    //引用asm api
    implementation("org.ow2.asm:asm:9.2")
    implementation("org.ow2.asm:asm-commons:9.2")
}

