package com.polarmods

import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        startFloating(this)
    }

    companion object {
        @JvmStatic
        fun startFloating(context: Context) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                if (!Settings.canDrawOverlays(context)) {
                    val intent = Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:${context.packageName}"))
                    context.startActivity(intent)
                } else {
                    val intent = Intent(context, Menu::class.java)
                    context.startService(intent)
                }
            } else {
                val intent = Intent(context, Menu::class.java)
                context.startService(intent)
            }
        }
    }
}