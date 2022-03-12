package com.polarmods

import android.annotation.SuppressLint
import android.app.ActivityManager
import android.app.Service
import android.content.Intent
import android.graphics.PixelFormat
import android.os.Build
import android.os.Handler
import android.os.IBinder
import android.os.Looper
import android.view.*
import java.util.*

@SuppressLint("ClickableViewAccessibility")
class FloatingMod : Service() {
    private lateinit var view: BaseView
    private lateinit var params: WindowManager.LayoutParams
    lateinit var mWindowManager: WindowManager
    private lateinit var rootFrame: View
    private val mHandler = Handler(Looper.getMainLooper())
    var esp: ESPView? = null

    override fun onBind(intent: Intent): IBinder? {
        return null
    }

    private external fun Init(ctx: BaseView?)

    override fun onStartCommand(intent: Intent, i: Int, i2: Int): Int {
        return START_NOT_STICKY
    }

    override fun onCreate() {
        super.onCreate()
        initBase()
        Timer().scheduleAtFixedRate(object : TimerTask() {
            override fun run() {
                thread()
            }
        }, 0, 1000)
    }

    @Suppress("DEPRECATION")
    private fun initBase() {
        //Floating View
        view = BaseView(this)
        rootFrame = view.rootView

        rootFrame.setOnTouchListener(onTouchListener())
        view.startimage.setOnTouchListener(onTouchListener())

        Init(view)

        val layoutType = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            WindowManager.LayoutParams.TYPE_PHONE
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            WindowManager.LayoutParams.TYPE_TOAST
        } else {
            WindowManager.LayoutParams.TYPE_SYSTEM_ALERT
        }

        params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            layoutType,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.START
        params.x = 0
        params.y = 100
        mWindowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        mWindowManager.addView(rootFrame, params)

        //ESP View
//        esp = ESPView(this)
//        val params2 = WindowManager.LayoutParams(
//            WindowManager.LayoutParams.MATCH_PARENT,
//            WindowManager.LayoutParams.MATCH_PARENT, 0, navigationBarHeight(),
//            layoutType,
//            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
//                    or WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE or WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN or WindowManager.LayoutParams.FLAG_FULLSCREEN,
//            PixelFormat.TRANSLUCENT
//        )
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
//            params2.layoutInDisplayCutoutMode =
//                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
//
//        params2.gravity = Gravity.TOP or Gravity.START
//        params2.x = 0
//        params2.y = 0
//        mWindowManager.addView(esp, params2)
    }

    private fun navigationBarHeight(): Int {
        val resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android")
        return if (resourceId > 0 && !ViewConfiguration.get(this)
                .hasPermanentMenuKey()
        ) resources.getDimensionPixelSize(resourceId) else 0
    }

    private val isNotInGame: Boolean
        get() {
            val runningAppProcessInfo = ActivityManager.RunningAppProcessInfo()
            ActivityManager.getMyMemoryState(runningAppProcessInfo)
            return runningAppProcessInfo.importance != 100
        }

    private fun thread() {
        if (isNotInGame) {
            mHandler.post {
                rootFrame.visibility = View.INVISIBLE
                esp?.visibility = View.INVISIBLE
            }
        } else {
            mHandler.post {
                rootFrame.visibility = View.VISIBLE
                esp?.visibility = View.VISIBLE
            }
        }
    }


    private fun onTouchListener(): View.OnTouchListener {
        return object : View.OnTouchListener {
            val collapsedView: View = view.mCollapsed
            val expandedView: View = view.mExpanded
            private var initialTouchX = 0f
            private var initialTouchY = 0f
            private var initialX = 0
            private var initialY = 0
            override fun onTouch(view: View, motionEvent: MotionEvent): Boolean {
                return when (motionEvent.action) {
                    MotionEvent.ACTION_DOWN -> {
                        initialX = params.x
                        initialY = params.y
                        initialTouchX = motionEvent.rawX
                        initialTouchY = motionEvent.rawY
                        true
                    }
                    MotionEvent.ACTION_UP -> {
                        val rawX: Int = (motionEvent.rawX - initialTouchX).toInt()
                        val rawY: Int = (motionEvent.rawY - initialTouchY).toInt()
                        if (rawX < 10 && rawY < 10 && isViewCollapsed) {
                            collapsedView.visibility = View.GONE
                            expandedView.visibility = View.VISIBLE
                        }
                        true
                    }
                    MotionEvent.ACTION_MOVE -> {
                        params.x = initialX + (motionEvent.rawX - initialTouchX).toInt()
                        params.y = initialY + (motionEvent.rawY - initialTouchY).toInt()
                        mWindowManager.updateViewLayout(rootFrame, params)
                        true
                    }
                    else -> false
                }
            }
        }
    }

    private val isViewCollapsed: Boolean
        get() = view.mCollapsed.visibility == View.VISIBLE

    override fun onDestroy() {
        mWindowManager.removeView(rootFrame)
        esp?.let { mWindowManager.removeView(it) }
    }

    companion object {
        init {
            System.loadLibrary("polarmods")
        }
    }
}