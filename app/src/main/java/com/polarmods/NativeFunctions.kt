package com.polarmods

object NativeFunctions {
    init {
        System.loadLibrary("polarmods")
    }

    external fun init()
    external fun changeSwitch(id: Int)
    external fun getFeatures(): Array<String>
}