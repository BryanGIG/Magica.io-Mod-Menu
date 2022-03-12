@file:Suppress("unused")

package com.polarmods

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.PorterDuff
import android.os.Process
import android.view.View

class ESPView(context: Context?) : View(context, null, 0), Runnable {
    var mStrokePaint: Paint? = null
    var mFilledPaint: Paint? = null
    var mTextPaint: Paint? = null
    var mThread: Thread
    var FPS = 60
    var sleepTime: Long

    external fun DrawOn(canvas: Canvas?)

    init {
        InitializePaints()
        isFocusableInTouchMode = false
        setBackgroundColor(Color.TRANSPARENT)
        sleepTime = 1000L / FPS
        mThread = Thread(this)
        mThread.start()
    }

    override fun onDraw(canvas: Canvas) {
        if (visibility == VISIBLE) {
            ClearCanvas(canvas)
            DrawOn(canvas)
        }
    }

    override fun run() {
        Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND)
        var nextFrameUpdateTime = System.currentTimeMillis()
        while (mThread.isAlive && !mThread.isInterrupted) {
            // Sleep
            try {
                var currentTime = System.currentTimeMillis()
                postInvalidate()
                currentTime += FPS.toLong()
                val difference = currentTime - nextFrameUpdateTime
                Thread.sleep(difference)
                nextFrameUpdateTime = System.currentTimeMillis()
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    fun InitializePaints() {
        mStrokePaint = Paint()
        mStrokePaint!!.style = Paint.Style.STROKE
        mStrokePaint!!.isAntiAlias = true
        mStrokePaint!!.color = Color.rgb(0, 0, 0)
        mFilledPaint = Paint()
        mFilledPaint!!.style = Paint.Style.FILL
        mFilledPaint!!.isAntiAlias = true
        mFilledPaint!!.color = Color.rgb(0, 0, 0)
        mTextPaint = Paint()
        mTextPaint!!.style = Paint.Style.FILL_AND_STROKE
        mTextPaint!!.isAntiAlias = true
        mTextPaint!!.color = Color.rgb(0, 0, 0)
        mTextPaint!!.textAlign = Paint.Align.CENTER
        mTextPaint!!.strokeWidth = 1.1f
    }

    fun ClearCanvas(cvs: Canvas) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
    }
    fun DrawLine(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        lineWidth: Float,
        fromX: Float,
        fromY: Float,
        toX: Float,
        toY: Float
    ) {
        mStrokePaint!!.color = Color.rgb(r, g, b)
        mStrokePaint!!.alpha = a
        mStrokePaint!!.strokeWidth = lineWidth
        cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint!!)
    }

    fun DrawText(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        txt: String?,
        posX: Float,
        posY: Float,
        size: Float
    ) {
        mTextPaint!!.color = Color.rgb(r, g, b)
        mTextPaint!!.alpha = a
        if (right > 1920 || bottom > 1920) mTextPaint!!.textSize =
            4 + size else if (right == 1920 || bottom == 1920) mTextPaint!!.textSize =
            2 + size else mTextPaint!!.textSize = size
        cvs.drawText(txt!!, posX, posY, mTextPaint!!)
    }

    fun DrawCircle(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        stroke: Float,
        posX: Float,
        posY: Float,
        radius: Float
    ) {
        mStrokePaint!!.color = Color.rgb(r, g, b)
        mStrokePaint!!.alpha = a
        mStrokePaint!!.strokeWidth = stroke
        cvs.drawCircle(posX, posY, radius, mStrokePaint!!)
    }

    fun DrawFilledCircle(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        posX: Float,
        posY: Float,
        radius: Float
    ) {
        mFilledPaint!!.color = Color.rgb(r, g, b)
        mFilledPaint!!.alpha = a
        cvs.drawCircle(posX, posY, radius, mFilledPaint!!)
    }

    fun DrawRect(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        stroke: Int,
        x: Float,
        y: Float,
        width: Float,
        height: Float
    ) {
        mStrokePaint!!.strokeWidth = stroke.toFloat()
        mStrokePaint!!.color = Color.rgb(r, g, b)
        mStrokePaint!!.alpha = a
        cvs.drawRect(x, y, x + width, y + height, mStrokePaint!!)
    }

    fun DrawFilledRect(
        cvs: Canvas,
        a: Int,
        r: Int,
        g: Int,
        b: Int,
        x: Float,
        y: Float,
        width: Float,
        height: Float
    ) {
        mFilledPaint!!.color = Color.rgb(r, g, b)
        mFilledPaint!!.alpha = a
        cvs.drawRect(x, y, x + width, y + height, mFilledPaint!!)
    }
}