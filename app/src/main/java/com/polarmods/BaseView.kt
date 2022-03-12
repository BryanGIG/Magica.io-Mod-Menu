package com.polarmods

import android.annotation.SuppressLint
import android.app.AlertDialog
import android.content.Context
import android.content.Intent
import android.content.res.ColorStateList
import android.graphics.BitmapFactory
import android.graphics.Color
import android.graphics.PorterDuff
import android.graphics.Typeface
import android.graphics.drawable.GradientDrawable
import android.net.Uri
import android.os.Build
import android.text.Html
import android.text.InputFilter
import android.text.InputType
import android.text.TextUtils
import android.text.method.DigitsKeyListener
import android.util.Base64
import android.util.Log
import android.util.TypedValue
import android.view.Gravity
import android.view.View
import android.view.View.OnClickListener
import android.view.View.OnFocusChangeListener
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.webkit.WebView
import android.widget.*
import java.util.*
import java.util.regex.Pattern
import kotlin.math.max

@SuppressLint("SetTextI18n")
class BaseView(val ctx: FloatingMod) : FrameLayout(ctx) {
    private var TEXT_COLOR = Color.parseColor("#82CAFD")
    private var TEXT_COLOR_2 = Color.parseColor("#FFFFFF")
    private var BTN_COLOR = Color.parseColor("#1C262D")
    private var MENU_BG_COLOR = Color.parseColor("#EE1C2A35") //#AARRGGBB
    private var MENU_FEATURE_BG_COLOR = Color.parseColor("#DD141C22") //#AARRGGBB
    private var MENU_WIDTH = 290
    private var MENU_HEIGHT = 210
    private var MENU_CORNER = 1f
    private var ICON_SIZE = 50 //Change both width and height of image
    private var ICON_ALPHA = 0.7f //Transparent
    private var HintTxtColor = Color.parseColor("#FF171E24")
    private var ToggleON = Color.GREEN
    private var ToggleOFF = Color.RED
    private var BtnON = Color.parseColor("#1b5e20")
    private var BtnOFF = Color.parseColor("#7f0000")
    private var CategoryBG = Color.parseColor("#2F3D4C")
    private var SeekBarColor = Color.parseColor("#80CBC4")
    private var SeekBarProgressColor = Color.parseColor("#80CBC4")
    private var CheckBoxColor = Color.parseColor("#80CBC4")
    private var RadioColor = Color.parseColor("#FFFFFF")
    private var NumberTxtColor = "#41c300"

    private lateinit var title: TextView
    private lateinit var heading: TextView
    private lateinit var patches: LinearLayout
    lateinit var mCollapsed: RelativeLayout
    lateinit var mExpanded: LinearLayout
    lateinit var startimage: ImageView

    external fun Changes(feature: String, value: Int, value2: String?)

    fun changeIcon(base64: String) {
        val decode = Base64.decode(base64, 0)
        startimage.setImageBitmap(BitmapFactory.decodeByteArray(decode, 0, decode.size))
    }

    private fun init() {
        val mRootContainer = RelativeLayout(context)
        mCollapsed = RelativeLayout(context)
        mCollapsed.visibility = View.VISIBLE
        mCollapsed.alpha = ICON_ALPHA

        //********** The box of the mod menu **********
        mExpanded = LinearLayout(context) // Menu markup (when the menu is expanded)
        //        mExpanded.setVisibility(View.GONE);
        mExpanded.setBackgroundColor(MENU_BG_COLOR)
        mExpanded.gravity = Gravity.CENTER
        mExpanded.orientation = LinearLayout.VERTICAL
        mExpanded.layoutParams = LinearLayout.LayoutParams(
            dp(MENU_WIDTH),
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        val gdMenuBody = GradientDrawable()
        gdMenuBody.cornerRadius = MENU_CORNER //Set corner
        gdMenuBody.setColor(MENU_BG_COLOR) //Set background color
        mExpanded.background = gdMenuBody //Apply aninmation to it
        startimage = ImageView(context)
        startimage.layoutParams = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        val applyDimension: Int = TypedValue.applyDimension(
            TypedValue.COMPLEX_UNIT_DIP,
            ICON_SIZE.toFloat(),
            resources.displayMetrics
        ).toInt() //Icon size
        startimage.layoutParams.height = applyDimension
        startimage.layoutParams.width = applyDimension
        startimage.requestLayout()
        startimage.scaleType = ImageView.ScaleType.FIT_XY
        (startimage.layoutParams as MarginLayoutParams).topMargin = convertDipToPixels(10)
        startimage.setOnClickListener {
            mCollapsed.visibility = View.GONE
            mExpanded.visibility = View.VISIBLE
        }

        //********** Title text **********
        val titleText = RelativeLayout(context)
        titleText.setPadding(10, 5, 10, 5)
        titleText.setVerticalGravity(16)
        title = TextView(context)
        title.text = "Polarmods.com"
        title.setTextColor(TEXT_COLOR)
        title.textSize = 18.0f
        title.gravity = Gravity.CENTER
        val rl: RelativeLayout.LayoutParams = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        rl.addRule(RelativeLayout.CENTER_HORIZONTAL)
        title.layoutParams = rl

        //********** Heading text **********
        heading = TextView(context)
        heading.text = "This mod was provided by @BryanGIG"
        heading.ellipsize = TextUtils.TruncateAt.MARQUEE
        heading.marqueeRepeatLimit = -1
        heading.isSingleLine = true
        heading.isSelected = true
        heading.setTextColor(TEXT_COLOR)
        heading.textSize = 10.0f
        heading.gravity = Gravity.CENTER
        heading.setPadding(0, 0, 0, 5)

        //********** Mod menu feature list **********
        val scrollView = ScrollView(context)
        //Auto size. To set size manually, change the width and height example 500, 500
        val scrlLL: LinearLayout.LayoutParams =
            LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, dp(MENU_HEIGHT))
        scrollView.layoutParams = scrlLL
        scrollView.setBackgroundColor(MENU_FEATURE_BG_COLOR)
        patches = LinearLayout(context)
        patches.orientation = LinearLayout.VERTICAL
        //**********  Hide/Kill button **********
        val relativeLayout = RelativeLayout(context)
        relativeLayout.setPadding(10, 3, 10, 3)
        relativeLayout.setVerticalGravity(Gravity.CENTER)
        val hideBtn = Button(context)
        hideBtn.setBackgroundColor(Color.TRANSPARENT)
        hideBtn.text = "HIDE/KILL (Hold)"
        hideBtn.setTextColor(TEXT_COLOR)
        hideBtn.setOnClickListener { view: View ->
            mCollapsed.visibility = View.VISIBLE
            mCollapsed.alpha = 0f
            mExpanded.visibility = View.GONE
            Toast.makeText(
                view.context,
                "Icon hidden. Remember the hidden icon position",
                Toast.LENGTH_LONG
            ).show()
        }

        hideBtn.setOnLongClickListener {
            Toast.makeText(context, "Menu service killed", Toast.LENGTH_LONG).show()
            ctx.stopSelf()
            true
        }

        //********** Close button **********
        val closeBtn = Button(context)
        closeBtn.setBackgroundColor(Color.TRANSPARENT)
        closeBtn.text = "MINIMIZE"
        closeBtn.setTextColor(TEXT_COLOR)
        closeBtn.setOnClickListener { view: View? ->
            mCollapsed.visibility = View.VISIBLE
            mCollapsed.alpha = ICON_ALPHA
            mExpanded.visibility = View.GONE
        }
        val layoutParams: RelativeLayout.LayoutParams = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )
        layoutParams.addRule(RelativeLayout.ALIGN_PARENT_RIGHT)
        closeBtn.layoutParams = layoutParams

        //********** Params **********
        //Variable to check later if the phone supports Draw over other apps permission
        addView(mRootContainer)
        mRootContainer.addView(mCollapsed)
        mRootContainer.addView(mExpanded)
        mCollapsed.addView(startimage)
        titleText.addView(title)
        mExpanded.addView(titleText)
        mExpanded.addView(heading)
        scrollView.addView(patches)
        mExpanded.addView(scrollView)
        relativeLayout.addView(hideBtn)
        relativeLayout.addView(closeBtn)
        mExpanded.addView(relativeLayout)
    }

    private fun featureList(listFT: Array<String>) {
        var featureNum: Int
        var subFeat = 0
        for (i in listFT.indices) {
            if (listFT[i] != "") {
                var switchedOn = false
                var feature = listFT[i]
                if (feature.contains("True_")) {
                    switchedOn = true
                    feature = feature.replaceFirst("True_".toRegex(), "")
                }
                val str = feature.split("_").toTypedArray()
                if (TextUtils.isDigitsOnly(str[0]) || str[0].matches("-[0-9]*".toRegex())) {
                    featureNum = str[0].toInt()
                    feature = feature.replaceFirst((str[0] + "_").toRegex(), "")
                    subFeat++
                } else {
                    //Subtract feature number. We don't want to count ButtonLink, Category, RichTextView and RichWebView
                    featureNum = i - subFeat
                }
                val strSplit = feature.split("_").toTypedArray()
                when (strSplit[0]) {
                    "Toggle" -> patches.addView(Switch(featureNum, strSplit[1], switchedOn))
                    "SeekBar" -> patches.addView(
                        SeekBar(
                            featureNum,
                            strSplit[1],
                            strSplit[2].toInt(),
                            strSplit[3].toInt()
                        )
                    )
                    "Button" -> patches.addView(Button(featureNum, strSplit[1]))
                    "ButtonOnOff" -> patches.addView(
                        ButtonOnOff(
                            featureNum,
                            strSplit[1],
                            switchedOn
                        )
                    )
                    "Spinner" -> {
                        patches.addView(RichTextView(strSplit[1]))
                        patches.addView(Spinner(featureNum, strSplit[1], strSplit[2]))
                    }
                    "InputText" -> patches.addView(TextField(featureNum, strSplit[1], false))
                    "InputValue" -> patches.addView(TextField(featureNum, strSplit[1], true))
                    "CheckBox" -> patches.addView(
                        CheckBox(
                            featureNum,
                            strSplit[1],
                            switchedOn
                        )
                    )
                    "RadioButton" -> patches.addView(
                        RadioButton(
                            featureNum,
                            strSplit[1],
                            strSplit[2]
                        )
                    )
                    "ButtonLink" -> {
                        subFeat++
                        patches.addView(ButtonLink(strSplit[1], strSplit[2]))
                    }
                    "Category" -> {
                        subFeat++
                        patches.addView(Category(strSplit[1]))
                    }
                    "RichTextView" -> {
                        subFeat++
                        patches.addView(RichTextView(strSplit[1]))
                    }
                    "RichWebView" -> {
                        subFeat++
                        patches.addView(RichWebView(strSplit[1]))
                    }
                }
            }
        }
    }

    private fun Switch(featureNum: Int, featureName: String, switchedOn: Boolean): View {
        val switchR = Switch(context)
        val buttonStates = ColorStateList(
            arrayOf(
                intArrayOf(-android.R.attr.state_enabled),
                intArrayOf(android.R.attr.state_checked),
                intArrayOf()
            ), intArrayOf(
                Color.BLUE,
                ToggleON,  // ON
                ToggleOFF // OFF
            )
        )
        //Set colors of the switch. Comment out if you don't like it
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            switchR.thumbDrawable.setTintList(buttonStates)
            switchR.trackDrawable.setTintList(buttonStates)
        }
        switchR.setOnCheckedChangeListener { _, isChecked: Boolean ->
            Changes(
                featureName,
                isChecked.toInt(), null
            )
        }
        switchR.isChecked = switchedOn
        switchR.text = featureName
        switchR.setTextColor(TEXT_COLOR_2)
        switchR.setPadding(10, 5, 0, 5)
        return switchR
    }

    private fun SeekBar(featureNum: Int, featureName: String, min: Int, max: Int): View {
        val linearLayout = LinearLayout(context)
        linearLayout.setPadding(10, 5, 0, 5)
        linearLayout.orientation = LinearLayout.VERTICAL
        linearLayout.gravity = Gravity.CENTER
        val textView = TextView(context)
        textView.text =
            toHTML("<font face='roboto'>$featureName: <font color='$NumberTxtColor'>$min</font>")
        textView.setTextColor(TEXT_COLOR_2)
        val seekBar = SeekBar(context)
        seekBar.setPadding(25, 10, 35, 10)
        seekBar.max = max
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) seekBar.min =
            min //setMin for Oreo and above
        seekBar.progress = min
        seekBar.thumb.setColorFilter(SeekBarColor, PorterDuff.Mode.SRC_ATOP)
        seekBar.progressDrawable.setColorFilter(SeekBarProgressColor, PorterDuff.Mode.SRC_ATOP)
        seekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onStartTrackingTouch(seekBar: SeekBar) {}
            override fun onStopTrackingTouch(seekBar: SeekBar) {}
            override fun onProgressChanged(seekBar: SeekBar, i: Int, z: Boolean) {
                //if progress is greater than minimum, don't go below. Else, set progress
                seekBar.progress = max(i, min)
                Changes(featureName, i, null)
                textView.text = toHTML(
                    "<font face='roboto'>$featureName: <font color='$NumberTxtColor'>${
                        max(
                            i,
                            min
                        )
                    } </font>"
                )
            }
        })
        linearLayout.addView(textView)
        linearLayout.addView(seekBar)
        return linearLayout
    }

    private fun Button(featureNum: Int, featureName: String): View {
        val button = Button(context)
        val layoutParams: LinearLayout.LayoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        layoutParams.setMargins(7, 5, 7, 5)
        button.layoutParams = layoutParams
        // button.setTextSize(13.0f);
        button.setTextColor(TEXT_COLOR_2)
        button.isAllCaps = false //Disable caps to support html
        button.text = toHTML(featureName)
        button.setBackgroundColor(BTN_COLOR)
        button.setOnClickListener { Changes(featureName, 1, null) }
        return button
    }

    private fun ButtonLink(featureName: String, url: String): View {
        val button = Button(context)
        val layoutParams: LinearLayout.LayoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        layoutParams.setMargins(7, 5, 7, 5)
        button.layoutParams = layoutParams
        button.isAllCaps = false //Disable caps to support html
        button.setTextColor(TEXT_COLOR_2)
        button.text = toHTML(featureName)
        button.setBackgroundColor(BTN_COLOR)
        button.setOnClickListener { v: View? ->
            val intent = Intent(Intent.ACTION_VIEW)
            intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK
            intent.data = Uri.parse(url)
            context.startActivity(intent)
        }
        return button
    }

    private fun ButtonOnOff(featureNum: Int, featureName: String, switchedOn: Boolean): View {
        val button = Button(context)
        val layoutParams: LinearLayout.LayoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        layoutParams.setMargins(7, 5, 7, 5)
        button.layoutParams = layoutParams
        button.setTextColor(TEXT_COLOR_2)
        button.isAllCaps = false //Disable caps to support html
        val finalFeatureName = featureName.replace("OnOff_", "")
        val finalIsOn = false
        button.setOnClickListener(object : OnClickListener {
            var isOn = finalIsOn
            override fun onClick(v: View) {
                Changes(featureName, isOn.toInt(), null)
                if (isOn) {
                    button.text = toHTML("$finalFeatureName: ON")
                    button.setBackgroundColor(BtnON)
                    isOn = false
                } else {
                    button.text = toHTML("$finalFeatureName: OFF")
                    button.setBackgroundColor(BtnOFF)
                    isOn = true
                }
            }
        })
        if (switchedOn) button.performClick()
        return button
    }

    private fun Spinner(featureNum: Int, featureName: String, list: String): View {
        val lists: List<String> = LinkedList(listOf(*list.split(",").toTypedArray()))

        // Create another LinearLayout as a workaround to use it as a background
        // and to keep the 'down' arrow symbol
        // If spinner had the setBackgroundColor set, there would be no arrow symbol
        val linearLayout2 = LinearLayout(context)
        val layoutParams2: LinearLayout.LayoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        layoutParams2.setMargins(10, 2, 10, 5)
        linearLayout2.orientation = LinearLayout.VERTICAL
        linearLayout2.setBackgroundColor(BTN_COLOR)
        linearLayout2.layoutParams = layoutParams2
        val spinner = Spinner(context, Spinner.MODE_DROPDOWN)
        spinner.setPadding(5, 10, 5, 8)
        spinner.layoutParams = layoutParams2
        spinner.background.setColorFilter(
            1,
            PorterDuff.Mode.SRC_ATOP
        ) //trick to show white down arrow color
        //Creating the ArrayAdapter instance having the list
        val aa: ArrayAdapter<String> =
            ArrayAdapter<String>(context, android.R.layout.simple_spinner_item, lists)
        aa.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        //Setting the ArrayAdapter data on the Spinner'
        spinner.adapter = aa
        //        spinner.setSelection(featureName);
        spinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parentView: AdapterView<*>,
                selectedItemView: View,
                position: Int,
                id: Long
            ) {
                Changes(featureName, position, null)
                (parentView.getChildAt(0) as TextView).setTextColor(TEXT_COLOR_2)
                Toast.makeText(context, featureName, Toast.LENGTH_SHORT).show()
            }

            override fun onNothingSelected(parent: AdapterView<*>?) {}
        }
        linearLayout2.addView(spinner)
        return linearLayout2
    }

    private fun TextField(feature: Int, featureName: String, numOnly: Boolean): View {
        val edittextstring = EditTextString()
        val edittextnum = EditTextNum()
        val linearLayout = LinearLayout(context)
        val layoutParams: LinearLayout.LayoutParams = LinearLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        layoutParams.setMargins(7, 5, 7, 5)
        val button = Button(context)
        if (numOnly) {
            edittextnum.num = 1
            button.text =
                toHTML("<font face='roboto'>$featureName: <font color='$NumberTxtColor'>1</font></font>")
        } else {
            val string = ""
            edittextstring.string = string
            button.text =
                toHTML("<font face='roboto'>$featureName: <font color='$NumberTxtColor'>$string</font></font>")
        }
        button.isAllCaps = false
        button.layoutParams = layoutParams
        button.setBackgroundColor(BTN_COLOR)
        button.setTextColor(TEXT_COLOR_2)
        button.setOnClickListener {
            val alert = AlertDialog.Builder(context, 2).create()
            alert.window?.setType(if (Build.VERSION.SDK_INT >= 26) 2038 else 2002)
            alert.setOnCancelListener {
                val imm =
                    context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
                imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0)
            }

            //LinearLayout
            val linearLayout1 = LinearLayout(context)
            linearLayout1.setPadding(5, 5, 5, 5)
            linearLayout1.orientation = LinearLayout.VERTICAL
            linearLayout1.setBackgroundColor(MENU_FEATURE_BG_COLOR)

            //TextView
            val textViewNote = TextView(context)
            textViewNote.text = "Tap OK to apply changes. Tap outside to cancel"
            textViewNote.setTextColor(TEXT_COLOR_2)

            //Edit text
            val edittext = EditText(context)
            edittext.maxLines = 1
            edittext.width = convertDipToPixels(300)
            edittext.setTextColor(TEXT_COLOR_2)
            edittext.setHintTextColor(HintTxtColor)
            if (numOnly) {
                edittext.inputType = InputType.TYPE_CLASS_NUMBER
                edittext.keyListener = DigitsKeyListener.getInstance("0123456789-")
                edittext.setText(edittextnum.num.toString())
                val FilterArray = arrayOfNulls<InputFilter>(1)
                FilterArray[0] = InputFilter.LengthFilter(10)
                edittext.filters = FilterArray
            } else {
                edittext.setText(edittextstring.string)
            }
            edittext.onFocusChangeListener = OnFocusChangeListener { v: View?, hasFocus: Boolean ->
                val imm =
                    context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
                if (hasFocus) {
                    imm.toggleSoftInput(
                        InputMethodManager.SHOW_FORCED,
                        InputMethodManager.HIDE_IMPLICIT_ONLY
                    )
                } else {
                    imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0)
                }
            }
            edittext.requestFocus()


            //Button
            val btndialog = Button(context)
            btndialog.setBackgroundColor(BTN_COLOR)
            btndialog.setTextColor(TEXT_COLOR_2)
            btndialog.text = "OK"
            btndialog.setOnClickListener { view1: View? ->
                if (numOnly) {
                    val num: Int = try {
                        if (TextUtils.isEmpty(edittext.text.toString()))
                            0 else edittext.text.toString().toInt()
                    } catch (ex: NumberFormatException) {
                        2147483640
                    }
                    edittextnum.num = num
                    button.text =
                        toHTML("<font face='roboto'>$featureName: <font color='#41c300'>$num</font></font>")
                    alert.dismiss()
                    Changes(featureName, num, null)
                } else {
                    val str: String = edittext.text.toString()
                    edittextstring.string = edittext.text.toString()
                    button.text =
                        toHTML("<font face='roboto'>$featureName: <font color='#41c300'>$str</font></font>")
                    alert.dismiss()
                    Changes(featureName, -1000, str)
                }
                edittext.isFocusable = false
            }
            linearLayout1.addView(textViewNote)
            linearLayout1.addView(edittext)
            linearLayout1.addView(btndialog)
            alert.setView(linearLayout1)
            alert.show()
        }
        linearLayout.addView(button)
        return linearLayout
    }

    private fun toHTML(html: String): CharSequence {
        return if (hasHTMLTags(html)) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                Html.fromHtml(html, Html.FROM_HTML_MODE_LEGACY)
            } else {
                Html.fromHtml(html)
            }
        } else {
            html
        }
    }

    private fun CheckBox(featureNum: Int, featureName: String, switchedOn: Boolean): View {
        val checkBox = CheckBox(context)
        checkBox.text = featureName
        checkBox.setTextColor(TEXT_COLOR_2)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) checkBox.buttonTintList =
            ColorStateList.valueOf(CheckBoxColor)
        checkBox.setOnCheckedChangeListener { _, isChecked ->
            Changes(
                featureName, isChecked.toInt(), null
            )
        }
        checkBox.isChecked = switchedOn
        return checkBox
    }

    private fun RadioButton(featureNum: Int, featureName: String, list: String): View {
        val lists: List<String> = LinkedList(listOf(*list.split(",").toTypedArray()))
        val textView = TextView(context)
        textView.text = "$featureName:"
        textView.setTextColor(TEXT_COLOR_2)
        val radioGroup = RadioGroup(context)
        radioGroup.setPadding(10, 5, 10, 5)
        radioGroup.orientation = LinearLayout.VERTICAL
        radioGroup.addView(textView)
        for (i in lists.indices) {
            val Radioo = RadioButton(context)
            val radioName = lists[i]
            val first_radio_listener = OnClickListener { v: View? ->
                textView.text =
                    toHTML("<font face='roboto'>$featureName: <font color='$NumberTxtColor'>$radioName</font>")
                Changes(featureName, radioGroup.indexOfChild(Radioo), null)
            }
            println(lists[i])
            Radioo.text = lists[i]
            Radioo.setTextColor(Color.LTGRAY)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) Radioo.buttonTintList =
                ColorStateList.valueOf(RadioColor)
            Radioo.setOnClickListener(first_radio_listener)
            radioGroup.addView(Radioo)
        }
        return radioGroup
    }

    private fun Category(text: String): View {
        val textView = TextView(context)
        textView.setBackgroundColor(CategoryBG)
        textView.text = toHTML(text)
        textView.gravity = Gravity.CENTER
        textView.setTextColor(TEXT_COLOR_2)
        textView.setTypeface(null, Typeface.BOLD)
        textView.setPadding(0, 5, 0, 5)
        return textView
    }

    private fun RichTextView(text: String): View {
        val textView = TextView(context)
        textView.text = toHTML(text)
        textView.setTextColor(TEXT_COLOR_2)
        textView.setPadding(10, 5, 10, 5)
        return textView
    }

    private fun RichWebView(text: String): View {
        val wView = WebView(context)
        wView.loadData(text, "text/html", "utf-8")
        wView.setBackgroundColor(0x00000000) //Transparent
        wView.setPadding(0, 5, 0, 5)
        wView.settings.setAppCacheEnabled(false)
        wView.requestLayout()
        return wView
    }

    private class EditTextString {
        var string: String? = null
    }

    private class EditTextNum {
        var num = 0
    }

    //For our image a little converter
    private fun convertDipToPixels(i: Int): Int {
        return (i.toFloat() * resources.displayMetrics.density + 0.5f).toInt()
    }

    private fun dp(i: Int): Int {
        return TypedValue.applyDimension(
            TypedValue.COMPLEX_UNIT_DIP,
            i.toFloat(),
            resources.displayMetrics
        ).toInt()
    }

    private val pattern = Pattern.compile("<(\"[^\"]*\"|'[^']*'|[^'\">])*>")

    private fun hasHTMLTags(text: String): Boolean {
        val matcher = pattern.matcher(text)
        return matcher.find()
    }

    init {
        init()
        mExpanded.visibility = View.GONE
    }

    private fun Boolean.toInt(): Int {
        return if (this) 1 else 0
    }
}
