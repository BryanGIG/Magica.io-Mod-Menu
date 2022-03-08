package com.polarmods

import android.annotation.SuppressLint
import android.app.Service
import android.content.Intent
import android.graphics.Color
import android.graphics.PixelFormat
import android.graphics.Typeface
import android.graphics.drawable.Drawable
import android.os.Build
import android.os.IBinder
import android.view.*
import android.view.View.OnTouchListener
import android.view.animation.AlphaAnimation
import android.view.animation.Animation
import android.widget.*
import java.io.IOException

class Menu : Service() {
    private lateinit var windowManager: WindowManager
    private var frameLayout: FrameLayout? = null
    private var linearLayout2: LinearLayout? = null
    private var relativeLayout: RelativeLayout? = null
    private var linearLayout: LinearLayout? = null

    override fun onCreate() {
        super.onCreate()
        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        NativeFunctions.init()
    }

    override fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        FloatButton()
        return START_NOT_STICKY
    }

    override fun onTaskRemoved(rootIntent: Intent) {
        super.onTaskRemoved(rootIntent)
        stopSelf()
    }

    override fun onDestroy() {
        super.onDestroy()
        if (frameLayout!!.rootView != null) {
            windowManager.removeView(frameLayout!!.rootView)
        }
    }

    private fun FloatButton() {
        val params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            if (Build.VERSION.SDK_INT >= 26) {
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
            } else {
                WindowManager.LayoutParams.TYPE_PHONE
            },
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.START
        params.x = 0
        params.y = 100
        buildMenu()
        startFeatures()
        windowManager.addView(frameLayout, params)
        frameLayout!!.rootView.setOnTouchListener(object : OnTouchListener {
            private var initialX = 0
            private var initialY = 0
            private var initialTouchX = 0f
            private var initialTouchY = 0f
            @SuppressLint("ClickableViewAccessibility")
            override fun onTouch(v: View, event: MotionEvent): Boolean {
                if (event.action == MotionEvent.ACTION_DOWN) {
                    initialX = params.x
                    initialY = params.y
                    initialTouchX = event.rawX
                    initialTouchY = event.rawY
                    return true
                } else if (event.action == MotionEvent.ACTION_UP) {
                    val differenceX = Math.abs(initialTouchX - event.rawX).toInt()
                    val differenceY = Math.abs(initialTouchY - event.rawY).toInt()
                    if (differenceX < 10 && differenceY < 10 && relativeLayout!!.visibility == View.VISIBLE) {
                        val fadeOut: Animation = AlphaAnimation(0F, 1F)
                        fadeOut.duration = 500
                        linearLayout!!.startAnimation(fadeOut)
                        relativeLayout!!.visibility = View.GONE
                        linearLayout!!.visibility = View.VISIBLE
                    }
                    return true
                } else if (event.action == MotionEvent.ACTION_MOVE) {
                    params.x = initialX + (event.rawX - initialTouchX).toInt()
                    params.y = initialY + (event.rawY - initialTouchY).toInt()
                    windowManager.updateViewLayout(frameLayout, params)
                    return true
                }
                return false
            }
        })
    }

    private fun startFeatures() {
        val featuresList = NativeFunctions.getFeatures()
        for (i in featuresList.indices) {
            val feature = featuresList[i]
            val featureTypes = feature.split("_").toTypedArray()[0]
            val featureNames = feature.split("_").toTypedArray()[1]
            if (featureTypes.contains("Switch")) {
                addSwitch(featureNames) { buttonView, isChecked -> NativeFunctions.changeSwitch(i) }
            }
        }
    }

    @SuppressLint("SetTextI18n")
    private fun buildMenu() {
        frameLayout = FrameLayout(this)
        frameLayout!!.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        relativeLayout = RelativeLayout(this)
        relativeLayout!!.layoutParams = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        frameLayout!!.addView(relativeLayout)
        val imageView = ImageView(this)
        imageView.layoutParams =
            RelativeLayout.LayoutParams(convertDipToPixels(60.0f), convertDipToPixels(60.0f))
        try {
            val open = assets.open("ic_float.png")
            imageView.setImageDrawable(Drawable.createFromStream(open, null))
            open.close()
        } catch (e: IOException) {
            e.printStackTrace()
        }
        relativeLayout!!.addView(imageView)
        linearLayout = LinearLayout(this)
        linearLayout!!.layoutParams = LinearLayout.LayoutParams(
            convertDipToPixels(260.0f),
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        linearLayout!!.setBackgroundColor(Color.parseColor("#37000000"))
        linearLayout!!.orientation = LinearLayout.VERTICAL
        linearLayout!!.setPadding(0, 0, 0, convertDipToPixels(4.0f))
        linearLayout!!.visibility = View.INVISIBLE
        linearLayout!!.weightSum = 1.0f
        frameLayout!!.addView(linearLayout)
        val scrollView = ScrollView(this)
        scrollView.layoutParams = FrameLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            convertDipToPixels(170.0f)
        )
        scrollView.scrollBarSize = convertDipToPixels(7.0f)
        linearLayout!!.addView(scrollView)
        linearLayout2 = LinearLayout(this)
        val layoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        layoutParams.gravity = Gravity.CENTER
        linearLayout2!!.orientation = LinearLayout.VERTICAL
        linearLayout2!!.layoutParams = layoutParams
        scrollView.addView(linearLayout2)
        val textView = TextView(this)
        val layoutParams2 =
            LinearLayout.LayoutParams(convertDipToPixels(70.0f), convertDipToPixels(38.0f))
        textView.text = "Close"
        textView.textSize = 25f
        textView.setTextColor(Color.WHITE)
        textView.layoutParams = layoutParams2
        linearLayout!!.addView(textView)
        textView.setOnClickListener { view: View? ->
            val fadeIn: Animation = AlphaAnimation(1F, 0F)
            fadeIn.duration = 500
            linearLayout!!.startAnimation(fadeIn)
            val fadeOut: Animation = AlphaAnimation(0F, 1F)
            fadeIn.duration = 500
            relativeLayout!!.startAnimation(fadeOut)
            relativeLayout!!.visibility = View.VISIBLE
            linearLayout!!.visibility = View.GONE
        }
        textView.setOnLongClickListener { view: View? ->
            val isKilled = stopSelfResult(1)
            if (isKilled) {
                Toast.makeText(this, "Successfully killed menu!", Toast.LENGTH_LONG).show()
            } else {
                Toast.makeText(this, "Failed to kill menu!", Toast.LENGTH_LONG).show()
            }
            false
        }
        val fadeIn: Animation = AlphaAnimation(0F, 1F)
        fadeIn.duration = 1500
        relativeLayout!!.startAnimation(fadeIn)
    }

    private fun convertDipToPixels(f: Float): Int {
        return (f * resources.displayMetrics.density + 0.5f).toInt()
    }

    private fun addSwitch(name: String, listener: CompoundButton.OnCheckedChangeListener) {
        @SuppressLint("UseSwitchCompatOrMaterialCode") val sw = Switch(this)
        val localLayoutParams = LinearLayout.LayoutParams(-1, -1)
        localLayoutParams.gravity = 16
        sw.text = name
        sw.textSize = 18.0f
        sw.setTextColor(-1)
        sw.setOnCheckedChangeListener(listener)
        sw.layoutParams = localLayoutParams
        linearLayout2!!.addView(sw)
    }

    private fun addCategory(txt: String) {
        /*LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, convertDipToPixels(31.0f)));
        linearLayout.setBackgroundColor(Color.parseColor("#37000000"));
        linearLayout2.addView(linearLayout);

        TranslateAnimation translateAnimation = new TranslateAnimation(500f, -500f, 0f, 0f);
        translateAnimation.setDuration(10000);
        translateAnimation.setInterpolator(new LinearInterpolator());
        translateAnimation.setRepeatCount(Animation.INFINITE);
        translateAnimation.setRepeatMode(Animation.RESTART);*/
        val textView = TextView(this)
        textView.text = txt
        textView.gravity = Gravity.CENTER
        textView.setTextColor(-1)
        textView.textSize = 18.0f
        textView.setTypeface(null, Typeface.BOLD)
        textView.setPadding(0, 5, 0, 5)
        //textView.startAnimation(translateAnimation);
        //linearLayout.addView(textView);
        linearLayout2!!.addView(textView)
    }

    override fun onBind(intent: Intent): IBinder? {
        return null
    }
}